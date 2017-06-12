#ifndef OPENGL_H_
#define OPENGL_H_

#define GL_GLEXT_PROTOTYPES

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#endif	/* OPENGL_H_ */
