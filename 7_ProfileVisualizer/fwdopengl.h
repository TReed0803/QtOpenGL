#ifndef FWDOPENGL_H
#define FWDOPENGL_H

#include "openglerror.h"
#include "macros.h"
#include "profiler.h"
#include <type_traits>

#endif // FWDOPENGL_H

/*******************************************************************************
 * Defines for managing overriden OpenGL classes
 ******************************************************************************/

// Reference parent class
#define PARENT APPEND(Q,GL_CLASS)

// Define both function declaration and definition
#define PROC_RETV(proc)     decltype(((PARENT*)0)->PARENT::proc())
#define FUNC_RETV(func,...) decltype(((PARENT*)0)->PARENT::func(PCAST(__VA_ARGS__)))
#define PROC_DECL(proc)     inline PROC_RETV(proc) proc()
#define FUNC_DECL(func,...) inline FUNC_RETV(func,__VA_ARGS__) func(PDECL(__VA_ARGS__))
#define PROC_CALL(proc)     PARENT::proc()
#define FUNC_CALL(func,...) PARENT::func(PCALL(__VA_ARGS__))
#define GL_REPORT_ERROR(name) { OpenGLError ev(STR(GL_CLASS), STR(name), OpenGLError::ErrorOn_##name); OpenGLError::sendEvent(&ev); }

// Branch into bool, void, and otherwise
#define GL_FUNC_IMPL_BOOL(func,...) PROFILER_PUSH_GPU_MARKER(STR(func)); bool res = FUNC_CALL(func,__VA_ARGS__); PROFILER_POP_GPU_MARKER(); if (!res) GL_REPORT_ERROR(func); return res;
#define GL_FUNC_IMPL_VOID(func,...) PROFILER_PUSH_GPU_MARKER(STR(func)); FUNC_CALL(func,__VA_ARGS__); PROFILER_POP_GPU_MARKER();
#define GL_FUNC_IMPL_RETV(func,...) PROFILER_PUSH_GPU_MARKER(STR(func)); FUNC_RETV(func,__VA_ARGS__) ret = FUNC_CALL(func,__VA_ARGS__); PROFILER_POP_GPU_MARKER(); return ret;
#define GL_PROC_IMPL_BOOL(func,...) PROFILER_PUSH_GPU_MARKER(STR(func)); bool res = PROC_CALL(func); PROFILER_POP_GPU_MARKER(); if (!res) GL_REPORT_ERROR(func); return res;
#define GL_PROC_IMPL_VOID(func,...) PROFILER_PUSH_GPU_MARKER(STR(func)); PROC_CALL(func); PROFILER_POP_GPU_MARKER();
#define GL_PROC_IMPL_RETV(func,...) PROFILER_PUSH_GPU_MARKER(STR(func)); PROC_RETV(func) ret = PROC_CALL(func); PROFILER_POP_GPU_MARKER(); return ret;

// Only handle Debug information if GL_DEBUG is set
#ifdef    GL_DEBUG
# define GL_METHOD(func,...) \
  template <typename T> typename inline std::enable_if<std::is_same<T, bool>::value,T>::type                                   APPEND(_,func)(PDECL(__VA_ARGS__)) { GL_FUNC_IMPL_BOOL(func,__VA_ARGS__); } \
  template <typename T> typename inline std::enable_if<std::is_same<T, void>::value,T>::type                                   APPEND(_,func)(PDECL(__VA_ARGS__)) { GL_FUNC_IMPL_VOID(func,__VA_ARGS__); } \
  template <typename T> typename inline std::enable_if<!std::is_same<T, bool>::value && !std::is_same<T, void>::value,T>::type APPEND(_,func)(PDECL(__VA_ARGS__)) { GL_FUNC_IMPL_RETV(func,__VA_ARGS__); } \
  inline FUNC_RETV(func,__VA_ARGS__) func(PDECL(__VA_ARGS__)) { return APPEND(_,func)<FUNC_RETV(func,__VA_ARGS__)>(PCALL(__VA_ARGS__)); }
# define GL_PROCEDURE(proc) \
  template <typename T> typename inline std::enable_if<std::is_same<T, bool>::value,T>::type                                   APPEND(_,proc)() { GL_PROC_IMPL_BOOL(proc); } \
  template <typename T> typename inline std::enable_if<std::is_same<T, void>::value,T>::type                                   APPEND(_,proc)() { GL_PROC_IMPL_VOID(proc); } \
  template <typename T> typename inline std::enable_if<!std::is_same<T, bool>::value && !std::is_same<T, void>::value,T>::type APPEND(_,proc)() { GL_PROC_IMPL_RETV(proc); } \
  inline PROC_RETV(proc) proc() { return APPEND(_,proc)<PROC_RETV(proc)>(); }
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
  GL_PROCEDURE(release);
  GL_METHOD(addShader,QOpenGLShader*);
  GL_METHOD(addShaderFromSourceCode,QOpenGLShader::ShaderType,const char*);
  GL_METHOD(addShaderFromSourceCode,QOpenGLShader::ShaderType,const QByteArray&);
  GL_METHOD(addShaderFromSourceCode,QOpenGLShader::ShaderType,const QString&);
  GL_METHOD(addShaderFromSourceFile,QOpenGLShader::ShaderType,const QString&);
  GL_METHOD(setUniformValue,int,const QMatrix4x4&);
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
  GL_PROCEDURE(release);
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
  GL_PROCEDURE(bind);
  GL_PROCEDURE(release);
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
