#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H

#include <OpenGLCommon>
#include <QOpenGLBuffer>

// Register to check OpenGLBuffer
class OpenGLBufferChecked : public QOpenGLBuffer
{
public:
  GL_CHECK(QOpenGLBuffer,bind);
  GL_CHECK(QOpenGLBuffer,create);
  GL_CHECK(QOpenGLBuffer,unmap);
  GL_CHECK(QOpenGLBuffer,read,int,void*,int);
};

// Final class
class OpenGLBuffer : public OpenGLBufferChecked
{
  // Intentionally Empty
};

#endif // OPENGLBUFFER_H
