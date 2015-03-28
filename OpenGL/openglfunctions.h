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

#include <OpenGLFunctions_4_3_Core>

class OpenGLFunctions : public OpenGLFunctions_4_3_Core
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
  static int getInteger(GLenum property);

  template <GLenum property>
  static int getInteger()
  {
    static int value = getInteger(property);
    return value;
  }

  static void pushViewport();
  static void popViewport();

  // 2.0
  static inline void glActiveTexture (GLenum texture)
  {
    GL::getInstance()->glActiveTexture (texture);
  }

  static inline void glAttachShader (GLuint program, GLuint shader)
  {
    GL::getInstance()->glAttachShader (program, shader);
  }

  static inline void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name)
  {
    GL::getInstance()->glBindAttribLocation (program, index, name);
  }

  static inline void glBindBuffer (GLenum target, GLuint buffer)
  {
    GL::getInstance()->glBindBuffer (target, buffer);
  }

  static inline void glBindFramebuffer (GLenum target, GLuint framebuffer)
  {
    GL::getInstance()->glBindFramebuffer (target, framebuffer);
  }

  static inline void glBindRenderbuffer (GLenum target, GLuint renderbuffer)
  {
    GL::getInstance()->glBindRenderbuffer (target, renderbuffer);
  }

  static inline void glBindTexture (GLenum target, GLuint texture)
  {
    GL::getInstance()->glBindTexture (target, texture);
  }

  static inline void glBlendColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
  {
    GL::getInstance()->glBlendColor (red, green, blue, alpha);
  }

  static inline void glBlendEquation (GLenum mode)
  {
    GL::getInstance()->glBlendEquation (mode);
  }

  static inline void glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha)
  {
    GL::getInstance()->glBlendEquationSeparate (modeRGB, modeAlpha);
  }

  static inline void glBlendFunc (GLenum sfactor, GLenum dfactor)
  {
    GL::getInstance()->glBlendFunc (sfactor, dfactor);
  }

  static inline void glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
  {
    GL::getInstance()->glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
  }

  static inline void glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage)
  {
    GL::getInstance()->glBufferData (target, size, data, usage);
  }

  static inline void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
  {
    GL::getInstance()->glBufferSubData (target, offset, size, data);
  }

  static inline GLenum glCheckFramebufferStatus (GLenum target)
  {
    GL::getInstance()->glCheckFramebufferStatus (target);
  }

  static inline void glClear (GLbitfield mask)
  {
    GL::getInstance()->glClear (mask);
  }

  static inline void glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
  {
    GL::getInstance()->glClearColor (red, green, blue, alpha);
  }

  static inline void glClearDepthf (GLfloat d)
  {
    GL::getInstance()->glClearDepthf (d);
  }

  static inline void glClearStencil (GLint s)
  {
    GL::getInstance()->glClearStencil (s);
  }

  static inline void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
  {
    GL::getInstance()->glColorMask (red, green, blue, alpha);
  }

  static inline void glCompileShader (GLuint shader)
  {
    GL::getInstance()->glCompileShader (shader);
  }

  static inline void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
  {
    GL::getInstance()->glCompressedTexImage2D (target, level, internalformat, width, height, border, imageSize, data);
  }

  static inline void glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
  {
    GL::getInstance()->glCompressedTexSubImage2D (target, level, xoffset, yoffset, width, height, format, imageSize, data);
  }

  static inline void glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
  {
    GL::getInstance()->glCopyTexImage2D (target, level, internalformat, x, y, width, height, border);
  }

  static inline void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
  {
    GL::getInstance()->glCopyTexSubImage2D (target, level, xoffset, yoffset, x, y, width, height);
  }

  static inline GLuint glCreateProgram (void)
  {
    return GL::getInstance()->glCreateProgram ();
  }

  static inline GLuint glCreateShader (GLenum type)
  {
    return GL::getInstance()->glCreateShader (type);
  }

  static inline void glCullFace (GLenum mode)
  {
    GL::getInstance()->glCullFace (mode);
  }

  static inline void glDeleteBuffers (GLsizei n, const GLuint *buffers)
  {
    GL::getInstance()->glDeleteBuffers (n, buffers);
  }

  static inline void glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers)
  {
    GL::getInstance()->glDeleteFramebuffers (n, framebuffers);
  }

  static inline void glDeleteProgram (GLuint program)
  {
    GL::getInstance()->glDeleteProgram (program);
  }

  static inline void glDeleteRenderbuffers (GLsizei n, const GLuint *renderbuffers)
  {
    GL::getInstance()->glDeleteRenderbuffers (n, renderbuffers);
  }

  static inline void glDeleteShader (GLuint shader)
  {
    GL::getInstance()->glDeleteShader ( shader);
  }

  static inline void glDeleteTextures (GLsizei n, const GLuint *textures)
  {
    GL::getInstance()->glDeleteTextures (n, textures);
  }

  static inline void glDepthFunc (GLenum func)
  {
    GL::getInstance()->glDepthFunc (func);
  }

  static inline void glDepthMask (GLboolean flag)
  {
    GL::getInstance()->glDepthMask (flag);
  }

  static inline void glDepthRange (GLfloat n, GLfloat f)
  {
    GL::getInstance()->glDepthRange (n, f);
  }

  static inline void glDetachShader (GLuint program, GLuint shader)
  {
    GL::getInstance()->glDetachShader (program, shader);
  }

  static inline void glDisable (GLenum cap)
  {
    GL::getInstance()->glDisable (cap);
  }

  static inline void glDisableVertexAttribArray (GLuint index)
  {
    GL::getInstance()->glDisableVertexAttribArray (index);
  }

  static inline void glDrawArrays (GLenum mode, GLint first, GLsizei count)
  {
    GL::getInstance()->glDrawArrays (mode, first, count);
  }

  static inline void glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices)
  {
    GL::getInstance()->glDrawElements (mode, count, type, indices);
  }

  static inline void glEnable (GLenum cap)
  {
    GL::getInstance()->glEnable (cap);
  }

  static inline void glEnableVertexAttribArray (GLuint index)
  {
    GL::getInstance()->glEnableVertexAttribArray (index);
  }

  static inline void glFinish (void)
  {
    GL::getInstance()->glFinish ();
  }

  static inline void glFlush (void)
  {
    GL::getInstance()->glFlush ();
  }

  static inline void glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
  {
    GL::getInstance()->glFramebufferRenderbuffer (target, attachment, renderbuffertarget, renderbuffer);
  }

  static inline void glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
  {
    GL::getInstance()->glFramebufferTexture2D (target, attachment, textarget, texture, level);
  }

  static inline void glFrontFace (GLenum mode)
  {
    GL::getInstance()->glFrontFace (mode);
  }

  static inline void glGenBuffers (GLsizei n, GLuint *buffers)
  {
    GL::getInstance()->glGenBuffers (n, buffers);
  }

  static inline void glGenerateMipmap (GLenum target)
  {
    GL::getInstance()->glGenerateMipmap (target);
  }

  static inline void glGenFramebuffers (GLsizei n, GLuint *framebuffers)
  {
    GL::getInstance()->glGenFramebuffers (n, framebuffers);
  }

  static inline void glGenRenderbuffers (GLsizei n, GLuint *renderbuffers)
  {
    GL::getInstance()->glGenRenderbuffers (n, renderbuffers);
  }

  static inline void glGenTextures (GLsizei n, GLuint *textures)
  {
    GL::getInstance()->glGenTextures (n, textures);
  }

  static inline void glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
  {
    GL::getInstance()->glGetActiveAttrib (program, index, bufSize, length, size, type, name);
  }

  static inline void glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
  {
    GL::getInstance()->glGetActiveUniform (program, index, bufSize, length, size, type, name);
  }

  static inline void glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)
  {
    GL::getInstance()->glGetAttachedShaders (program, maxCount, count, shaders);
  }

  static inline GLint glGetAttribLocation (GLuint program, const GLchar *name)
  {
    return GL::getInstance()->glGetAttribLocation (program, name);
  }

  static inline void glGetBooleanv (GLenum pname, GLboolean *data)
  {
    GL::getInstance()->glGetBooleanv (pname, data);
  }

  static inline void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetBufferParameteriv (target, pname, params);
  }

  static inline GLenum glGetError (void)
  {
    return GL::getInstance()->glGetError ();
  }

  static inline void glGetFloatv (GLenum pname, GLfloat *data)
  {
    GL::getInstance()->glGetFloatv (pname, data);
  }

  static inline void glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetFramebufferAttachmentParameteriv (target, attachment, pname, params);
  }

  static inline void glGetIntegerv (GLenum pname, GLint *data)
  {
    GL::getInstance()->glGetIntegerv (pname, data);
  }

  static inline void glGetProgramiv (GLuint program, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetProgramiv (program, pname, params);
  }

  static inline void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
  {
    GL::getInstance()->glGetProgramInfoLog (program, bufSize, length, infoLog);
  }

  static inline void glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetRenderbufferParameteriv (target, pname, params);
  }

  static inline void glGetShaderiv (GLuint shader, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetShaderiv (shader, pname, params);
  }

  static inline void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
  {
    GL::getInstance()->glGetShaderInfoLog (shader, bufSize, length, infoLog);
  }

  static inline void glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
  {
    GL::getInstance()->glGetShaderSource (shader, bufSize, length, source);
  }

  static inline const GLubyte *glGetString (GLenum name)
  {
    return GL::getInstance()->glGetString (name);
  }

  static inline void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params)
  {
    GL::getInstance()->glGetTexParameterfv (target, pname, params);
  }

  static inline void glGetTexParameteriv (GLenum target, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetTexParameteriv (target, pname, params);
  }

  static inline void glGetUniformfv (GLuint program, GLint location, GLfloat *params)
  {
    GL::getInstance()->glGetUniformfv (program, location, params);
  }

  static inline void glGetUniformiv (GLuint program, GLint location, GLint *params)
  {
    GL::getInstance()->glGetUniformiv (program, location, params);
  }

  static inline GLint glGetUniformLocation (GLuint program, const GLchar *name)
  {
    return GL::getInstance()->glGetUniformLocation (program, name);
  }

  static inline void glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params)
  {
    GL::getInstance()->glGetVertexAttribfv (index, pname, params);
  }

  static inline void glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetVertexAttribiv (index, pname, params);
  }

  static inline void glGetVertexAttribPointerv (GLuint index, GLenum pname, void **pointer)
  {
    GL::getInstance()->glGetVertexAttribPointerv (index, pname, pointer);
  }

  static inline void glHint (GLenum target, GLenum mode)
  {
    GL::getInstance()->glHint (target, mode);
  }

  static inline GLboolean glIsBuffer (GLuint buffer)
  {
    return GL::getInstance()->glIsBuffer (buffer);
  }

  static inline GLboolean glIsEnabled (GLenum cap)
  {
    return GL::getInstance()->glIsEnabled (cap);
  }

  static inline GLboolean glIsFramebuffer (GLuint framebuffer)
  {
    return GL::getInstance()->glIsFramebuffer (framebuffer);
  }

  static inline GLboolean glIsProgram (GLuint program)
  {
    return GL::getInstance()->glIsProgram (program);
  }

  static inline GLboolean glIsRenderbuffer (GLuint renderbuffer)
  {
    return GL::getInstance()->glIsRenderbuffer (renderbuffer);
  }

  static inline GLboolean glIsShader (GLuint shader)
  {
    return GL::getInstance()->glIsShader (shader);
  }

  static inline GLboolean glIsTexture (GLuint texture)
  {
    return GL::getInstance()->glIsTexture (texture);
  }

  static inline void glLineWidth (GLfloat width)
  {
    GL::getInstance()->glLineWidth (width);
  }

  static inline void glLinkProgram (GLuint program)
  {
    GL::getInstance()->glLinkProgram (program);
  }

  static inline void glPixelStorei (GLenum pname, GLint param)
  {
    GL::getInstance()->glPixelStorei (pname, param);
  }

  static inline void glPolygonOffset (GLfloat factor, GLfloat units)
  {
    GL::getInstance()->glPolygonOffset (factor, units);
  }

  static inline void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
  {
    GL::getInstance()->glReadPixels (x, y, width, height, format, type, pixels);
  }

  static inline void glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
  {
    GL::getInstance()->glRenderbufferStorage (target, internalformat, width, height);
  }

  static inline void glSampleCoverage (GLfloat value, GLboolean invert)
  {
    GL::getInstance()->glSampleCoverage (value, invert);
  }

  static inline void glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
  {
    GL::getInstance()->glScissor (x, y, width, height);
  }

  static inline void glShaderSource (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
  {
    GL::getInstance()->glShaderSource (shader, count, string, length);
  }

  static inline void glStencilFunc (GLenum func, GLint ref, GLuint mask)
  {
    GL::getInstance()->glStencilFunc (func, ref, mask);
  }

  static inline void glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask)
  {
    GL::getInstance()->glStencilFuncSeparate (face, func, ref, mask);
  }

  static inline void glStencilMask (GLuint mask)
  {
    GL::getInstance()->glStencilMask (mask);
  }

  static inline void glStencilMaskSeparate (GLenum face, GLuint mask)
  {
    GL::getInstance()->glStencilMaskSeparate (face, mask);
  }

  static inline void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
  {
    GL::getInstance()->glStencilOp (fail, zfail, zpass);
  }

  static inline void glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
  {
    GL::getInstance()->glStencilOpSeparate (face, sfail, dpfail, dppass);
  }

  static inline void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
  {
    GL::getInstance()->glTexImage2D (target, level, internalformat, width, height, border, format, type, pixels);
  }

  static inline void glTexParameterf (GLenum target, GLenum pname, GLfloat param)
  {
    GL::getInstance()->glTexParameterf (target, pname, param);
  }

  static inline void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
  {
    GL::getInstance()->glTexParameterfv (target, pname, params);
  }

  static inline void glTexParameteri (GLenum target, GLenum pname, GLint param)
  {
    GL::getInstance()->glTexParameteri (target, pname, param);
  }

  static inline void glTexParameteriv (GLenum target, GLenum pname, const GLint *params)
  {
    GL::getInstance()->glTexParameteriv (target, pname, params);
  }

  static inline void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
  {
    GL::getInstance()->glTexSubImage2D (target, level, xoffset, yoffset, width, height, format, type, pixels);
  }

  static inline void glUniform1f (GLint location, GLfloat v0)
  {
    GL::getInstance()->glUniform1f (location, v0);
  }

  static inline void glUniform1fv (GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glUniform1fv (location, count, value);
  }

  static inline void glUniform1i (GLint location, GLint v0)
  {
    GL::getInstance()->glUniform1i (location, v0);
  }

  static inline void glUniform1iv (GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glUniform1iv (location, count, value);
  }

  static inline void glUniform2f (GLint location, GLfloat v0, GLfloat v1)
  {
    GL::getInstance()->glUniform2f (location, v0, v1);
  }

  static inline void glUniform2fv (GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glUniform2fv (location, count, value);
  }

  static inline void glUniform2i (GLint location, GLint v0, GLint v1)
  {
    GL::getInstance()->glUniform2i (location, v0, v1);
  }

  static inline void glUniform2iv (GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glUniform2iv (location, count, value);
  }

  static inline void glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
  {
    GL::getInstance()->glUniform3f (location, v0, v1, v2);
  }

  static inline void glUniform3fv (GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glUniform3fv (location, count, value);
  }

  static inline void glUniform3i (GLint location, GLint v0, GLint v1, GLint v2)
  {
    GL::getInstance()->glUniform3i (location, v0, v1, v2);
  }

  static inline void glUniform3iv (GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glUniform3iv (location, count, value);
  }

  static inline void glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
  {
    GL::getInstance()->glUniform4f (location, v0, v1, v2, v3);
  }

  static inline void glUniform4fv (GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glUniform4fv (location, count, value);
  }

  static inline void glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
  {
    GL::getInstance()->glUniform4i (location, v0, v1, v2, v3);
  }

  static inline void glUniform4iv (GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glUniform4iv (location, count, value);
  }

  static inline void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix2fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix3fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix4fv (location, count, transpose, value);
  }

  static inline void glUseProgram (GLuint program)
  {
    GL::getInstance()->glUseProgram (program);
  }

  static inline void glValidateProgram (GLuint program)
  {
    GL::getInstance()->glValidateProgram (program);
  }

  static inline void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
  {
    GL::getInstance()->glVertexAttribPointer (index, size, type, normalized, stride, pointer);
  }

  static void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);


  // 3.0
  static inline void glReadBuffer (GLenum src)
  {
    GL::getInstance()->glReadBuffer (src);
  }

  static inline void glDrawRangeElements (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices)
  {
    GL::getInstance()->glDrawRangeElements (mode, start, end, count, type, indices);
  }

  static inline void glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)
  {
    GL::getInstance()->glTexImage3D (target, level, internalformat, width, height, depth, border, format, type, pixels);
  }

  static inline void glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
  {
    GL::getInstance()->glTexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
  }

  static inline void glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
  {
    GL::getInstance()->glCopyTexSubImage3D (target, level, xoffset, yoffset, zoffset, x, y, width, height);
  }

  static inline void glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data)
  {
    GL::getInstance()->glCompressedTexImage3D (target, level, internalformat, width, height, depth, border, imageSize, data);
  }

  static inline void glCompressedTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)
  {
    GL::getInstance()->glCompressedTexSubImage3D (target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
  }

  static inline void glGenQueries (GLsizei n, GLuint *ids)
  {
    GL::getInstance()->glGenQueries (n, ids);
  }

  static inline void glDeleteQueries (GLsizei n, const GLuint *ids)
  {
    GL::getInstance()->glDeleteQueries (n, ids);
  }

  static inline GLboolean glIsQuery (GLuint id)
  {
    return GL::getInstance()->glIsQuery (id);
  }

  static inline void glBeginQuery (GLenum target, GLuint id)
  {
    GL::getInstance()->glBeginQuery (target, id);
  }

  static inline void glEndQuery (GLenum target)
  {
    GL::getInstance()->glEndQuery (target);
  }

  static inline void glGetQueryiv (GLenum target, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetQueryiv (target, pname, params);
  }

  static inline void glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params)
  {
    GL::getInstance()->glGetQueryObjectuiv (id, pname, params);
  }

  static inline GLboolean glUnmapBuffer (GLenum target)
  {
    return GL::getInstance()->glUnmapBuffer (target);
  }

  static inline void glGetBufferPointerv (GLenum target, GLenum pname, void **params)
  {
    GL::getInstance()->glGetBufferPointerv (target, pname, params);
  }

  static inline void glDrawBuffers (GLsizei n, const GLenum *bufs)
  {
    GL::getInstance()->glDrawBuffers (n, bufs);
  }

  static inline void glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix2x3fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix3x2fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix2x4fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix4x2fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix3x4fv (location, count, transpose, value);
  }

  static inline void glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glUniformMatrix4x3fv (location, count, transpose, value);
  }

  static inline void glBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
  {
    GL::getInstance()->glBlitFramebuffer (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
  }

  static inline void glRenderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
  {
    GL::getInstance()->glRenderbufferStorageMultisample (target, samples, internalformat, width, height);
  }

  static inline void glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
  {
    GL::getInstance()->glFramebufferTextureLayer (target, attachment, texture, level, layer);
  }

  static inline void *glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
  {
    GL::getInstance()->glMapBufferRange (target, offset, length, access);
  }

  static inline void glFlushMappedBufferRange (GLenum target, GLintptr offset, GLsizeiptr length)
  {
    GL::getInstance()->glFlushMappedBufferRange (target, offset, length);
  }

  static inline void glBindVertexArray (GLuint array)
  {
    GL::getInstance()->glBindVertexArray (array);
  }

  static inline void glDeleteVertexArrays (GLsizei n, const GLuint *arrays)
  {
    GL::getInstance()->glDeleteVertexArrays (n, arrays);
  }

  static inline void glGenVertexArrays (GLsizei n, GLuint *arrays)
  {
    GL::getInstance()->glGenVertexArrays (n, arrays);
  }

  static inline GLboolean glIsVertexArray (GLuint array)
  {
    return GL::getInstance()->glIsVertexArray (array);
  }

  static inline void glGetIntegeri_v (GLenum target, GLuint index, GLint *data)
  {
    GL::getInstance()->glGetIntegeri_v (target, index, data);
  }

  static inline void glBeginTransformFeedback (GLenum primitiveMode)
  {
    GL::getInstance()->glBeginTransformFeedback (primitiveMode);
  }

  static inline void glEndTransformFeedback (void)
  {
    GL::getInstance()->glEndTransformFeedback ();
  }

  static inline void glBindBufferRange (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
  {
    GL::getInstance()->glBindBufferRange (target, index, buffer, offset, size);
  }

  static inline void glBindBufferBase (GLenum target, GLuint index, GLuint buffer)
  {
    GL::getInstance()->glBindBufferBase (target, index, buffer);
  }

  static inline void glTransformFeedbackVaryings (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode)
  {
    GL::getInstance()->glTransformFeedbackVaryings (program, count, varyings, bufferMode);
  }

  static inline void glGetTransformFeedbackVarying (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
  {
    GL::getInstance()->glGetTransformFeedbackVarying (program, index, bufSize, length, size, type, name);
  }

  static inline void glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)
  {
    GL::getInstance()->glVertexAttribIPointer (index, size, type, stride, pointer);
  }

  static inline void glGetVertexAttribIiv (GLuint index, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetVertexAttribIiv (index, pname, params);
  }

  static inline void glGetVertexAttribIuiv (GLuint index, GLenum pname, GLuint *params)
  {
    GL::getInstance()->glGetVertexAttribIuiv (index, pname, params);
  }

  static inline void glGetUniformuiv (GLuint program, GLint location, GLuint *params)
  {
    GL::getInstance()->glGetUniformuiv (program, location, params);
  }

  static inline GLint glGetFragDataLocation (GLuint program, const GLchar *name)
  {
    return GL::getInstance()->glGetFragDataLocation (program, name);
  }

  static inline void glUniform1ui (GLint location, GLuint v0)
  {
    GL::getInstance()->glUniform1ui (location, v0);
  }

  static inline void glUniform2ui (GLint location, GLuint v0, GLuint v1)
  {
    GL::getInstance()->glUniform2ui (location, v0, v1);
  }

  static inline void glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2)
  {
    GL::getInstance()->glUniform3ui (location, v0, v1, v2);
  }

  static inline void glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
  {
    GL::getInstance()->glUniform4ui (location, v0, v1, v2, v3);
  }

  static inline void glUniform1uiv (GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glUniform1uiv (location, count, value);
  }

  static inline void glUniform2uiv (GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glUniform2uiv (location, count, value);
  }

  static inline void glUniform3uiv (GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glUniform3uiv (location, count, value);
  }

  static inline void glUniform4uiv (GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glUniform4uiv (location, count, value);
  }

  static inline void glClearBufferiv (GLenum buffer, GLint drawbuffer, const GLint *value)
  {
    GL::getInstance()->glClearBufferiv (buffer, drawbuffer, value);
  }

  static inline void glClearBufferuiv (GLenum buffer, GLint drawbuffer, const GLuint *value)
  {
    GL::getInstance()->glClearBufferuiv (buffer, drawbuffer, value);
  }

  static inline void glClearBufferfv (GLenum buffer, GLint drawbuffer, const GLfloat *value)
  {
    GL::getInstance()->glClearBufferfv (buffer, drawbuffer, value);
  }

  static inline void glClearBufferfi (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
  {
    GL::getInstance()->glClearBufferfi (buffer, drawbuffer, depth, stencil);
  }

  static inline const GLubyte *glGetStringi (GLenum name, GLuint index)
  {
    return GL::getInstance()->glGetStringi (name, index);
  }

  static inline void glCopyBufferSubData (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
  {
    GL::getInstance()->glCopyBufferSubData (readTarget, writeTarget, readOffset, writeOffset, size);
  }

  static inline void glGetUniformIndices (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)
  {
    GL::getInstance()->glGetUniformIndices (program, uniformCount, uniformNames, uniformIndices);
  }

  static inline void glGetActiveUniformsiv (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetActiveUniformsiv (program, uniformCount, uniformIndices, pname, params);
  }

  static inline GLuint glGetUniformBlockIndex (GLuint program, const GLchar *uniformBlockName)
  {
    return GL::getInstance()->glGetUniformBlockIndex (program, uniformBlockName);
  }

  static inline void glGetActiveUniformBlockiv (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetActiveUniformBlockiv (program, uniformBlockIndex, pname, params);
  }

  static inline void glGetActiveUniformBlockName (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
  {
    GL::getInstance()->glGetActiveUniformBlockName (program, uniformBlockIndex, bufSize, length, uniformBlockName);
  }

  static inline void glUniformBlockBinding (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
  {
    GL::getInstance()->glUniformBlockBinding (program, uniformBlockIndex, uniformBlockBinding);
  }

  static inline void glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
  {
    GL::getInstance()->glDrawArraysInstanced (mode, first, count, instancecount);
  }

  static inline void glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
  {
    GL::getInstance()->glDrawElementsInstanced (mode, count, type, indices, instancecount);
  }

  static inline GLsync glFenceSync (GLenum condition, GLbitfield flags)
  {
    return GL::getInstance()->glFenceSync (condition, flags);
  }

  static inline GLboolean glIsSync (GLsync sync)
  {
    GL::getInstance()->glIsSync (sync);
  }

  static inline void glDeleteSync (GLsync sync)
  {
    GL::getInstance()->glDeleteSync (sync);
  }

  static inline GLenum glClientWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout)
  {
    return GL::getInstance()->glClientWaitSync (sync, flags, timeout);
  }

  static inline void glWaitSync (GLsync sync, GLbitfield flags, GLuint64 timeout)
  {
    GL::getInstance()->glWaitSync (sync, flags, timeout);
  }

  static inline void glGetInteger64v (GLenum pname, GLint64 *data)
  {
    GL::getInstance()->glGetInteger64v (pname, data);
  }

  static inline void glGetSynciv (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)
  {
    GL::getInstance()->glGetSynciv (sync, pname, bufSize, length, values);
  }

  static inline void glGetInteger64i_v (GLenum target, GLuint index, GLint64 *data)
  {
    GL::getInstance()->glGetInteger64i_v (target, index, data);
  }

  static inline void glGetBufferParameteri64v (GLenum target, GLenum pname, GLint64 *params)
  {
    GL::getInstance()->glGetBufferParameteri64v (target, pname, params);
  }

  static inline void glGenSamplers (GLsizei count, GLuint *samplers)
  {
    GL::getInstance()->glGenSamplers (count, samplers);
  }

  static inline void glDeleteSamplers (GLsizei count, const GLuint *samplers)
  {
    GL::getInstance()->glDeleteSamplers (count, samplers);
  }

  static inline GLboolean glIsSampler (GLuint sampler)
  {
    return GL::getInstance()->glIsSampler (sampler);
  }

  static inline void glBindSampler (GLuint unit, GLuint sampler)
  {
    GL::getInstance()->glBindSampler (unit, sampler);
  }

  static inline void glSamplerParameteri (GLuint sampler, GLenum pname, GLint param)
  {
    GL::getInstance()->glSamplerParameteri (sampler, pname, param);
  }

  static inline void glSamplerParameteriv (GLuint sampler, GLenum pname, const GLint *param)
  {
    GL::getInstance()->glSamplerParameteriv (sampler, pname, param);
  }

  static inline void glSamplerParameterf (GLuint sampler, GLenum pname, GLfloat param)
  {
    GL::getInstance()->glSamplerParameterf (sampler, pname, param);
  }

  static inline void glSamplerParameterfv (GLuint sampler, GLenum pname, const GLfloat *param)
  {
    GL::getInstance()->glSamplerParameterfv (sampler, pname, param);
  }

  static inline void glGetSamplerParameteriv (GLuint sampler, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetSamplerParameteriv (sampler, pname, params);
  }

  static inline void glGetSamplerParameterfv (GLuint sampler, GLenum pname, GLfloat *params)
  {
    GL::getInstance()->glGetSamplerParameterfv (sampler, pname, params);
  }

  static inline void glVertexAttribDivisor (GLuint index, GLuint divisor)
  {
    GL::getInstance()->glVertexAttribDivisor (index, divisor);
  }

  // gles 3.1
  static inline void glDispatchCompute (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
  {
    GL::getInstance()->glDispatchCompute (num_groups_x, num_groups_y, num_groups_z);
  }

  static inline void glDispatchComputeIndirect (GLintptr indirect)
  {
    GL::getInstance()->glDispatchComputeIndirect (indirect);
  }

  static inline void glDrawArraysIndirect (GLenum mode, const void *indirect)
  {
    GL::getInstance()->glDrawArraysIndirect (mode, indirect);
  }

  static inline void glDrawElementsIndirect (GLenum mode, GLenum type, const void *indirect)
  {
    GL::getInstance()->glDrawElementsIndirect (mode, type, indirect);
  }

  static inline void glFramebufferParameteri (GLenum target, GLenum pname, GLint param)
  {
    GL::getInstance()->glFramebufferParameteri (target, pname, param);
  }

  static inline void glGetFramebufferParameteriv (GLenum target, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetFramebufferParameteriv (target, pname, params);
  }

  static inline void glGetProgramInterfaceiv (GLuint program, GLenum programInterface, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetProgramInterfaceiv (program, programInterface, pname, params);
  }

  static inline GLuint glGetProgramResourceIndex (GLuint program, GLenum programInterface, const GLchar *name)
  {
    GL::getInstance()->glGetProgramResourceIndex (program, programInterface, name);
  }

  static inline void glGetProgramResourceName (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name)
  {
    GL::getInstance()->glGetProgramResourceName (program, programInterface, index, bufSize, length, name);
  }

  static inline void glGetProgramResourceiv (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params)
  {
    GL::getInstance()->glGetProgramResourceiv (program, programInterface, index, propCount, props, bufSize, length, params);
  }

  static inline GLint glGetProgramResourceLocation (GLuint program, GLenum programInterface, const GLchar *name)
  {
    GL::getInstance()->glGetProgramResourceLocation (program, programInterface, name);
  }

  static inline void glUseProgramStages (GLuint pipeline, GLbitfield stages, GLuint program)
  {
    GL::getInstance()->glUseProgramStages (pipeline, stages, program);
  }

  static inline void glActiveShaderProgram (GLuint pipeline, GLuint program)
  {
    GL::getInstance()->glActiveShaderProgram (pipeline, program);
  }

  static inline GLuint glCreateShaderProgramv (GLenum type, GLsizei count, const GLchar *const*strings)
  {
    GL::getInstance()->glCreateShaderProgramv (type, count, strings);
  }

  static inline void glBindProgramPipeline (GLuint pipeline)
  {
    GL::getInstance()->glBindProgramPipeline (pipeline);
  }

  static inline void glDeleteProgramPipelines (GLsizei n, const GLuint *pipelines)
  {
    GL::getInstance()->glDeleteProgramPipelines (n, pipelines);
  }

  static inline void glGenProgramPipelines (GLsizei n, GLuint *pipelines)
  {
    GL::getInstance()->glGenProgramPipelines (n, pipelines);
  }

  static inline GLboolean glIsProgramPipeline (GLuint pipeline)
  {
    GL::getInstance()->glIsProgramPipeline (pipeline);
  }

  static inline void glGetProgramPipelineiv (GLuint pipeline, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetProgramPipelineiv (pipeline, pname, params);
  }

  static inline void glProgramUniform1i (GLuint program, GLint location, GLint v0)
  {
    GL::getInstance()->glProgramUniform1i (program, location, v0);
  }

  static inline void glProgramUniform2i (GLuint program, GLint location, GLint v0, GLint v1)
  {
    GL::getInstance()->glProgramUniform2i (program, location, v0, v1);
  }

  static inline void glProgramUniform3i (GLuint program, GLint location, GLint v0, GLint v1, GLint v2)
  {
    GL::getInstance()->glProgramUniform3i (program, location, v0, v1, v2);
  }

  static inline void glProgramUniform4i (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
  {
    GL::getInstance()->glProgramUniform4i (program, location, v0, v1,  v2, v3);
  }

  static inline void glProgramUniform1ui (GLuint program, GLint location, GLuint v0)
  {
    GL::getInstance()->glProgramUniform1ui (program, location, v0);
  }

  static inline void glProgramUniform2ui (GLuint program, GLint location, GLuint v0, GLuint v1)
  {
    GL::getInstance()->glProgramUniform2ui (program, location, v0, v1);
  }

  static inline void glProgramUniform3ui (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)
  {
    GL::getInstance()->glProgramUniform3ui (program, location, v0, v1, v2);
  }

  static inline void glProgramUniform4ui (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
  {
    GL::getInstance()->glProgramUniform4ui (program, location, v0, v1, v2, v3);
  }

  static inline void glProgramUniform1f (GLuint program, GLint location, GLfloat v0)
  {
    GL::getInstance()->glProgramUniform1f (program, location, v0);
  }

  static inline void glProgramUniform2f (GLuint program, GLint location, GLfloat v0, GLfloat v1)
  {
    GL::getInstance()->glProgramUniform2f (program, location, v0, v1);
  }

  static inline void glProgramUniform3f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
  {
    GL::getInstance()->glProgramUniform3f (program, location, v0, v1, v2);
  }

  static inline void glProgramUniform4f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
  {
    GL::getInstance()->glProgramUniform4f (program, location, v0, v1, v2, v3);
  }

  static inline void glProgramUniform1iv (GLuint program, GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glProgramUniform1iv (program, location, count, value);
  }

  static inline void glProgramUniform2iv (GLuint program, GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glProgramUniform2iv (program, location, count, value);
  }

  static inline void glProgramUniform3iv (GLuint program, GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glProgramUniform3iv (program, location, count, value);
  }

  static inline void glProgramUniform4iv (GLuint program, GLint location, GLsizei count, const GLint *value)
  {
    GL::getInstance()->glProgramUniform4iv (program, location, count, value);
  }

  static inline void glProgramUniform1uiv (GLuint program, GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glProgramUniform1uiv (program, location, count, value);
  }

  static inline void glProgramUniform2uiv (GLuint program, GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glProgramUniform2uiv (program, location, count, value);
  }

  static inline void glProgramUniform3uiv (GLuint program, GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glProgramUniform3uiv (program, location, count, value);
  }

  static inline void glProgramUniform4uiv (GLuint program, GLint location, GLsizei count, const GLuint *value)
  {
    GL::getInstance()->glProgramUniform4uiv (program, location, count, value);
  }

  static inline void glProgramUniform1fv (GLuint program, GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniform1fv (program, location, count, value);
  }

  static inline void glProgramUniform2fv (GLuint program, GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniform2fv (program, location, count, value);
  }

  static inline void glProgramUniform3fv (GLuint program, GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniform3fv (program, location, count, value);
  }

  static inline void glProgramUniform4fv (GLuint program, GLint location, GLsizei count, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniform4fv (program, location, count, value);
  }

  static inline void glProgramUniformMatrix2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix2fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix3fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix4fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix2x3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix2x3fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix3x2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix3x2fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix2x4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix2x4fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix4x2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix4x2fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix3x4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix3x4fv (program, location, count, transpose, value);
  }

  static inline void glProgramUniformMatrix4x3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
  {
    GL::getInstance()->glProgramUniformMatrix4x3fv (program, location, count, transpose, value);
  }

  static inline void glValidateProgramPipeline (GLuint pipeline)
  {
    GL::getInstance()->glValidateProgramPipeline (pipeline);
  }

  static inline void glGetProgramPipelineInfoLog (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
  {
    GL::getInstance()->glGetProgramPipelineInfoLog (pipeline, bufSize, length, infoLog);
  }

  static inline void glBindImageTexture (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
  {
    GL::getInstance()->glBindImageTexture (unit, texture, level, layered, layer, access, format);
  }

  static inline void glGetBooleani_v (GLenum target, GLuint index, GLboolean *data)
  {
    GL::getInstance()->glGetBooleani_v (target, index, data);
  }

  static inline void glMemoryBarrier (GLbitfield barriers)
  {
    GL::getInstance()->glMemoryBarrier (barriers);
  }

  static inline void glTexStorage2DMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
  {
    GL::getInstance()->glTexStorage2DMultisample (target, samples, internalformat, width, height, fixedsamplelocations);
  }

  static inline void glGetMultisamplefv (GLenum pname, GLuint index, GLfloat *val)
  {
    GL::getInstance()->glGetMultisamplefv (pname, index, val);
  }

  static inline void glSampleMaski (GLuint maskNumber, GLbitfield mask)
  {
    GL::getInstance()->glSampleMaski (maskNumber, mask);
  }

  static inline void glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params)
  {
    GL::getInstance()->glGetTexLevelParameteriv (target, level, pname, params);
  }

  static inline void glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params)
  {
    GL::getInstance()->glGetTexLevelParameterfv (target, level, pname, params);
  }

  static inline void glBindVertexBuffer (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
  {
    GL::getInstance()->glBindVertexBuffer (bindingindex, buffer, offset, stride);
  }

  static inline void glVertexAttribFormat (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
  {
    GL::getInstance()->glVertexAttribFormat (attribindex, size, type, normalized, relativeoffset);
  }

  static inline void glVertexAttribIFormat (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
  {
    GL::getInstance()->glVertexAttribIFormat (attribindex, size, type, relativeoffset);
  }

  static inline void glVertexAttribBinding (GLuint attribindex, GLuint bindingindex)
  {
    GL::getInstance()->glVertexAttribBinding (attribindex, bindingindex);
  }

  static inline void glVertexBindingDivisor (GLuint bindingindex, GLuint divisor)
  {
    GL::getInstance()->glVertexBindingDivisor (bindingindex, divisor);
  }

#if !defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)

  static inline void glGetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint *params)
  {
      GL::getInstance()->glGetUniformSubroutineuiv(shadertype, location, params);
  }

  static inline void glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint *indices)
  {
      GL::getInstance()->glUniformSubroutinesuiv(shadertype, count, indices);
  }

  static inline void glGetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name)
  {
      GL::getInstance()->glGetActiveSubroutineName(program, shadertype, index, bufsize, length, name);
  }

  static inline void glGetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name)
  {
      GL::getInstance()->glGetActiveSubroutineUniformName(program, shadertype, index, bufsize, length, name);
  }

  static inline void glGetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values)
  {
      GL::getInstance()->glGetActiveSubroutineUniformiv(program, shadertype, index, pname, values);
  }

  static inline GLuint glGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar *name)
  {
      return GL::getInstance()->glGetSubroutineIndex(program, shadertype, name);
  }

  static inline GLint glGetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar *name)
  {
      return GL::getInstance()->glGetSubroutineUniformLocation(program, shadertype, name);
  }

#endif

};

#endif // OPENGLFUNCTIONS_H

