#ifndef OPENGLVERTEXARRAYOBJECT_H
#define OPENGLVERTEXARRAYOBJECT_H

#include <QOpenGLVertexArrayObject>
#include "openglcommon.h"

// Register to check OpenGLBuffer
class OpenGLVertexArrayObjectChecked : public QOpenGLVertexArrayObject
{
public:
  GL_CHECK(QOpenGLVertexArrayObject,create);
};

GL_FINALIZE(OpenGLVertexArrayObject,Checked);

#endif // OPENGLVERTEXARRAYOBJECT_H
