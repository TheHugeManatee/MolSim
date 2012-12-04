/*
 * @file RenderOutputWriter.h
 *
 * @date 02.12.2012
 * @author Jakob Weiss
 */

#ifndef RENDEROUTPUTWRITER_H_
#define RENDEROUTPUTWRITER_H_

#include "Particle.h"
#include "ParticleContainer.h"

#include <log4cxx/logger.h>

#include <pthread.h>

namespace outputWriter {

/**
 * @class RenderOutputWriter
 *
 * this output writer is used to display the current frame as a OpenGL rendering in a window.
 * it creates a separate thread that will handle all the display functionality, which uses FreeGLUT as the framework
 * to simplify displaying tasks and platform specific issues.
 *
 * Since glut has no OO design, the implementation found in RenderOutputWriter.cpp is not very beautiful. The RenderOutputWriter class
 * is mainly just a mock class which will initiate the (global) glut thread if not present, and then update the (global) particle list
 * that will be rendered within the window. The design could be improved using a singleton for the glut renderer, but for now it
 * will have to suffice.
 *
 * \subsection rUsage Usage
 *  - Use the arrow keys or left mouse button to rotate the view
 *  - use wasd or right mouse button to move the domain around
 *  - use q and e or the mouse wheel to zoom in/out
 *
 * @warning Since GLUT and FreeGLUT are not designed for multithreading, they are somewhat tricky to use
 * 	in our scenario. It seems the glut functions have to be called by the same thread that did the initialization,
 * 	which is why for example the idle callback polls the redrawRequested boolean to issue a call to glutPostRedisplay()
 * 	from within the the glut thread. No thread synchronization is used so there may be concurrency issues, but it seems
 * 	that most things are not critical with respect to timing, so this should be ok..
 *
 */
class RenderOutputWriter {
private:
	/**
	 * static thread specifier of the rendering thread that decouples glut rendering from the simulator
	 */
	static pthread_t renderingThread;

public:
	RenderOutputWriter();

	static log4cxx::LoggerPtr logger;

	/**
	 * updates the global particle list to be rendered in the glut 3D view
	 */
	void plotParticles(ParticleContainer &container, const std::string& filename, int iteration);
};

}
#endif /* RENDEROUTPUTWRITER_H_ */
