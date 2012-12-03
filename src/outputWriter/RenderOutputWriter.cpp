/*
 * @file RenderOutputWriter.cpp
 *
 * @date 02.12.2012
 * @author Jakob Weiss
 */

#include "RenderOutputWriter.h"

#include "utils/Settings.h"



#include <iostream>
#include <string>
#include <sstream>
#include <GL/freeglut.h>
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
const GLfloat high_shininess[] = { 100.0f };

std::vector<Particle> render3dParticles;

int particleGeoList;
int particlesTotalList;

bool redrawRequested = false;
bool recompileRequested = false;
int currentIteration = 0;
static double camPosition[3] = {10, 10, 10};
static double camRotation[3] = {0,0,0};


pthread_t RenderOutputWriter::renderingThread;
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

/*!
    Does printf()-like work using freeglut/OpenGLUT
    glutBitmapString().  Uses a fixed font.  Prints
    at the indicated row/column position.

    Limitation: Cannot address pixels.
    Limitation: Renders in screen coords, not model coords.

    Taken from freeglut shapes example
*/
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
static void display(void)
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*90.0;

	glPushMatrix();

	glTranslated(0, 0, -camPosition[2]);
	glRotated(camRotation[0], 1, 0, 0);
	glRotated(camRotation[1], 0, 1, 0);
	glRotated(camRotation[2], 0, 0, 1);
	glTranslated(-camPosition[0], -camPosition[1], 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto d = Settings::domainSize;

	//draw the simulation domain box
	glLineWidth(2.0);
	//x[0] direction
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(d[0], 0.0, 0.0);

	glVertex3d(0.0, d[1], 0.0);
	glVertex3d(d[0], d[1], 0.0);

	glVertex3d(0.0, 0.0, d[2]);
	glVertex3d(d[0], 0.0, d[2]);

	glVertex3d(0.0, d[1], d[2]);
	glVertex3d(d[0], d[1], d[2]);
	glEnd();
	//x[1] direction
	glColor3d(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, d[1], 0.0);

	glVertex3d(d[0], 0.0, 0.0);
	glVertex3d(d[0], d[1], 0.0);

	glVertex3d(0.0, 0.0, d[2]);
	glVertex3d(0.0, d[1], d[2]);

	glVertex3d(d[0], 0.0, d[2]);
	glVertex3d(d[0], d[1], d[2]);
	glEnd();
	//x[2] direction
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, d[2]);

	glVertex3d(0.0, d[1], 0.0);
	glVertex3d(0.0, d[1], d[2]);

	glVertex3d(d[0], 0.0, 0.0);
	glVertex3d(d[0], 0.0, d[2]);

	glVertex3d(d[0], d[1], 0.0);
	glVertex3d(d[0], d[1], d[2]);
	glEnd();

	glEnable(GL_LIGHTING);
	//draw the particles
	if(recompileRequested) {
		LOG4CXX_TRACE(RenderOutputWriter::logger, "Redrawing display lists...");
		glNewList(particlesTotalList, GL_COMPILE_AND_EXECUTE);
		for(int i=0; i < render3dParticles.size(); i++) {
			glPushMatrix();
			glColor3dv(typeColors[render3dParticles[i].type % typeColorCount]);
			glTranslated(render3dParticles[i].x[0], render3dParticles[i].x[1], render3dParticles[i].x[2]);
			//glutSolidSphere(0.5, 16, 16);
			glCallList(particleGeoList);
			glPopMatrix();
		}

		glEndList();
		recompileRequested = false;
	}
	else {
		glCallList(particlesTotalList);
	}


	glDisable(GL_LIGHTING);
	glColor3d(0.1,0.1,0.4);

	shapesPrintf(1, 3, "%i particles", render3dParticles.size());
	shapesPrintf(2, 3, "Iteration %i", currentIteration);
	shapesPrintf(3, 3, "Time %f", currentIteration*Settings::deltaT);

	glutSwapBuffers();

	glPopMatrix();

}

/**
 * glut keydown handler - used for movement of the camera position
 */
static void
key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27 : glutLeaveMainLoop () ;      break;

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
 * check if a redraw has been requested by another thread - that is, when new particles have been
 * drawn etc.
 */
static void idle() {
	if(redrawRequested) {
		glutPostRedisplay();
		redrawRequested = false;
	}
}

/**
 * thread main routine for initializing glut and entering main loop..
 */
void * renderFunction(void* arg) {
	int argc = 1;
	char* argv[] = {"MolSim"};

	camPosition[0] = Settings::domainSize[0]/2.0;
	camPosition[1] = Settings::domainSize[1]/2.0;
	camPosition[2] = Settings::domainSize[2] + (Settings::domainSize[0] + Settings::domainSize[1]) / 2;

	glutInitWindowSize(640,480);
	glutInitWindowPosition(40,40);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow("Molecular Dynamics Rendering");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;

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
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	particleGeoList = glGenLists(2);
	particlesTotalList = particleGeoList + 1;

	glNewList(particleGeoList, GL_COMPILE);

	glutSolidSphere(0.5, 8, 8);

	glEndList();


	LOG4CXX_DEBUG(RenderOutputWriter::logger, "Entering rendering loop...");
	glutMainLoop();
	LOG4CXX_DEBUG(RenderOutputWriter::logger, "Rendering finished!");

	return NULL;
}

RenderOutputWriter::RenderOutputWriter() {
	//check if the rendering thread is active, otherwise create one
	int status = pthread_kill(renderingThread, 0);

	if(status) {
		pthread_create(&renderingThread, NULL, renderFunction, NULL);
	}

}

void RenderOutputWriter::plotParticles(ParticleContainer & container, const std::string& filename, int iteration) {
	currentIteration = iteration;

	render3dParticles.resize(container.getSize());
	int i = 0;
	container.each([&] (Particle &p) {
		render3dParticles[i++] = p;
	});
	redrawRequested = true;
	recompileRequested = true;
}

} //namespace outputWriter
