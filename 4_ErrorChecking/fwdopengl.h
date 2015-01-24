#ifndef FWDOPENGL_H
#define FWDOPENGL_H

#include "openglerror.h"
#include "macros.h"

#endif // FWDOPENGL_H

/*******************************************************************************
 * Defines for managing overriden OpenGL classes
 ******************************************************************************/

// Reference parent class
#define PARENT APPEND(Q,GL_CLASS)

// Define both function declaration and definition
#define PROC_DECL(proc)     inline bool proc()
#define FUNC_DECL(func,...) inline bool func(PDECL(__VA_ARGS__))
#define PROC_CALL(proc)     PARENT::proc()
#define FUNC_CALL(func,...) PARENT::func(PCALL(__VA_ARGS__))
#define GL_REPORT_ERROR(name) { OpenGLError ev(STR(GL_CLASS), STR(name), OpenGLError::ErrorOn_##name); return OpenGLError::sendEvent(&ev); }

// Only handle Debug information if GL_DEBUG is set
#ifdef    GL_DEBUG
# define GL_METHOD(func,...) FUNC_DECL(func,__VA_ARGS__) { if (!FUNC_CALL(func,__VA_ARGS__)) { GL_REPORT_ERROR(func); return false; } return true; }
# define GL_PROCEDURE(proc)  PROC_DECL(proc) { if (!PROC_CALL(proc)) { GL_REPORT_ERROR(proc); return false; } return true; }
#else
# define GL_METHOD(func,...)
# define GL_PROCEDURE(proc)
#endif // GL_DEBUG

// define beginning and end of class
#define GL_BEGIN class GL_CLASS : public PARENT { public:
#define GL_END };

// Define constructors
#define GL_QOBJECT() GL_CLASS(QObject *parent = 0) : PARENT(parent) {}

/*******************************************************************************
 * OpenGL class overrides
 ******************************************************************************/

//#include <QOpenGLShaderProgram>
class OpenGLShaderProgram;
#if defined(QOPENGLSHADERPROGRAM_H) && !defined(OPENGLSHADERPROGRAM_H)
#define OPENGLSHADERPROGRAM_H
# define GL_CLASS OpenGLShaderProgram
GL_BEGIN
  GL_QOBJECT();
  GL_PROCEDURE(link);
  GL_PROCEDURE(bind);
  GL_PROCEDURE(create);
  GL_METHOD(addShader,QOpenGLShader*);
  GL_METHOD(addShaderFromSourceCode,QOpenGLShader::ShaderType,const char*);
  GL_METHOD(addShaderFromSourceCode,QOpenGLShader::ShaderType,const QByteArray&);
  GL_METHOD(addShaderFromSourceCode,QOpenGLShader::ShaderType,const QString&);
  GL_METHOD(addShaderFromSourceFile,QOpenGLShader::ShaderType,const QString&);
GL_END
# undef GL_CLASS
#endif // QOPENGLSHADERPROGRAM_H

//#include <QOpenGLBuffer>
class OpenGLBuffer;
#if defined(QOPENGLBUFFER_H) && !defined(OPENGLBUFFER_H)
#define OPENGLBUFFER_H
# define GL_CLASS OpenGLBuffer
GL_BEGIN
  GL_PROCEDURE(bind);
  GL_PROCEDURE(create);
  GL_PROCEDURE(unmap);
GL_END
# undef GL_CLASS
#endif // QOPENGLBUFFER_H

//#include <QOpenGLVertexArrayObject>
class OpenGLVertexArrayObject;
#if defined(QOPENGLVERTEXARRAYOBJECT_H) && !defined(OPENGLVERTEXARRAYOBJECT_H)
#define OPENGLVERTEXARRAYOBJECT_H
# define GL_CLASS OpenGLVertexArrayObject
GL_BEGIN
  GL_QOBJECT();
  GL_PROCEDURE(create);
GL_END
# undef GL_CLASS
#endif // QOPENGLBUFFER_H


/*******************************************************************************
 * Undefine everything
 ******************************************************************************/

#undef GL_QOBJECT
#undef GL_END
#undef GL_BEGIN
#undef GL_PROCEDURE
#undef GL_METHOD
#undef GL_REPORT_ERROR
#undef FUNC_CALL
#undef PROC_CALL
#undef FUNC_DECL
#undef PROC_DECL
#undef PARENT
