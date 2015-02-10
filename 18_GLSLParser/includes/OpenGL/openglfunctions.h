#ifndef OPENGLFUNCTIONS_H
#define OPENGLFUNCTIONS_H

#include <OpenGLCommon>
#include <QtOpenGL/QGL>

// Depending on what is available -
// Select OpenGL ES 3.0 or OpenGL 3.3 Core.
#if !defined(QT_NO_OPENGL) && defined(QT_OPENGL_ES_3)

#include <OpenGLFunctions_ES3_0>

class OpenGLFunctions : public OpenGLFunctions_ES3_0
{
public:
  void glClearDepth(GLclampf depth);
};

inline void OpenGLFunctions::glClearDepth(GLclampf depth)
{
  glClearDepthf(depth);
}

#elif !defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)

#include <OpenGLFunctions_3_3_Core>

class OpenGLFunctions : public OpenGLFunctions_3_3_Core
{
public:
  void glClearDepthf(GLdouble depth);
};

inline void OpenGLFunctions::glClearDepthf(GLdouble depth)
{
  glClearDepth(depth);
}

#else
//#error Expected OpenGL to be available!
#endif

#endif // OPENGLFUNCTIONS_H

