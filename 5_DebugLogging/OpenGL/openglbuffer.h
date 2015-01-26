#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H

#include <QOpenGLBuffer>
#include "openglcommon.h"

// Register to check OpenGLBuffer
class OpenGLBufferChecked : public QOpenGLBuffer
{
public:
  GL_CHECK(QOpenGLBuffer,bind);
  GL_CHECK(QOpenGLBuffer,create);
  GL_CHECK(QOpenGLBuffer,unmap);
  GL_CHECK(QOpenGLBuffer,read,int,void*,int);
};

GL_FINALIZE(OpenGLBuffer,Checked);

#endif // OPENGLBUFFER_H
