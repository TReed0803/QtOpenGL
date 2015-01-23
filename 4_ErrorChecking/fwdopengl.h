#ifndef OPENGLERRORWRAPPER_H
#define OPENGLERRORWRAPPER_H

#include "openglerrorevent.h"
#include "macros.h"

// Defining Declaration and Definition of parameters
#define DECL_0()
#define DECL_1(t1) t1 a
#define DECL_2(t1,t2) t1 a, t2 b
#define DECL_3(t1,t2,t3) t1 a, t2 b, t3 c
#define DECL_4(t1,t2,t3) t1 a, t2 b, t3 c, t4 d
#define DECL_5(t1,t2,t3) t1 a, t2 b, t3 c, t4 d, t5 e
#define DEFN_0()
#define DEFN_1() a
#define DEFN_2() a, b
#define DEFN_3() a, b, c
#define DEFN_4() a, b, c, d
#define DEFN_5() a, b, c, d, e

// Define both parameter declaration and definition
#define PARAMS_DECL(n,...) DELAY(DECL_)##DELAY(n)(__VA_ARGS__)
#define PARAMS_DEFN(n,...) DELAY(DEFN_)##DELAY(n)()

// Define both function declaration and definition
#define FUNC_DECL_N(func,n,...) inline bool func(PARAMS_DECL(n,__VA_ARGS__))
#define FUNC_DECL(func,...) FUNC_DECL_N(func,NARGS(__VA_ARGS__),__VA_ARGS__)
#define FUNC_DEFN_N(func,n,...) this->APPEND(Q,GL_CLASS)::func(PARAMS_DEFN(n,__VA_ARGS__))
#define FUNC_DEFN(func,...) FUNC_DEFN_N(func,NARGS(__VA_ARGS__),__VA_ARGS__)

#ifdef GL_WRAP_ERRORS
// Define both method and procedure definitions.
// Note: This is a custom naming convention. Because of how the preprocessor
//       expands, we cannot detect when there are 0 variables passed into NARGS(...).
//       Because of this, we say that GL_PROCEDURE is a sepecial case of GL_METHOD where NARGS = 0.
#define GL_METHOD_N(func,n,...) FUNC_DECL_N(func,n,__VA_ARGS__) { if (!FUNC_DEFN_N(func,n,__VA_ARGS__)) { OpenGLErrorEvent event(#func); return QGuiApplication::sendEvent(QGuiApplication::focusWindow(),&event); } return true; }
#define GL_METHOD(func,...) GL_METHOD_N(func,NARGS(__VA_ARGS__),__VA_ARGS__)
#define GL_PROCEDURE(func)  GL_METHOD_N(func,0)
#else
#define GL_METHOD_N(func,n,...)
#define GL_METHOD(func,...)
#define GL_PROCEDURE(func)
#endif // GL_WRAP_ERRORS

// define beginning and end of class
#define GL_BEGIN class GL_CLASS : public APPEND(Q,GL_CLASS) { public:
#define GL_END };

// Define constructors
#define GL_QOBJECT() GL_CLASS(QObject *parent = 0) : APPEND(Q,GL_CLASS)(parent) {}

#endif // OPENGLERRORWRAPPER_H


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
