#ifndef OPENGLVERTEXARRAYOBJECT_H
#define OPENGLVERTEXARRAYOBJECT_H

#include <QOpenGLVertexArrayObject>
#include "openglcommon.h"

// Register to check OpenGLBuffer
class OpenGLVertexArrayObjectChecked : public QOpenGLVertexArrayObject
{
public:
  OpenGLVertexArrayObjectChecked(QObject *parent = 0) : QOpenGLVertexArrayObject(parent) {}
  GL_CHECK(QOpenGLVertexArrayObject,create);
};

GL_QFINALIZE(OpenGLVertexArrayObject,Checked);

#endif // OPENGLVERTEXARRAYOBJECT_H
