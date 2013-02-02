/*
 * @file RenderOutputWriter.cpp
 *
 * @date 02.12.2012
 * @author Jakob Weiss
 */

#ifndef NOGLVISUALIZER

#include "RenderOutputWriter.h"

#include "utils/Settings.h"

#include "utils/ColorCoding/Grayscale.h"

#include "../CellListContainer.h"

#include <signal.h>
#include <iostream>
#include <string>
#include <sstream>
#ifndef NOFREEGLUT
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

namespace outputWriter {

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat mat_shininess[] = { 10.0f };

CellListContainer *theContainer;

/**
 * global list containing the particles in the rendering
 */
std::vector<Particle> render3dParticles;

/**
 * OpenGL display list ID for the particle "primitive", normally a sphere
 */
#define NUMPRIMITIVES 5
int particleGeoLists;
int curPrimitiveIdx = 0;
/**
 * OpenGL display list ID for the complete collection of particles currently in the view
 * the particles are rendered into a display list whenever the particles change, which greatly improves
 * rendering performance because the geometry has to be run down the GL pipeline only once
 */
int particlesTotalList;

/**
 * this flag is polled by the rendering thread to determine if a redraw should be issued
 */
bool redrawRequested = false;
/**
 * this flag is polled by the main thred to determine if the display lists should be updated/recompiled
 */
bool recompileRequested = false;
/// the current iteration being displayed
int currentIteration = 0;
/// the camera position in 3d space
static double camPosition[3] = {10, 10, 10};
/// the rotation around the three primary axes in degrees
static double camRotation[3] = {0,0,0};

bool renderHalo = false;
bool renderMembrane = true;
bool renderingPaused = false;
bool renderCells = false;
bool renderFilledCells = false;
int cellCount[3];
double cellSizes[3];


pthread_t RenderOutputWriter::renderingThread;
bool RenderOutputWriter::threadIsSpawned = false;
log4cxx::LoggerPtr RenderOutputWriter::logger = log4cxx::Logger::getLogger("RenderOutputWriter");

/**
 * onResize glut callback
 */
static void
resize(int width, int height)
{
	const float ar = (float) width / (float) height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}

static void drawBox(double *c1, double *c2) {
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(c2[0], c1[1], c1[2]);
	glVertex3d(c1[0], c1[1], c1[2]);


	glVertex3d(c2[0], c1[1], c2[2]);
	glVertex3d(c1[0], c1[1], c2[2]);

	glVertex3d(c2[0], c2[1], c1[2]);
	glVertex3d(c1[0], c2[1], c1[2]);

	glVertex3d(c2[0], c2[1], c2[2]);
	glVertex3d(c1[0], c2[1], c2[2]);

	//x[1] direction
	glColor3d(0, 1.0, 0);

	glVertex3d(c2[0], c2[1], c2[2]);
	glVertex3d(c2[0], c1[1], c2[2]);

	glVertex3d(c1[0], c2[1], c2[2]);
	glVertex3d(c1[0], c1[1], c2[2]);

	glVertex3d(c2[0], c2[1], c1[2]);
	glVertex3d(c2[0], c1[1], c1[2]);

	glVertex3d(c1[0], c2[1], c1[2]);
	glVertex3d(c1[0], c1[1], c1[2]);

	//x[2] direction
	glColor3d(0, 0, 1.0);

	glVertex3d(c2[0], c2[1], c2[2]);
	glVertex3d(c2[0], c2[1], c1[2]);

	glVertex3d(c2[0], c1[1], c2[2]);
	glVertex3d(c2[0], c1[1], c1[2]);

	glVertex3d(c1[0], c2[1], c2[2]);
	glVertex3d(c1[0], c2[1], c1[2]);

	glVertex3d(c1[0], c1[1], c2[2]);
	glVertex3d(c1[0], c1[1], c1[2]);
}

static void drawBoxAndCells() {
		//draw the simulation domain box
	glLineWidth(2.0);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

	const auto d = Settings::domainSize;
	if(renderCells) {
		int h = renderHalo?1:0;
		//x[0] direction
		glColor3d(1.0, 0.0, 0.0);
		for(int x1=-h; x1 <= cellCount[1]-4+h; x1++)
			for(int x2=-h; x2 <= cellCount[2]-4+h; x2++){
				glVertex3d(-cellSizes[0]*h, x1*cellSizes[1], x2*cellSizes[2]);
				glVertex3d(d[0]+cellSizes[0]*h, x1*cellSizes[1], x2*cellSizes[2]);
		}

		//x[1] direction
		glColor3d(0.0, 1.0, 0.0);
		for(int x0=-h; x0 <= cellCount[0]-4+h; x0++)
			for(int x2=-h; x2 <= cellCount[2]-4+h; x2++){
				glVertex3d(x0*cellSizes[0], -cellSizes[1]*h, x2*cellSizes[2]);
				glVertex3d(x0*cellSizes[0], d[1]+cellSizes[1]*h, x2*cellSizes[2]);
		}

		//x[2] direction
		glColor3d(0.0, 0.0, 1.0);
		for(int x0=-h; x0 <= cellCount[0]-4+h; x0++)
			for(int x1=-h; x1 <= cellCount[1]-4+h; x1++){
				glVertex3d(x0*cellSizes[0], x1*cellSizes[1], -cellSizes[2]*h);
				glVertex3d(x0*cellSizes[0], x1*cellSizes[1], d[2]+cellSizes[2]*h);
		}

	}
	else {
		double c1[] = {0, 0, 0};
		double c2[] = {Settings::domainSize[0], Settings::domainSize[1], Settings::domainSize[2]};

		drawBox(c1, c2);

	}
	glEnd();

	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
}

/*!
    Does printf()-like work using freeglut/OpenGLUT
    glutBitmapString().  Uses a fixed font.  Prints
    at the indicated row/column position.

    Limitation: Cannot address pixels.
    Limitation: Renders in screen coords, not model coords.

    Taken from freeglut shapes example
*/
#ifndef NOFREEGLUT
static void shapesPrintf (int row, int col, const char *fmt, ...)
{
    static char buf[256];
    int viewport[4];
    void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
    (void) _vsnprintf (buf, sizeof(buf), fmt, args);
#else
    (void) vsnprintf (buf, sizeof(buf), fmt, args);
#endif
    va_end(args);

    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

        glOrtho(0,viewport[2],0,viewport[3],-1,1);

        glRasterPos2i
        (
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * (row+2) + viewport[3]
        );
        glutBitmapString (font, (unsigned char*)buf);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
#endif

/**
 * colors to use for different particle types
 */
const double typeColors[][3] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{0, 0, 0},
		{1, 1, 0},
		{1, 0, 1},
		{0, 1, 1},
		{1, 1, 1}
};
const int typeColorCount = sizeof(typeColors)/sizeof(typeColors[0]);

/**
 * the glut rendering function callback
 * drawing of the particles happens here
 */
void RenderOutputWriter::display(void)
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*90.0;
	//ColorCoder *palette = new Grayscale();
	//palette->setMin(0);
	//palette->setMax(100);

