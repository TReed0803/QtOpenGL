#ifndef OPENGLFUNCTIONS_H
#define OPENGLFUNCTIONS_H OpenGLFunctions

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

class GL
{
private:
  static OpenGLFunctions *m_functions;
public:
  static OpenGLFunctions *getInstance();
  static void setInstance(OpenGLFunctions *f);

  // 2.0
  /*
  static inline void glActiveTexture (GLenum texture){ GL::getInstance()->glActiveTexture (texture); }
  static inline void glAttachShader (GLuint program, GLuint shader) { GL::getInstance()->glAttachShader (program, shader); }
  static inline void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name) { GL::getInstance()->glBindAttribLocation (program, index, name); }
  static inline void glBindBuffer (GLenum target, GLuint buffer) { GL::getInstance()->glBindBuffer (target, buffer); }
  static inline void glBindFramebuffer (GLenum target, GLuint framebuffer) { GL::getInstance()->glBindFramebuffer (target, framebuffer); }
  static inline void glBindRenderbuffer (GLenum target, GLuint renderbuffer) { GL::getInstance()->glBindRenderbuffer (target, renderbuffer); }
  static inline void glBindTexture (GLenum target, GLuint texture) { GL::getInstance()->glBindTexture (target, texture); }
  static inline void glBlendColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { GL::getInstance()->glBlendColor (red, green, blue, alpha); }
  static inline void glBlendEquation (GLenum mode) { GL::getInstance()->glBlendEquation (mode); }
  static inline void glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha) { GL::getInstance()->glBlendEquationSeparate (modeRGB, modeAlpha); }
  static inline void glBlendFunc (GLenum sfactor, GLenum dfactor) { GL::getInstance()->glBlendFunc (sfactor, dfactor); }
  static inline void glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) { GL::getInstance()->glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha); }
  static inline void glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage) { GL::getInstance()->glBufferData (target, size, data, usage); }
  static inline void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data) { GL::getInstance()->glBufferSubData (target, offset, size, data); }
  static inline GLenum glCheckFramebufferStatus (GLenum target) { GL::getInstance()->glCheckFramebufferStatus (target); }
  static inline void glClear (GLbitfield mask) { GL::getInstance()->glClear (mask); }
  static inline void glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { GL::getInstance()->glClearColor (red, green, blue, alpha); }
  static inline void glClearDepthf (GLfloat d) { GL::getInstance()->glClearDepthf (d); }
  static inline void glClearStencil (GLint s) { GL::getInstance()->glClearStencil (s); }
  static inline void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) { GL::getInstance()->glColorMask (red, green, blue, alpha); }
  static inline void glCompileShader (GLuint shader) { GL::getInstance()->glCompileShader (GLuint shader); }
  static inline void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) { GL::getInstance()->glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data); }
  static inline void glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) { GL::getInstance()->glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data); }
  static inline void glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) { GL::getInstance()->glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border); }
  static inline void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) { GL::getInstance()->glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height); }
  static inline GLuint glCreateProgram (void) { GL::getInstance()->glCreateProgram (void); }
  static inline GLuint glCreateShader (GLenum type) { GL::getInstance()->glCreateShader (GLenum type); }
  static inline void glCullFace (GLenum mode) { GL::getInstance()->glCullFace (GLenum mode); }
  static inline void glDeleteBuffers (GLsizei n, const GLuint *buffers) { GL::getInstance()->glDeleteBuffers (GLsizei n, const GLuint *buffers); }
  static inline void glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers) { GL::getInstance()->glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers); }
  static inline void glDeleteProgram (GLuint program) { GL::getInstance()->glDeleteProgram (GLuint program); }
  static inline void glDeleteRenderbuffers (GLsizei n, const GLuint *renderbuffers) { GL::getInstance()->glDeleteRenderbuffers (GLsizei n, const GLuint *renderbuffers); }
  static inline void glDeleteShader (GLuint shader) { GL::getInstance()->glDeleteShader (GLuint shader); }
  static inline void glDeleteTextures (GLsizei n, const GLuint *textures) { GL::getInstance()->glDeleteTextures (GLsizei n, const GLuint *textures); }
  static inline void glDepthFunc (GLenum func) { GL::getInstance()->glDepthFunc (GLenum func); }
  static inline void glDepthMask (GLboolean flag) { GL::getInstance()->glDepthMask (GLboolean flag); }
  static inline void glDepthRangef (GLfloat n, GLfloat f) { GL::getInstance()->glDepthRangef (GLfloat n, GLfloat f); }
  static inline void glDetachShader (GLuint program, GLuint shader) { GL::getInstance()->glDetachShader (GLuint program, GLuint shader); }
  static inline void glDisable (GLenum cap) { GL::getInstance()->glDisable (GLenum cap); }
  static inline void glDisableVertexAttribArray (GLuint index) { GL::getInstance()->glDisableVertexAttribArray (GLuint index); }
  static inline void glDrawArrays (GLenum mode, GLint first, GLsizei count) { GL::getInstance()->glDrawArrays (GLenum mode, GLint first, GLsizei count); }
  static inline void glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices) { GL::getInstance()->glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices); }
  static inline void glEnable (GLenum cap) { GL::getInstance()->glEnable (GLenum cap); }
  static inline void glEnableVertexAttribArray (GLuint index) { GL::getInstance()->glEnableVertexAttribArray (GLuint index); }
  static inline void glFinish (void) { GL::getInstance()->glFinish (void); }
  static inline void glFlush (void) { GL::getInstance()->glFlush (void); }
  static inline void glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) { GL::getInstance()->glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer); }
  static inline void glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) { GL::getInstance()->glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level); }
  static inline void glFrontFace (GLenum mode) { GL::getInstance()->glFrontFace (GLenum mode); }
  static inline void glGenBuffers (GLsizei n, GLuint *buffers) { GL::getInstance()->glGenBuffers (GLsizei n, GLuint *buffers); }
  static inline void glGenerateMipmap (GLenum target) { GL::getInstance()->glGenerateMipmap (GLenum target); }
  static inline void glGenFramebuffers (GLsizei n, GLuint *framebuffers) { GL::getInstance()->glGenFramebuffers (GLsizei n, GLuint *framebuffers); }
  static inline void glGenRenderbuffers (GLsizei n, GLuint *renderbuffers) { GL::getInstance()->glGenRenderbuffers (GLsizei n, GLuint *renderbuffers); }
  static inline void glGenTextures (GLsizei n, GLuint *textures) { GL::getInstance()->glGenTextures (GLsizei n, GLuint *textures); }
  static inline void glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) { GL::getInstance()->glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name); }
  static inline void glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) { GL::getInstance()->glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name); }
  static inline void glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) { GL::getInstance()->glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders); }
  static inline GLint glGetAttribLocation (GLuint program, const GLchar *name) { GL::getInstance()->glGetAttribLocation (GLuint program, const GLchar *name); }
  static inline void glGetBooleanv (GLenum pname, GLboolean *data) { GL::getInstance()->glGetBooleanv (GLenum pname, GLboolean *data); }
  static inline void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) { GL::getInstance()->glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params); }
  static inline GLenum glGetError (void) { GL::getInstance()->glGetError (void); }
  static inline void glGetFloatv (GLenum pname, GLfloat *data) { GL::getInstance()->glGetFloatv (GLenum pname, GLfloat *data); }
  static inline void glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params) { GL::getInstance()->glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params); }
  */static inline void glGetIntegerv (GLenum pname, GLint *data) { GL::getInstance()->glGetIntegerv (pname, data); }/*
  static inline void glGetProgramiv (GLuint program, GLenum pname, GLint *params) { GL::getInstance()->glGetProgramiv (GLuint program, GLenum pname, GLint *params); }
  static inline void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) { GL::getInstance()->glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog); }
  static inline void glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params) { GL::getInstance()->glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params); }
  static inline void glGetShaderiv (GLuint shader, GLenum pname, GLint *params) { GL::getInstance()->glGetShaderiv (GLuint shader, GLenum pname, GLint *params); }
  static inline void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) { GL::getInstance()->glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog); }
  static inline void glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) { GL::getInstance()->glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision); }
  static inline void glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) { GL::getInstance()->glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source); }
  static inline const GLubyte *glGetString (GLenum name) { GL::getInstance()->glGetString (GLenum name); }
  static inline void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params) { GL::getInstance()->glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params); }
  static inline void glGetTexParameteriv (GLenum target, GLenum pname, GLint *params) { GL::getInstance()->glGetTexParameteriv (GLenum target, GLenum pname, GLint *params); }
  static inline void glGetUniformfv (GLuint program, GLint location, GLfloat *params) { GL::getInstance()->glGetUniformfv (GLuint program, GLint location, GLfloat *params); }
  static inline void glGetUniformiv (GLuint program, GLint location, GLint *params) { GL::getInstance()->glGetUniformiv (GLuint program, GLint location, GLint *params); }
  static inline GLint glGetUniformLocation (GLuint program, const GLchar *name) { GL::getInstance()->glGetUniformLocation (GLuint program, const GLchar *name); }
  static inline void glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params) { GL::getInstance()->glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params); }
  static inline void glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params) { GL::getInstance()->glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params); }
  static inline void glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer) { GL::getInstance()->glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer); }
  static inline void glHint (GLenum target, GLenum mode) { GL::getInstance()->glHint (GLenum target, GLenum mode); }
  static inline GLboolean glIsBuffer (GLuint buffer) { GL::getInstance()->glIsBuffer (GLuint buffer); }
  static inline GLboolean glIsEnabled (GLenum cap) { GL::getInstance()->glIsEnabled (GLenum cap); }
  static inline GLboolean glIsFramebuffer (GLuint framebuffer) { GL::getInstance()->glIsFramebuffer (GLuint framebuffer); }
  static inline GLboolean glIsProgram (GLuint program) { GL::getInstance()->glIsProgram (GLuint program); }
  static inline GLboolean glIsRenderbuffer (GLuint renderbuffer) { GL::getInstance()->glIsRenderbuffer (GLuint renderbuffer); }
  static inline GLboolean glIsShader (GLuint shader) { GL::getInstance()->glIsShader (GLuint shader); }
  static inline GLboolean glIsTexture (GLuint texture) { GL::getInstance()->glIsTexture (GLuint texture); }
  static inline void glLineWidth (GLfloat width) { GL::getInstance()->glLineWidth (GLfloat width); }
  static inline void glLinkProgram (GLuint program) { GL::getInstance()->glLinkProgram (GLuint program); }
  static inline void glPixelStorei (GLenum pname, GLint param) { GL::getInstance()->glPixelStorei (GLenum pname, GLint param); }
  static inline void glPolygonOffset (GLfloat factor, GLfloat units) { GL::getInstance()->glPolygonOffset (GLfloat factor, GLfloat units); }
  static inline void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) { GL::getInstance()->glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels); }
  static inline void glReleaseShaderCompiler (void) { GL::getInstance()->glReleaseShaderCompiler (void); }
  static inline void glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height) { GL::getInstance()->glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height); }
  static inline void glSampleCoverage (GLfloat value, GLboolean invert) { GL::getInstance()->glSampleCoverage (GLfloat value, GLboolean invert); }
  static inline void glScissor (GLint x, GLint y, GLsizei width, GLsizei height) { GL::getInstance()->glScissor (GLint x, GLint y, GLsizei width, GLsizei height); }
  static inline void glShaderBinary (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length) { GL::getInstance()->glShaderBinary (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length); }
  static inline void glShaderSource (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) { GL::getInstance()->glShaderSource (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length); }
  static inline void glStencilFunc (GLenum func, GLint ref, GLuint mask) { GL::getInstance()->glStencilFunc (GLenum func, GLint ref, GLuint mask); }
  static inline void glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask) { GL::getInstance()->glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask); }
  static inline void glStencilMask (GLuint mask) { GL::getInstance()->glStencilMask (GLuint mask); }
  static inline void glStencilMaskSeparate (GLenum face, GLuint mask) { GL::getInstance()->glStencilMaskSeparate (GLenum face, GLuint mask); }
  static inline void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass) { GL::getInstance()->glStencilOp (GLenum fail, GLenum zfail, GLenum zpass); }
  static inline void glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) { GL::getInstance()->glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass); }
  static inline void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) { GL::getInstance()->glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels); }
  static inline void glTexParameterf (GLenum target, GLenum pname, GLfloat param) { GL::getInstance()->glTexParameterf (GLenum target, GLenum pname, GLfloat param); }
  static inline void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params) { GL::getInstance()->glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params); }
  static inline void glTexParameteri (GLenum target, GLenum pname, GLint param) { GL::getInstance()->glTexParameteri (GLenum target, GLenum pname, GLint param); }
  static inline void glTexParameteriv (GLenum target, GLenum pname, const GLint *params) { GL::getInstance()->glTexParameteriv (GLenum target, GLenum pname, const GLint *params); }
  static inline void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) { GL::getInstance()->glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels); }
  static inline void glUniform1f (GLint location, GLfloat v0) { GL::getInstance()->glUniform1f (GLint location, GLfloat v0); }
  static inline void glUniform1fv (GLint location, GLsizei count, const GLfloat *value) { GL::getInstance()->glUniform1fv (GLint location, GLsizei count, const GLfloat *value); }
  static inline void glUniform1i (GLint location, GLint v0) { GL::getInstance()->glUniform1i (GLint location, GLint v0); }
  static inline void glUniform1iv (GLint location, GLsizei count, const GLint *value) { GL::getInstance()->glUniform1iv (GLint location, GLsizei count, const GLint *value); }
  static inline void glUniform2f (GLint location, GLfloat v0, GLfloat v1) { GL::getInstance()->glUniform2f (GLint location, GLfloat v0, GLfloat v1); }
  static inline void glUniform2fv (GLint location, GLsizei count, const GLfloat *value) { GL::getInstance()->glUniform2fv (GLint location, GLsizei count, const GLfloat *value); }
  static inline void glUniform2i (GLint location, GLint v0, GLint v1) { GL::getInstance()->glUniform2i (GLint location, GLint v0, GLint v1); }
  static inline void glUniform2iv (GLint location, GLsizei count, const GLint *value) { GL::getInstance()->glUniform2iv (GLint location, GLsizei count, const GLint *value); }
  static inline void glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2) { GL::getInstance()->glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2); }
  static inline void glUniform3fv (GLint location, GLsizei count, const GLfloat *value) { GL::getInstance()->glUniform3fv (GLint location, GLsizei count, const GLfloat *value); }
  static inline void glUniform3i (GLint location, GLint v0, GLint v1, GLint v2) { GL::getInstance()->glUniform3i (GLint location, GLint v0, GLint v1, GLint v2); }
  static inline void glUniform3iv (GLint location, GLsizei count, const GLint *value) { GL::getInstance()->glUniform3iv (GLint location, GLsizei count, const GLint *value); }
  static inline void glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { GL::getInstance()->glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); }
  static inline void glUniform4fv (GLint location, GLsizei count, const GLfloat *value) { GL::getInstance()->glUniform4fv (GLint location, GLsizei count, const GLfloat *value); }
  static inline void glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3) { GL::getInstance()->glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3); }
  static inline void glUniform4iv (GLint location, GLsizei count, const GLint *value) { GL::getInstance()->glUniform4iv (GLint location, GLsizei count, const GLint *value); }
  static inline void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUseProgram (GLuint program) { GL::getInstance()->glUseProgram (GLuint program); }
  static inline void glValidateProgram (GLuint program) { GL::getInstance()->glValidateProgram (GLuint program); }
  static inline void glVertexAttrib1f (GLuint index, GLfloat x) { GL::getInstance()->glVertexAttrib1f (GLuint index, GLfloat x); }
  static inline void glVertexAttrib1fv (GLuint index, const GLfloat *v) { GL::getInstance()->glVertexAttrib1fv (GLuint index, const GLfloat *v); }
  static inline void glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y) { GL::getInstance()->glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y); }
  static inline void glVertexAttrib2fv (GLuint index, const GLfloat *v) { GL::getInstance()->glVertexAttrib2fv (GLuint index, const GLfloat *v); }
  static inline void glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z) { GL::getInstance()->glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z); }
  static inline void glVertexAttrib3fv (GLuint index, const GLfloat *v) { GL::getInstance()->glVertexAttrib3fv (GLuint index, const GLfloat *v); }
  static inline void glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) { GL::getInstance()->glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w); }
  static inline void glVertexAttrib4fv (GLuint index, const GLfloat *v) { GL::getInstance()->glVertexAttrib4fv (GLuint index, const GLfloat *v); }
  static inline void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) { GL::getInstance()->glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer); }
  static inline void glViewport (GLint x, GLint y, GLsizei width, GLsizei height) { GL::getInstance()->glViewport (GLint x, GLint y, GLsizei width, GLsizei height); }

  static inline void glReadBuffer (GLenum src) { GL::getInstance()->glReadBuffer (GLenum src); }
  static inline void glDrawRangeElements (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) { GL::getInstance()->glDrawRangeElements (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices); }
  static inline void glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) { GL::getInstance()->glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels); }
  static inline void glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) { GL::getInstance()->glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels); }
  static inline void glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) { GL::getInstance()->glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height); }
  static inline void glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) { GL::getInstance()->glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data); }
  static inline void glCompressedTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) { GL::getInstance()->glCompressedTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data); }
  static inline void glGenQueries (GLsizei n, GLuint *ids) { GL::getInstance()->glGenQueries (GLsizei n, GLuint *ids); }
  static inline void glDeleteQueries (GLsizei n, const GLuint *ids) { GL::getInstance()->glDeleteQueries (GLsizei n, const GLuint *ids); }
  static inline GLboolean glIsQuery (GLuint id) { GL::getInstance()->glIsQuery (GLuint id); }
  static inline void glBeginQuery (GLenum target, GLuint id) { GL::getInstance()->glBeginQuery (GLenum target, GLuint id); }
  static inline void glEndQuery (GLenum target) { GL::getInstance()->glEndQuery (GLenum target); }
  static inline void glGetQueryiv (GLenum target, GLenum pname, GLint *params) { GL::getInstance()->glGetQueryiv (GLenum target, GLenum pname, GLint *params); }
  static inline void glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params) { GL::getInstance()->glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params); }
  static inline GLboolean glUnmapBuffer (GLenum target) { GL::getInstance()->glUnmapBuffer (GLenum target); }
  static inline void glGetBufferPointerv (GLenum target, GLenum pname, void **params) { GL::getInstance()->glGetBufferPointerv (GLenum target, GLenum pname, void **params); }
  static inline void glDrawBuffers (GLsizei n, const GLenum *bufs) { GL::getInstance()->glDrawBuffers (GLsizei n, const GLenum *bufs); }
  static inline void glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { GL::getInstance()->glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); }
  static inline void glBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) { GL::getInstance()->glBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter); }
  static inline void glRenderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) { GL::getInstance()->glRenderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height); }
  static inline void glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) { GL::getInstance()->glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer); }
  static inline void *glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) { GL::getInstance()->glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access); }
  static inline void glFlushMappedBufferRange (GLenum target, GLintptr offset, GLsizeiptr length) { GL::getInstance()->glFlushMappedBufferRange (GLenum target, GLintptr offset, GLsizeiptr length); }
  static inline void glBindVertexArray (GLuint array) { GL::getInstance()->glBindVertexArray (GLuint array); }
  static inline void glDeleteVertexArrays (GLsizei n, const GLuint *arrays) { GL::getInstance()->glDeleteVertexArrays (GLsizei n, const GLuint *arrays); }
  static inline void glGenVertexArrays (GLsizei n, GLuint *arrays) { GL::getInstance()->glGenVertexArrays (GLsizei n, GLuint *arrays); }
  static inline GLboolean glIsVertexArray (GLuint array) { GL::getInstance()->glIsVertexArray (GLuint array); }
  static inline void glGetIntegeri_v (GLenum target, GLuint index, GLint *data) { GL::getInstance()->glGetIntegeri_v (GLenum target, GLuint index, GLint *data); }
  static inline void glBeginTransformFeedback (GLenum primitiveMode) { GL::getInstance()->glBeginTransformFeedback (GLenum primitiveMode); }
  static inline void glEndTransformFeedback (void) { GL::getInstance()->glEndTransformFeedback (void); }*/
  static inline void glBindBufferRange (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) { GL::getInstance()->glBindBufferRange (target, index, buffer, offset, size); }
  /*
  static inline void glBindBufferBase (GLenum target, GLuint index, GLuint buffer) { GL::getInstance()->glBindBufferBase (GLenum target, GLuint index, GLuint buffer); }
  static inline void glTransformFeedbackVaryings (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) { GL::getInstance()->glTransformFeedbackVaryings (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode); }
  static inline void glGetTransformFeedbackVarying (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) { GL::getInstance()->glGetTransformFeedbackVarying (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name); }
  static inline void glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) { GL::getInstance()->glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer); }
  static inline void glGetVertexAttribIiv (GLuint index, GLenum pname, GLint *params) { GL::getInstance()->glGetVertexAttribIiv (GLuint index, GLenum pname, GLint *params); }
  static inline void glGetVertexAttribIuiv (GLuint index, GLenum pname, GLuint *params) { GL::getInstance()->glGetVertexAttribIuiv (GLuint index, GLenum pname, GLuint *params); }
  static inline void glVertexAttribI4i (GLuint index, GLint x, GLint y, GLint z, GLint w) { GL::getInstance()->glVertexAttribI4i (GLuint index, GLint x, GLint y, GLint z, GLint w); }
  static inline void glVertexAttribI4ui (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) { GL::getInstance()->glVertexAttribI4ui (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w); }
  static inline void glVertexAttribI4iv (GLuint index, const GLint *v) { GL::getInstance()->glVertexAttribI4iv (GLuint index, const GLint *v); }
  static inline void glVertexAttribI4uiv (GLuint index, const GLuint *v) { GL::getInstance()->glVertexAttribI4uiv (GLuint index, const GLuint *v); }
  static inline void glGetUniformuiv (GLuint program, GLint location, GLuint *params) { GL::getInstance()->glGetUniformuiv (GLuint program, GLint location, GLuint *params); }
  static inline GLint glGetFragDataLocation (GLuint program, const GLchar *name) { GL::getInstance()->glGetFragDataLocation (GLuint program, const GLchar *name); }
  static inline void glUniform1ui (GLint location, GLuint v0) { GL::getInstance()->glUniform1ui (GLint location, GLuint v0); }
  static inline void glUniform2ui (GLint location, GLuint v0, GLuint v1) { GL::getInstance()->glUniform2ui (GLint location, GLuint v0, GLuint v1); }
  static inline void glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2) { GL::getInstance()->glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2); }
  static inline void glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) { GL::getInstance()->glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3); }
  static inline void glUniform1uiv (GLint location, GLsizei count, const GLuint *value) { GL::getInstance()->glUniform1uiv (GLint location, GLsizei count, const GLuint *value); }
  static inline void glUniform2uiv (GLint location, GLsizei count, const GLuint *value) { GL::getInstance()->glUniform2uiv (GLint location, GLsizei count, const GLuint *value); }
  static inline void glUniform3uiv (GLint location, GLsizei count, const GLuint *value) { GL::getInstance()->glUniform3uiv (GLint location, GLsizei count, const GLuint *value); }
  static inline void glUniform4uiv (GLint location, GLsizei count, const GLuint *value) { GL::getInstance()->glUniform4uiv (GLint location, GLsizei count, const GLuint *value); }
  static inline void glClearBufferiv (GLenum buffer, GLint drawbuffer, const GLint *value) { GL::getInstance()->glClearBufferiv (GLenum buffer, GLint drawbuffer, const GLint *value); }
  static inline void glClearBufferuiv (GLenum buffer, GLint drawbuffer, const GLuint *value) { GL::getInstance()->glClearBufferuiv (GLenum buffer, GLint drawbuffer, const GLuint *value); }
  static inline void glClearBufferfv (GLenum buffer, GLint drawbuffer, const GLfloat *value) { GL::getInstance()->glClearBufferfv (GLenum buffer, GLint drawbuffer, const GLfloat *value); }
  static inline void glClearBufferfi (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) { GL::getInstance()->glClearBufferfi (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil); }
  static inline const GLubyte *glGetStringi (GLenum name, GLuint index) { GL::getInstance()->glGetStringi (GLenum name, GLuint index); }
  static inline void glCopyBufferSubData (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) { GL::getInstance()->glCopyBufferSubData (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size); }
  static inline void glGetUniformIndices (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices) { GL::getInstance()->glGetUniformIndices (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices); }
  static inline void glGetActiveUniformsiv (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) { GL::getInstance()->glGetActiveUniformsiv (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params); }
  static inline GLuint glGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName) { GL::getInstance()->glGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName); }
  static inline void glGetActiveUniformBlockiv (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) { GL::getInstance()->glGetActiveUniformBlockiv (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params); }
  static inline void glGetActiveUniformBlockName (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) { GL::getInstance()->glGetActiveUniformBlockName (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName); }
  static inline void glUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) { GL::getInstance()->glUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding); }
  static inline void glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount) { GL::getInstance()->glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount); }
  static inline void glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) { GL::getInstance()->glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount); }
  static inline GLsync glFenceSync (GLenum condition, GLbitfield flags) { GL::getInstance()->glFenceSync (GLenum condition, GLbitfield flags); }
  static inline GLboolean glIsSync (GLsync sync) { GL::getInstance()->glIsSync (GLsync sync); }
  static inline void glDeleteSync (GLsync sync) { GL::getInstance()->glDeleteSync (GLsync sync); }
  static inline GLenum glClientWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout) { GL::getInstance()->glClientWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout); }
  static inline void glWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout) { GL::getInstance()->glWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout); }
  static inline void glGetInteger64v (GLenum pname, GLint64 *data) { GL::getInstance()->glGetInteger64v (GLenum pname, GLint64 *data); }
  static inline void glGetSynciv (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) { GL::getInstance()->glGetSynciv (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values); }
  static inline void glGetInteger64i_v (GLenum target, GLuint index, GLint64 *data) { GL::getInstance()->glGetInteger64i_v (GLenum target, GLuint index, GLint64 *data); }
  static inline void glGetBufferParameteri64v (GLenum target, GLenum pname, GLint64 *params) { GL::getInstance()->glGetBufferParameteri64v (GLenum target, GLenum pname, GLint64 *params); }
  static inline void glGenSamplers (GLsizei count, GLuint *samplers) { GL::getInstance()->glGenSamplers (GLsizei count, GLuint *samplers); }
  static inline void glDeleteSamplers (GLsizei count, const GLuint *samplers) { GL::getInstance()->glDeleteSamplers (GLsizei count, const GLuint *samplers); }
  static inline GLboolean glIsSampler (GLuint sampler) { GL::getInstance()->glIsSampler (GLuint sampler); }
  static inline void glBindSampler (GLuint unit, GLuint sampler) { GL::getInstance()->glBindSampler (GLuint unit, GLuint sampler); }
  static inline void glSamplerParameteri (GLuint sampler, GLenum pname, GLint param) { GL::getInstance()->glSamplerParameteri (GLuint sampler, GLenum pname, GLint param); }
  static inline void glSamplerParameteriv (GLuint sampler, GLenum pname, const GLint *param) { GL::getInstance()->glSamplerParameteriv (GLuint sampler, GLenum pname, const GLint *param); }
  static inline void glSamplerParameterf (GLuint sampler, GLenum pname, GLfloat param) { GL::getInstance()->glSamplerParameterf (GLuint sampler, GLenum pname, GLfloat param); }
  static inline void glSamplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param) { GL::getInstance()->glSamplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param); }
  static inline void glGetSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params) { GL::getInstance()->glGetSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params); }
  static inline void glGetSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params) { GL::getInstance()->glGetSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params); }
  static inline void glVertexAttribDivisor (GLuint index, GLuint divisor) { GL::getInstance()->glVertexAttribDivisor (GLuint index, GLuint divisor); }
  static inline void glBindTransformFeedback (GLenum target, GLuint id) { GL::getInstance()->glBindTransformFeedback (GLenum target, GLuint id); }
  static inline void glDeleteTransformFeedbacks (GLsizei n, const GLuint *ids) { GL::getInstance()->glDeleteTransformFeedbacks (GLsizei n, const GLuint *ids); }
  static inline void glGenTransformFeedbacks (GLsizei n, GLuint *ids) { GL::getInstance()->glGenTransformFeedbacks (GLsizei n, GLuint *ids); }
  static inline GLboolean glIsTransformFeedback (GLuint id) { GL::getInstance()->glIsTransformFeedback (GLuint id); }
  static inline void glPauseTransformFeedback (void) { GL::getInstance()->glPauseTransformFeedback (void); }
  static inline void glResumeTransformFeedback (void) { GL::getInstance()->glResumeTransformFeedback (void); }
  static inline void glGetProgramBinary (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) { GL::getInstance()->glGetProgramBinary (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary); }
  static inline void glProgramBinary (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length) { GL::getInstance()->glProgramBinary (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length); }
  static inline void glProgramParameteri (GLuint program, GLenum pname, GLint value) { GL::getInstance()->glProgramParameteri (GLuint program, GLenum pname, GLint value); }
  static inline void glInvalidateFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments) { GL::getInstance()->glInvalidateFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments); }
  static inline void glInvalidateSubFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) { GL::getInstance()->glInvalidateSubFramebuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height); }
  static inline void glTexStorage2D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) { GL::getInstance()->glTexStorage2D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height); }
  static inline void glTexStorage3D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) { GL::getInstance()->glTexStorage3D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth); }
  static inline void glGetInternalformativ (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params) { GL::getInstance()->glGetInternalformativ (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params); }
  */
};

#endif // OPENGLFUNCTIONS_H

