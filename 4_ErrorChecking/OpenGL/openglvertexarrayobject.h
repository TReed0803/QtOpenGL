#ifndef OPENGLVERTEXARRAYOBJECT_H
#define OPENGLVERTEXARRAYOBJECT_H

#include <QOpenGLVertexArrayObject>
#include "openglcommon.h"

// Register to check OpenGLBuffer
class OpenGLVertexArrayObjectChecked : public QOpenGLVertexArrayObject
{
public:
  explicit OpenGLVertexArrayObjectChecked(QObject *parent = 0) : QOpenGLVertexArrayObject(parent) {}
  GL_CHECK(QOpenGLVertexArrayObject,create);
};

// Final class
class OpenGLVertexArrayObject : public OpenGLVertexArrayObjectChecked
{
public:
  explicit OpenGLVertexArrayObject(QObject *parent = 0) : OpenGLVertexArrayObjectChecked(parent) {}
};

#endif // OPENGLVERTEXARRAYOBJECT_H