	glPushMatrix();

	//set the current viewing position
	glTranslated(-camPosition[0], -camPosition[1], -camPosition[2]);

	const auto d = Settings::domainSize;


	//move to domain center
	glTranslated(d[0]/2, d[1]/2, d[2]/2);
	//apply rotation around the center of the domain
	glRotated(camRotation[0], 1, 0, 0);
	glRotated(camRotation[1], 0, 1, 0);
	glRotated(camRotation[2], 0, 0, 1);
	//translate back
	glTranslated(-d[0]/2, -d[1]/2, -d[2]/2);

	//clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	drawBoxAndCells();

	double color[3];

	//draw the particles
	if(recompileRequested) {
		LOG4CXX_TRACE(RenderOutputWriter::logger, "Redrawing display lists...");

		glNewList(particlesTotalList, GL_COMPILE_AND_EXECUTE);
		glEnable(GL_LIGHTING);
		int s = render3dParticles.size();
		for(int i=0; i < s; i++) {
			glPushMatrix();
			glColor3dv(typeColors[render3dParticles[i].type % typeColorCount]);
			//palette->getColor(render3dParticles[i].x.L2Norm(), color);
			//glColor3dv(color);
			glTranslated(render3dParticles[i].x[0], render3dParticles[i].x[1], render3dParticles[i].x[2]);
			//glutSolidSphere(0.5, 16, 16);
			glScaled(Settings::particleTypes[render3dParticles[i].type].sigma,
					Settings::particleTypes[render3dParticles[i].type].sigma,
					Settings::particleTypes[render3dParticles[i].type].sigma);
			glCallList(particleGeoLists + curPrimitiveIdx);
			glPopMatrix();
		}


		if(renderFilledCells) {
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			bool maskHalo = !renderHalo;

			CellListContainer *cc = theContainer;

			int nX0 = cc->nX0, nX1 = cc->nX1, nX2 = cc->nX2;

			for(int x0=1+maskHalo; x0 < nX0-1-maskHalo; x0++)
				for(int x1=1+maskHalo; x1 < nX1-1-maskHalo; x1++)
					for(int x2=1+maskHalo; x2 < nX2-1-maskHalo; x2++) {
						int s = cc->cells[x2 + x1*nX2 + x0*nX2*nX1].getSize();
						if(s) {
							double c1[] = {(x0-2)*cellSizes[0], (x1-2)*cellSizes[1], (x2-2)*cellSizes[2]};
							double c2[] = {(x0-1)*cellSizes[0], (x1-1)*cellSizes[1], (x2-1)*cellSizes[2]};
							drawBox(c1, c2);
						}
					}

			glEnd();
			glEnable(GL_LIGHTING);
		}

		//draw springs
		if(Settings::scenarioType == ScenarioType::Membrane && renderMembrane) {
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
			for(int i=0; i < render3dParticles.size(); i++) {
				//glColor3dv(typeColors[render3dParticles[i].type % typeColorCount]);
				glColor3d(0.7, 0.7, 0.7);
				for(int j = i + 1; j < s; j++) {
					if(	Settings::particleTypes[render3dParticles[i].type].isMolecule &&
						render3dParticles[i].type == render3dParticles[j].type && (
							render3dParticles[i].isNeighbour(render3dParticles[j])
							|| render3dParticles[i].isFaceDiagonal(render3dParticles[j])
							|| render3dParticles[i].isSpaceDiagonal(render3dParticles[j])
						)) {

						if((render3dParticles[i].x - render3dParticles[j].x).LengthOptimizedR3Squared() <= Settings::rCutoff*Settings::rCutoff) {

							glVertex3dv(&render3dParticles[i].x[0]);
							glVertex3dv(&render3dParticles[j].x[0]);
						}
						//else
						//	glColor3d(0.6, 0.6, 0.6);

					}
				}
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}
		glEndList();
		//reset the flag
		recompileRequested = false;
	}
	else {
		glCallList(particlesTotalList);
	}



	glColor3d(0.1,0.1,0.4);
#ifndef NOFREEGLUT
	shapesPrintf(1, 3, "%i particles", render3dParticles.size());
	shapesPrintf(2, 3, "Iteration %i", currentIteration);
	shapesPrintf(3, 3, "Time %f", currentIteration*Settings::deltaT);
#endif
	glutSwapBuffers();

	glPopMatrix();

	//delete palette;
}

/**
 * glut keydown handler - used for movement of the camera position
 */
static void
key(unsigned char key, int x, int y)
{

	switch (key)
	{
#ifndef NOFREEGLUT
	case 27 : glutLeaveMainLoop () ;      break;
#endif
	case 'a': camPosition[0] -= 1.0; break;
	case 'd': camPosition[0] += 1.0; break;
	case 's': camPosition[1] -= 1.0; break;
	case 'w': camPosition[1] += 1.0; break;
	case 'q': camPosition[2] -= 1.0; break;
	case 'e': camPosition[2] += 1.0; break;

	default:
		break;
	}

	glutPostRedisplay();
}

static void
keyup(unsigned char key, int x, int y)
{

	switch (key)
	{
#ifndef NOFREEGLUT
	case 27 : glutLeaveMainLoop () ;      break;
#endif
	case 'h': renderHalo = !renderHalo; break;
	case 'm': renderMembrane = !renderMembrane; break;
	case 'p': renderingPaused = !renderingPaused; break;
	case 'c' : renderCells = !renderCells; break;
	case 'f': renderFilledCells = !renderFilledCells; break;
	case '.': curPrimitiveIdx = (curPrimitiveIdx+1)%NUMPRIMITIVES; break;

	default:
		break;
	}

	glutPostRedisplay();
}

/**
 * glut special keys handler - used for camera rotation via the arrow keys
 */
static void special (int key, int x, int y)
{
	switch (key)
	{

	case GLUT_KEY_UP:         camRotation[0] -= 10.0;        break;
	case GLUT_KEY_DOWN:       camRotation[0] += 10.0;        break;

	case GLUT_KEY_RIGHT:      camRotation[1] += 10.0;        break;
	case GLUT_KEY_LEFT:       camRotation[1] -= 10.0;        break;

	default:
		break;
	}

	glutPostRedisplay();
}

/**
 * available mouse action types
 */
enum MouseAction {
	 NONE, ROTATE, MOVE, ZOOM
};
/**
 * the current mouse state (determined by the mouse button handler)
 */
MouseAction currentMouseState = NONE;
/// last registered mouse location x
int lastMouseX = 0;
/// last registered mouse location y
int lastMouseY = 0;

/**
 * mouse button handler - called when mouse button is pressed or released
 * saves the last button press to specify the action to be performed if the
 * mouse is moved
 */
static void mouse(int button, int state, int x, int y) {
	if(state == GLUT_UP) {
		currentMouseState = NONE;
	}
	else {
		lastMouseX = x;
		lastMouseY = y;
		switch (button) {
		case GLUT_LEFT_BUTTON:
			currentMouseState = ROTATE;
			break;
		case GLUT_RIGHT_BUTTON:
			currentMouseState = MOVE;
			break;
		case GLUT_MIDDLE_BUTTON:
			currentMouseState = ZOOM;
			break;
		default:
			currentMouseState = NONE;
			break;
		}
	}
}

#define ROTATION_SPEED 1
#define MOVE_SPEED -0.05
#define ZOOM_SPEED 0.05

/**
 * glut mouse handler - called when mouse is moved while a buttons pressed
 * handles movement (right mouse), rotation (left mouse) and zoom (middle mouse)
 */
static void mouseactivemove(int x, int y) {
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;
	switch (currentMouseState) {
	case NONE: break;
	case ROTATE:
		camRotation[0] += dy * ROTATION_SPEED;
		camRotation[1] += dx * ROTATION_SPEED;
		glutPostRedisplay();
		break;
	case MOVE:
		camPosition[0] += dx * MOVE_SPEED;
		camPosition[1] -= dy * MOVE_SPEED;
		glutPostRedisplay();
		break;
	case ZOOM:
		camPosition[2] += dy * ZOOM_SPEED;
		glutPostRedisplay();
		break;
	}
	lastMouseX = x;
	lastMouseY = y;
}

#define WHEEL_ZOOM_SPEED (-0.3)
/**
 * freeglut mouse wheel handler function - handles zooming
 */
void mousewheel(int wheelno, int direction, int x, int y) {
	camPosition[2] += direction * 0.5;
	glutPostRedisplay();
}

/**
 * glut idle function callback
 *
 * checks if a redraw has been requested by another thread - that is, when new particles have been
 * drawn etc and then redraws the frame
 */
static void idle() {
	if(redrawRequested) {
		glutPostRedisplay();
		redrawRequested = false;
	}
}

/**
 * rendering thread main routine
 * handles setup of freeglut framework and enters the glutMainLoop()
 */
void * renderFunction(void* arg) {
	int argc = 1;
	char* argv[] = {"MolSim"};

	//intialize camera position to match current simulation domain
	camPosition[0] = Settings::domainSize[0]/2.0;
	camPosition[1] = Settings::domainSize[1]/2.0;
	//most times this works to display the whole region at an adequate zoom level
	camPosition[2] = Settings::domainSize[2] + (Settings::domainSize[0] + Settings::domainSize[1]) / 2;

	glutInitWindowSize(640,480);
	glutInitWindowPosition(40,40);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow("Molecular Dynamics Rendering");

	glutReshapeFunc(resize);
	glutDisplayFunc(RenderOutputWriter::display);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
#ifndef NOFREEGLUT
	glutMouseWheelFunc(mousewheel);
	glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;
#endif
	glutMotionFunc(mouseactivemove);


	glClearColor(1,1,1,1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	particleGeoLists = glGenLists(NUMPRIMITIVES+1);
	particlesTotalList = particleGeoLists + NUMPRIMITIVES;


	glNewList(particleGeoLists, GL_COMPILE);
		glutSolidSphere(0.5, 8, 8);
	glEndList();

	glNewList(particleGeoLists+1, GL_COMPILE);
		glutWireSphere(0.5, 8, 8);
	glEndList();

	glNewList(particleGeoLists+2, GL_COMPILE);
		glutSolidSphere(0.5, 8, 2);
	glEndList();

	glNewList(particleGeoLists+3, GL_COMPILE);
		glutSolidCube(1);
	glEndList();

	glPointSize(5.0);
	glNewList(particleGeoLists+4, GL_COMPILE);
			glBegin(GL_POINTS);
			glVertex3d(0,0,0);
			glEnd();
	glEndList();






	LOG4CXX_DEBUG(RenderOutputWriter::logger, "Entering rendering loop...");
	glutMainLoop();
	LOG4CXX_DEBUG(RenderOutputWriter::logger, "Rendering finished!");

	return NULL;
}

RenderOutputWriter::RenderOutputWriter() {
	//check if the rendering thread is active
	//kill with a signal of 0 does not actually kill the thread, but returns an error
	//if the thread does not exist
	//int status = pthread_kill(renderingThread, 0);
	//if an error occurred, the thread did not exists, so we create a new rendering thread
	if(!threadIsSpawned) {
		pthread_create(&renderingThread, NULL, renderFunction, NULL);
		threadIsSpawned = true;
	}

}

void RenderOutputWriter::plotParticles(ParticleContainer & container, const std::string& filename, int iteration) {
	currentIteration = iteration;

	//copy particle data over
/*	container.each([&] (Particle &p) {
#ifdef _OPENMP
#pragma omp critical
#endif
		render3dParticles[i++] = p;
	});*/

	if(renderingPaused) return;

	CellListContainer *cc = (CellListContainer*)&container;

	cellCount[0] = cc->nX0;
	cellCount[1] = cc->nX1;
	cellCount[2] = cc->nX2;
	cellSizes[0] = cc->edgeLength[0];
	cellSizes[1] = cc->edgeLength[1];
	cellSizes[2] = cc->edgeLength[2];

	bool maskHalo = !renderHalo;

	theContainer = cc;

	render3dParticles.resize(cc->getSize(!maskHalo));
	int i = 0;

	int nX0 = cc->nX0, nX1 = cc->nX1, nX2 = cc->nX2;

	for(int x0=1+maskHalo; x0 < nX0-1-maskHalo; x0++)
		for(int x1=1+maskHalo; x1 < nX1-1-maskHalo; x1++)
			for(int x2=1+maskHalo; x2 < nX2-1-maskHalo; x2++) {
				ParticleContainer &plist = cc->cells[x2 + x1*nX2 + x0*nX2*nX1];
				int s = plist.getSize();
				plist.each([&] (Particle &p) {
					render3dParticles[i++] = p;
				});
			}

	//signal the rendering thread to recompile display list and redraw buffer
	redrawRequested = true;
	recompileRequested = true;

}

} //namespace outputWriter

#endif //#ifndef NOGLVISUALIZER
