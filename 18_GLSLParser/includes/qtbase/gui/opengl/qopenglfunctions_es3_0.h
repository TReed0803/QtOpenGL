/****************************************************************************
**
** Copyright (C) 2013 Klaralvdalens Datakonsult AB (KDAB)
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QOPENGLVERSIONFUNCTIONS_ES3_H
#define QOPENGLVERSIONFUNCTIONS_ES3_H

#include <QtCore/qglobal.h>

#if defined(QT_OPENGL_ES_3) || defined(Q_QDOC)

#include <QtGui/QOpenGLVersionFunctions>
#include <QtGui/qopenglcontext.h>

QT_BEGIN_NAMESPACE

class QOpenGLFunctions_ES3_0Private;

class Q_GUI_EXPORT QOpenGLFunctions_ES3_0// : public QAbstractOpenGLFunctions
{
public:
    QOpenGLFunctions_ES3_0();
    ~QOpenGLFunctions_ES3_0();

    bool initializeOpenGLFunctions();// Q_DECL_OVERRIDE;

    // OpenGL ES2 core functions
    void glActiveTexture(GLenum texture);
    void glAttachShader(GLuint program, GLuint shader);
    void glBindAttribLocation(GLuint program, GLuint index, const GLchar* name);
    void glBindBuffer(GLenum target, GLuint buffer);
    void glBindFramebuffer(GLenum target, GLuint framebuffer);
    void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
    void glBindTexture(GLenum target, GLuint texture);
    void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void glBlendEquation(GLenum mode);
    void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
    void glBlendFunc(GLenum sfactor, GLenum dfactor);
    void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
    void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
    GLenum glCheckFramebufferStatus(GLenum target);
    void glClear(GLbitfield mask);
    void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void glClearDepthf(GLclampf depth);
    void glClearStencil(GLint s);
    void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    void glCompileShader(GLuint shader);
    void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data);
    void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data);
    void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    GLuint glCreateProgram(void);
    GLuint glCreateShader(GLenum type);
    void glCullFace(GLenum mode);
    void glDeleteBuffers(GLsizei n, const GLuint* buffers);
    void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
    void glDeleteProgram(GLuint program);
    void glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
    void glDeleteShader(GLuint shader);
    void glDeleteTextures(GLsizei n, const GLuint* textures);
    void glDepthFunc(GLenum func);
    void glDepthMask(GLboolean flag);
    void glDepthRangef(GLclampf zNear, GLclampf zFar);
    void glDetachShader(GLuint program, GLuint shader);
    void glDisable(GLenum cap);
    void glDisableVertexAttribArray(GLuint index);
    void glDrawArrays(GLenum mode, GLint first, GLsizei count);
    void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
    void glEnable(GLenum cap);
    void glEnableVertexAttribArray(GLuint index);
    void glFinish(void);
    void glFlush(void);
    void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    void glFrontFace(GLenum mode);
    void glGenBuffers(GLsizei n, GLuint* buffers);
    void glGenerateMipmap(GLenum target);
    void glGenFramebuffers(GLsizei n, GLuint* framebuffers);
    void glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
    void glGenTextures(GLsizei n, GLuint* textures);
    void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
    void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
    void glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
    int glGetAttribLocation(GLuint program, const GLchar* name);
    void glGetBooleanv(GLenum pname, GLboolean* params);
    void glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
    GLenum glGetError(void);
    void glGetFloatv(GLenum pname, GLfloat* params);
    void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
    void glGetIntegerv(GLenum pname, GLint* params);
    void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
    void glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);
    void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
    void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
    void glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog);
    void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
    void glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source);
    const GLubyte* glGetString(GLenum name);
    void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
    void glGetTexParameteriv(GLenum target, GLenum pname, GLint* params);
    void glGetUniformfv(GLuint program, GLint location, GLfloat* params);
    void glGetUniformiv(GLuint program, GLint location, GLint* params);
    int glGetUniformLocation(GLuint program, const GLchar* name);
    void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params);
    void glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
    void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid** pointer);
    void glHint(GLenum target, GLenum mode);
    GLboolean glIsBuffer(GLuint buffer);
    GLboolean glIsEnabled(GLenum cap);
    GLboolean glIsFramebuffer(GLuint framebuffer);
    GLboolean glIsProgram(GLuint program);
    GLboolean glIsRenderbuffer(GLuint renderbuffer);
    GLboolean glIsShader(GLuint shader);
    GLboolean glIsTexture(GLuint texture);
    void glLineWidth(GLfloat width);
    void glLinkProgram(GLuint program);
    void glPixelStorei(GLenum pname, GLint param);
    void glPolygonOffset(GLfloat factor, GLfloat units);
    void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
    void glReleaseShaderCompiler(void);
    void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    void glSampleCoverage(GLclampf value, GLboolean invert);
    void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length);
    void glShaderSource(GLuint shader, GLsizei count, const GLchar* *string, const GLint* length);
    void glStencilFunc(GLenum func, GLint ref, GLuint mask);
    void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
    void glStencilMask(GLuint mask);
    void glStencilMaskSeparate(GLenum face, GLuint mask);
    void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
    void glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
    void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
    void glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
    void glTexParameteri(GLenum target, GLenum pname, GLint param);
    void glTexParameteriv(GLenum target, GLenum pname, const GLint* params);
    void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
    void glUniform1f(GLint location, GLfloat x);
    void glUniform1fv(GLint location, GLsizei count, const GLfloat* v);
    void glUniform1i(GLint location, GLint x);
    void glUniform1iv(GLint location, GLsizei count, const GLint* v);
    void glUniform2f(GLint location, GLfloat x, GLfloat y);
    void glUniform2fv(GLint location, GLsizei count, const GLfloat* v);
    void glUniform2i(GLint location, GLint x, GLint y);
    void glUniform2iv(GLint location, GLsizei count, const GLint* v);
    void glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z);
    void glUniform3fv(GLint location, GLsizei count, const GLfloat* v);
    void glUniform3i(GLint location, GLint x, GLint y, GLint z);
    void glUniform3iv(GLint location, GLsizei count, const GLint* v);
    void glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void glUniform4fv(GLint location, GLsizei count, const GLfloat* v);
    void glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w);
    void glUniform4iv(GLint location, GLsizei count, const GLint* v);
    void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void glUseProgram(GLuint program);
    void glValidateProgram(GLuint program);
    void glVertexAttrib1f(GLuint indx, GLfloat x);
    void glVertexAttrib1fv(GLuint indx, const GLfloat* values);
    void glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y);
    void glVertexAttrib2fv(GLuint indx, const GLfloat* values);
    void glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z);
    void glVertexAttrib3fv(GLuint indx, const GLfloat* values);
    void glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void glVertexAttrib4fv(GLuint indx, const GLfloat* values);
    void glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr);
    void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

    // OpenGL ES3 core functions
    void glReadBuffer(GLenum src);
    void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
    void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
    void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
    void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
    void glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
    void glGenQueries(GLsizei n, GLuint *ids);
    void glDeleteQueries(GLsizei n, const GLuint *ids);
    GLboolean glIsQuery(GLuint id);
    void glBeginQuery(GLenum target, GLuint id);
    void glEndQuery(GLenum target);
    void glGetQueryiv(GLenum target, GLenum pname, GLint *params);
    void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params);
    GLboolean glUnmapBuffer(GLenum target);
    void glGetBufferPointerv(GLenum target, GLenum pname, void **params);
    void glDrawBuffers(GLsizei n, const GLenum *bufs);
    void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
    void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
    void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
    void  glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    void glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
    void glBindVertexArray(GLuint array);
    void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
    void glGenVertexArrays(GLsizei n, GLuint *arrays);
    GLboolean glIsVertexArray(GLuint array);
    void glGetIntegeri_v(GLenum target, GLuint index, GLint *data);
    void glBeginTransformFeedback(GLenum primitiveMode);
    void glEndTransformFeedback(void);
    void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
    void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
    void glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
    void glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
    void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
    void glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params);
    void glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params);
    void glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w);
    void glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
    void glVertexAttribI4iv(GLuint index, const GLint *v);
    void glVertexAttribI4uiv(GLuint index, const GLuint *v);
    void glGetUniformuiv(GLuint program, GLint location, GLuint *params);
    GLint glGetFragDataLocation(GLuint program, const GLchar *name);
    void glUniform1ui(GLint location, GLuint v0);
    void glUniform2ui(GLint location, GLuint v0, GLuint v1);
    void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
    void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    void glUniform1uiv(GLint location, GLsizei count, const GLuint *value);
    void glUniform2uiv(GLint location, GLsizei count, const GLuint *value);
    void glUniform3uiv(GLint location, GLsizei count, const GLuint *value);
    void glUniform4uiv(GLint location, GLsizei count, const GLuint *value);
    void glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value);
    void glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value);
    void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value);
    void glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
    const GLubyte *glGetStringi(GLenum name, GLuint index);
    void glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
    void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
    void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
    GLuint glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName);
    void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
    void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
    void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
    void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
    void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
    GLsync glFenceSync(GLenum condition, GLbitfield flags);
    GLboolean glIsSync(GLsync sync);
    void glDeleteSync(GLsync sync);
    GLenum glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
    void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
    void glGetInteger64v(GLenum pname, GLint64 *data);
    void glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
    void glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data);
    void glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params);
    void glGenSamplers(GLsizei count, GLuint *samplers);
    void glDeleteSamplers(GLsizei count, const GLuint *samplers);
    GLboolean glIsSampler(GLuint sampler);
    void glBindSampler(GLuint unit, GLuint sampler);
    void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
    void glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param);
    void glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
    void glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *param);
    void glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params);
    void glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params);
    void glVertexAttribDivisor(GLuint index, GLuint divisor);
    void glBindTransformFeedback(GLenum target, GLuint id);
    void glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids);
    void glGenTransformFeedbacks(GLsizei n, GLuint *ids);
    GLboolean glIsTransformFeedback(GLuint id);
    void glPauseTransformFeedback(void);
    void glResumeTransformFeedback(void);
    void glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
    void glProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
    void glProgramParameteri(GLuint program, GLenum pname, GLint value);
    void glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments);
    void glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
    void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    void glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
    void glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);

private:
    friend class QOpenGLContext;

    //static bool isContextCompatible(QOpenGLContext *context);
    //static QOpenGLVersionProfile versionProfile();

    // For future expansion - not used
    QOpenGLFunctions_ES3_0Private* d_es3;
};

// OpenGL ES2 core functions
inline void QOpenGLFunctions_ES3_0::glActiveTexture(GLenum texture)
{
    ::glActiveTexture(texture);
}

inline void QOpenGLFunctions_ES3_0::glAttachShader(GLuint program, GLuint shader)
{
    ::glAttachShader(program, shader);
}

inline void QOpenGLFunctions_ES3_0::glBindAttribLocation(GLuint program, GLuint index, const GLchar* name)
{
    ::glBindAttribLocation(program, index, name);
}

inline void QOpenGLFunctions_ES3_0::glBindBuffer(GLenum target, GLuint buffer)
{
    ::glBindBuffer(target, buffer);
}

inline void QOpenGLFunctions_ES3_0::glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    ::glBindFramebuffer(target, framebuffer);
}

inline void QOpenGLFunctions_ES3_0::glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    ::glBindRenderbuffer(target, renderbuffer);
}

inline void QOpenGLFunctions_ES3_0::glBindTexture(GLenum target, GLuint texture)
{
    ::glBindTexture(target, texture);
}

inline void QOpenGLFunctions_ES3_0::glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    ::glBlendColor(red, green, blue, alpha);
}

inline void QOpenGLFunctions_ES3_0::glBlendEquation(GLenum mode)
{
    ::glBlendEquation(mode);
}

inline void QOpenGLFunctions_ES3_0::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    ::glBlendEquationSeparate(modeRGB, modeAlpha);
}

inline void QOpenGLFunctions_ES3_0::glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    ::glBlendFunc(sfactor, dfactor);
}

inline void QOpenGLFunctions_ES3_0::glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    ::glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

inline void QOpenGLFunctions_ES3_0::glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    ::glBufferData(target, size, data, usage);
}

inline void QOpenGLFunctions_ES3_0::glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
    ::glBufferSubData(target, offset, size, data);
}

inline GLenum QOpenGLFunctions_ES3_0::glCheckFramebufferStatus(GLenum target)
{
    return ::glCheckFramebufferStatus(target);
}

inline void QOpenGLFunctions_ES3_0::glClear(GLbitfield mask)
{
    ::glClear(mask);
}

inline void QOpenGLFunctions_ES3_0::glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    ::glClearColor(red, green, blue, alpha);
}

inline void QOpenGLFunctions_ES3_0::glClearDepthf(GLclampf depth)
{
    ::glClearDepthf(depth);
}

inline void QOpenGLFunctions_ES3_0::glClearStencil(GLint s)
{
    ::glClearStencil(s);
}

inline void QOpenGLFunctions_ES3_0::glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    ::glColorMask(red, green, blue, alpha);
}

inline void QOpenGLFunctions_ES3_0::glCompileShader(GLuint shader)
{
    ::glCompileShader(shader);
}

inline void QOpenGLFunctions_ES3_0::glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data)
{
    ::glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

inline void QOpenGLFunctions_ES3_0::glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data)
{
    ::glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

inline void QOpenGLFunctions_ES3_0::glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    ::glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

inline void QOpenGLFunctions_ES3_0::glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

inline GLuint QOpenGLFunctions_ES3_0::glCreateProgram(void)
{
    return ::glCreateProgram();
}

inline GLuint QOpenGLFunctions_ES3_0::glCreateShader(GLenum type)
{
    return ::glCreateShader(type);
}

inline void QOpenGLFunctions_ES3_0::glCullFace(GLenum mode)
{
    ::glCullFace(mode);
}

inline void QOpenGLFunctions_ES3_0::glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
    ::glDeleteBuffers(n, buffers);
}

inline void QOpenGLFunctions_ES3_0::glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
    ::glDeleteFramebuffers(n, framebuffers);
}

inline void QOpenGLFunctions_ES3_0::glDeleteProgram(GLuint program)
{
    ::glDeleteProgram(program);
}

inline void QOpenGLFunctions_ES3_0::glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
    ::glDeleteRenderbuffers(n, renderbuffers);
}

inline void QOpenGLFunctions_ES3_0::glDeleteShader(GLuint shader)
{
    ::glDeleteShader(shader);
}

inline void QOpenGLFunctions_ES3_0::glDeleteTextures(GLsizei n, const GLuint* textures)
{
    ::glDeleteTextures(n, textures);
}

inline void QOpenGLFunctions_ES3_0::glDepthFunc(GLenum func)
{
    ::glDepthFunc(func);
}

inline void QOpenGLFunctions_ES3_0::glDepthMask(GLboolean flag)
{
    ::glDepthMask(flag);
}

inline void QOpenGLFunctions_ES3_0::glDepthRangef(GLclampf zNear, GLclampf zFar)
{
    ::glDepthRangef(zNear, zFar);
}

inline void QOpenGLFunctions_ES3_0::glDetachShader(GLuint program, GLuint shader)
{
    ::glDetachShader(program, shader);
}

inline void QOpenGLFunctions_ES3_0::glDisable(GLenum cap)
{
    ::glDisable(cap);
}

inline void QOpenGLFunctions_ES3_0::glDisableVertexAttribArray(GLuint index)
{
    ::glDisableVertexAttribArray(index);
}

inline void QOpenGLFunctions_ES3_0::glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    ::glDrawArrays(mode, first, count);
}

inline void QOpenGLFunctions_ES3_0::glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
    ::glDrawElements(mode, count, type, indices);
}

inline void QOpenGLFunctions_ES3_0::glEnable(GLenum cap)
{
    ::glEnable(cap);
}

inline void QOpenGLFunctions_ES3_0::glEnableVertexAttribArray(GLuint index)
{
    ::glEnableVertexAttribArray(index);
}

inline void QOpenGLFunctions_ES3_0::glFinish(void)
{
    ::glFinish();
}

inline void QOpenGLFunctions_ES3_0::glFlush(void)
{
    ::glFlush();
}

inline void QOpenGLFunctions_ES3_0::glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    ::glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

inline void QOpenGLFunctions_ES3_0::glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    ::glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

inline void QOpenGLFunctions_ES3_0::glFrontFace(GLenum mode)
{
    ::glFrontFace(mode);
}

inline void QOpenGLFunctions_ES3_0::glGenBuffers(GLsizei n, GLuint* buffers)
{
    ::glGenBuffers(n, buffers);
}

inline void QOpenGLFunctions_ES3_0::glGenerateMipmap(GLenum target)
{
    ::glGenerateMipmap(target);
}

inline void QOpenGLFunctions_ES3_0::glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
    ::glGenFramebuffers(n, framebuffers);
}

inline void QOpenGLFunctions_ES3_0::glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
    ::glGenRenderbuffers(n, renderbuffers);
}

inline void QOpenGLFunctions_ES3_0::glGenTextures(GLsizei n, GLuint* textures)
{
    ::glGenTextures(n, textures);
}

inline void QOpenGLFunctions_ES3_0::glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
    ::glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

inline void QOpenGLFunctions_ES3_0::glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
    ::glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

inline void QOpenGLFunctions_ES3_0::glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
    ::glGetAttachedShaders(program, maxcount, count, shaders);
}

inline int QOpenGLFunctions_ES3_0::glGetAttribLocation(GLuint program, const GLchar* name)
{
    return ::glGetAttribLocation(program, name);
}

inline void QOpenGLFunctions_ES3_0::glGetBooleanv(GLenum pname, GLboolean* params)
{
    ::glGetBooleanv(pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    ::glGetBufferParameteriv(target, pname, params);
}

inline GLenum QOpenGLFunctions_ES3_0::glGetError(void)
{
    return ::glGetError();
}

inline void QOpenGLFunctions_ES3_0::glGetFloatv(GLenum pname, GLfloat* params)
{
    ::glGetFloatv(pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
    ::glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetIntegerv(GLenum pname, GLint* params)
{
    ::glGetIntegerv(pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    ::glGetProgramiv(program, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog)
{
    ::glGetProgramInfoLog(program, bufsize, length, infolog);
}

inline void QOpenGLFunctions_ES3_0::glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    ::glGetRenderbufferParameteriv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    ::glGetShaderiv(shader, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog)
{
    ::glGetShaderInfoLog(shader, bufsize, length, infolog);
}

inline void QOpenGLFunctions_ES3_0::glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
    ::glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

inline void QOpenGLFunctions_ES3_0::glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source)
{
    ::glGetShaderSource(shader, bufsize, length, source);
}

inline const GLubyte* QOpenGLFunctions_ES3_0::glGetString(GLenum name)
{
    return ::glGetString(name);
}

inline void QOpenGLFunctions_ES3_0::glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
    ::glGetTexParameterfv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
    ::glGetTexParameteriv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
    ::glGetUniformfv(program, location, params);
}

inline void QOpenGLFunctions_ES3_0::glGetUniformiv(GLuint program, GLint location, GLint* params)
{
    ::glGetUniformiv(program, location, params);
}

inline int QOpenGLFunctions_ES3_0::glGetUniformLocation(GLuint program, const GLchar* name)
{
    return ::glGetUniformLocation(program, name);
}

inline void QOpenGLFunctions_ES3_0::glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
    ::glGetVertexAttribfv(index, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
    ::glGetVertexAttribiv(index, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid** pointer)
{
    ::glGetVertexAttribPointerv(index, pname, pointer);
}

inline void QOpenGLFunctions_ES3_0::glHint(GLenum target, GLenum mode)
{
    ::glHint(target, mode);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsBuffer(GLuint buffer)
{
    return ::glIsBuffer(buffer);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsEnabled(GLenum cap)
{
    return ::glIsEnabled(cap);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsFramebuffer(GLuint framebuffer)
{
    return ::glIsFramebuffer(framebuffer);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsProgram(GLuint program)
{
    return ::glIsProgram(program);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsRenderbuffer(GLuint renderbuffer)
{
    return ::glIsRenderbuffer(renderbuffer);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsShader(GLuint shader)
{
    return ::glIsShader(shader);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsTexture(GLuint texture)
{
    return ::glIsTexture(texture);
}

inline void QOpenGLFunctions_ES3_0::glLineWidth(GLfloat width)
{
    ::glLineWidth(width);
}

inline void QOpenGLFunctions_ES3_0::glLinkProgram(GLuint program)
{
    ::glLinkProgram(program);
}

inline void QOpenGLFunctions_ES3_0::glPixelStorei(GLenum pname, GLint param)
{
    ::glPixelStorei(pname, param);
}

inline void QOpenGLFunctions_ES3_0::glPolygonOffset(GLfloat factor, GLfloat units)
{
    ::glPolygonOffset(factor, units);
}

inline void QOpenGLFunctions_ES3_0::glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
    ::glReadPixels(x, y, width, height, format, type, pixels);
}

inline void QOpenGLFunctions_ES3_0::glReleaseShaderCompiler(void)
{
    ::glReleaseShaderCompiler();
}

inline void QOpenGLFunctions_ES3_0::glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    ::glRenderbufferStorage(target, internalformat, width, height);
}

inline void QOpenGLFunctions_ES3_0::glSampleCoverage(GLclampf value, GLboolean invert)
{
    ::glSampleCoverage(value, invert);
}

inline void QOpenGLFunctions_ES3_0::glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    ::glScissor(x, y, width, height);
}

inline void QOpenGLFunctions_ES3_0::glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length)
{
    ::glShaderBinary(n, shaders, binaryformat, binary, length);
}

inline void QOpenGLFunctions_ES3_0::glShaderSource(GLuint shader, GLsizei count, const GLchar* *string, const GLint* length)
{
    ::glShaderSource(shader, count, string, length);
}

inline void QOpenGLFunctions_ES3_0::glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    ::glStencilFunc(func, ref, mask);
}

inline void QOpenGLFunctions_ES3_0::glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    ::glStencilFuncSeparate(face, func, ref, mask);
}

inline void QOpenGLFunctions_ES3_0::glStencilMask(GLuint mask)
{
    ::glStencilMask(mask);
}

inline void QOpenGLFunctions_ES3_0::glStencilMaskSeparate(GLenum face, GLuint mask)
{
    ::glStencilMaskSeparate(face, mask);
}

inline void QOpenGLFunctions_ES3_0::glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    ::glStencilOp(fail, zfail, zpass);
}

inline void QOpenGLFunctions_ES3_0::glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
    ::glStencilOpSeparate(face, fail, zfail, zpass);
}

inline void QOpenGLFunctions_ES3_0::glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
    ::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

inline void QOpenGLFunctions_ES3_0::glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    ::glTexParameterf(target, pname, param);
}

inline void QOpenGLFunctions_ES3_0::glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
    ::glTexParameterfv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    ::glTexParameteri(target, pname, param);
}

inline void QOpenGLFunctions_ES3_0::glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
    ::glTexParameteriv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
    ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

inline void QOpenGLFunctions_ES3_0::glUniform1f(GLint location, GLfloat x)
{
    ::glUniform1f(location, x);
}

inline void QOpenGLFunctions_ES3_0::glUniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
    ::glUniform1fv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform1i(GLint location, GLint x)
{
    ::glUniform1i(location, x);
}

inline void QOpenGLFunctions_ES3_0::glUniform1iv(GLint location, GLsizei count, const GLint* v)
{
    ::glUniform1iv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform2f(GLint location, GLfloat x, GLfloat y)
{
    ::glUniform2f(location, x, y);
}

inline void QOpenGLFunctions_ES3_0::glUniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
    ::glUniform2fv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform2i(GLint location, GLint x, GLint y)
{
    ::glUniform2i(location, x, y);
}

inline void QOpenGLFunctions_ES3_0::glUniform2iv(GLint location, GLsizei count, const GLint* v)
{
    ::glUniform2iv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
    ::glUniform3f(location, x, y, z);
}

inline void QOpenGLFunctions_ES3_0::glUniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
    ::glUniform3fv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
    ::glUniform3i(location, x, y, z);
}

inline void QOpenGLFunctions_ES3_0::glUniform3iv(GLint location, GLsizei count, const GLint* v)
{
    ::glUniform3iv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    ::glUniform4f(location, x, y, z, w);
}

inline void QOpenGLFunctions_ES3_0::glUniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
    ::glUniform4fv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
    ::glUniform4i(location, x, y, z, w);
}

inline void QOpenGLFunctions_ES3_0::glUniform4iv(GLint location, GLsizei count, const GLint* v)
{
    ::glUniform4iv(location, count, v);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    ::glUniformMatrix2fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    ::glUniformMatrix3fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    ::glUniformMatrix4fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUseProgram(GLuint program)
{
    ::glUseProgram(program);
}

inline void QOpenGLFunctions_ES3_0::glValidateProgram(GLuint program)
{
    ::glValidateProgram(program);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib1f(GLuint indx, GLfloat x)
{
    ::glVertexAttrib1f(indx, x);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib1fv(GLuint indx, const GLfloat* values)
{
    ::glVertexAttrib1fv(indx, values);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
    ::glVertexAttrib2f(indx, x, y);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib2fv(GLuint indx, const GLfloat* values)
{
    ::glVertexAttrib2fv(indx, values);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
    ::glVertexAttrib3f(indx, x, y, z);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib3fv(GLuint indx, const GLfloat* values)
{
    ::glVertexAttrib3fv(indx, values);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    ::glVertexAttrib4f(indx, x, y, z, w);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttrib4fv(GLuint indx, const GLfloat* values)
{
    ::glVertexAttrib4fv(indx, values);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
{
    ::glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

inline void QOpenGLFunctions_ES3_0::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    ::glViewport(x, y, width, height);
}

// OpenGL ES3 core functions
inline void QOpenGLFunctions_ES3_0::glReadBuffer(GLenum src)
{
    ::glReadBuffer(src);
}

inline void QOpenGLFunctions_ES3_0::glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices)
{
    ::glDrawRangeElements(mode, start, end, count, type, indices);
}

inline void QOpenGLFunctions_ES3_0::glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)
{
    ::glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
}

inline void QOpenGLFunctions_ES3_0::glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    ::glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

inline void QOpenGLFunctions_ES3_0::glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ::glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
}

inline void QOpenGLFunctions_ES3_0::glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data)
{
    ::glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
}

inline void QOpenGLFunctions_ES3_0::glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)
{
    ::glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
}

inline void QOpenGLFunctions_ES3_0::glGenQueries(GLsizei n, GLuint *ids)
{
    ::glGenQueries(n, ids);
}

inline void QOpenGLFunctions_ES3_0::glDeleteQueries(GLsizei n, const GLuint *ids)
{
    ::glDeleteQueries(n, ids);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsQuery(GLuint id)
{
    return ::glIsQuery(id);
}

inline void QOpenGLFunctions_ES3_0::glBeginQuery(GLenum target, GLuint id)
{
    ::glBeginQuery(target, id);
}

inline void QOpenGLFunctions_ES3_0::glEndQuery(GLenum target)
{
    ::glEndQuery(target);
}

inline void QOpenGLFunctions_ES3_0::glGetQueryiv(GLenum target, GLenum pname, GLint *params)
{
    ::glGetQueryiv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
{
    ::glGetQueryObjectuiv(id, pname, params);
}

inline GLboolean QOpenGLFunctions_ES3_0::glUnmapBuffer(GLenum target)
{
    return ::glUnmapBuffer(target);
}

inline void QOpenGLFunctions_ES3_0::glGetBufferPointerv(GLenum target, GLenum pname, void **params)
{
    ::glGetBufferPointerv(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glDrawBuffers(GLsizei n, const GLenum *bufs)
{
    ::glDrawBuffers(n, bufs);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ::glUniformMatrix2x3fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ::glUniformMatrix3x2fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ::glUniformMatrix2x4fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ::glUniformMatrix4x2fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ::glUniformMatrix3x4fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ::glUniformMatrix4x3fv(location, count, transpose, value);
}

inline void QOpenGLFunctions_ES3_0::glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
    ::glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

inline void QOpenGLFunctions_ES3_0::glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
    ::glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}

inline void QOpenGLFunctions_ES3_0::glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
    ::glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

inline void QOpenGLFunctions_ES3_0::glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    ::glMapBufferRange(target, offset, length, access);
}

inline void QOpenGLFunctions_ES3_0::glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
    ::glFlushMappedBufferRange(target, offset, length);
}

inline void QOpenGLFunctions_ES3_0::glBindVertexArray(GLuint array)
{
    ::glBindVertexArray(array);
}

inline void QOpenGLFunctions_ES3_0::glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    ::glDeleteVertexArrays(n, arrays);
}

inline void QOpenGLFunctions_ES3_0::glGenVertexArrays(GLsizei n, GLuint *arrays)
{
    ::glGenVertexArrays(n, arrays);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsVertexArray(GLuint array)
{
    return ::glIsVertexArray(array);
}

inline void QOpenGLFunctions_ES3_0::glGetIntegeri_v(GLenum target, GLuint index, GLint *data)
{
    ::glGetIntegeri_v(target, index, data);
}

inline void QOpenGLFunctions_ES3_0::glBeginTransformFeedback(GLenum primitiveMode)
{
    ::glBeginTransformFeedback(primitiveMode);
}

inline void QOpenGLFunctions_ES3_0::glEndTransformFeedback(void)
{
    ::glEndTransformFeedback();
}

inline void QOpenGLFunctions_ES3_0::glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    ::glBindBufferRange(target, index, buffer, offset, size);
}

inline void QOpenGLFunctions_ES3_0::glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
    ::glBindBufferBase(target, index, buffer);
}

inline void QOpenGLFunctions_ES3_0::glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode)
{
    ::glTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

inline void QOpenGLFunctions_ES3_0::glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
{
    ::glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    ::glVertexAttribIPointer(index, size, type, stride, pointer);
}

inline void QOpenGLFunctions_ES3_0::glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params)
{
    ::glGetVertexAttribIiv(index, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params)
{
    ::glGetVertexAttribIuiv(index, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
    ::glVertexAttribI4i(index, x, y, z, w);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
{
    ::glVertexAttribI4ui(index, x, y, z, w);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribI4iv(GLuint index, const GLint *v)
{
    ::glVertexAttribI4iv(index, v);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribI4uiv(GLuint index, const GLuint *v)
{
    ::glVertexAttribI4uiv(index, v);
}

inline void QOpenGLFunctions_ES3_0::glGetUniformuiv(GLuint program, GLint location, GLuint *params)
{
    ::glGetUniformuiv(program, location, params);
}

inline GLint QOpenGLFunctions_ES3_0::glGetFragDataLocation(GLuint program, const GLchar *name)
{
    return ::glGetFragDataLocation(program, name);
}

inline void QOpenGLFunctions_ES3_0::glUniform1ui(GLint location, GLuint v0)
{
    ::glUniform1ui(location, v0);
}

inline void QOpenGLFunctions_ES3_0::glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
    ::glUniform2ui(location, v0, v1);
}

inline void QOpenGLFunctions_ES3_0::glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
    ::glUniform3ui(location, v0, v1, v2);
}

inline void QOpenGLFunctions_ES3_0::glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
    ::glUniform4ui(location, v0, v1, v2, v3);
}

inline void QOpenGLFunctions_ES3_0::glUniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
    ::glUniform1uiv(location, count, value);
}

inline void QOpenGLFunctions_ES3_0::glUniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
    ::glUniform2uiv(location, count, value);
}

inline void QOpenGLFunctions_ES3_0::glUniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
    ::glUniform3uiv(location, count, value);
}

inline void QOpenGLFunctions_ES3_0::glUniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
    ::glUniform4uiv(location, count, value);
}

inline void QOpenGLFunctions_ES3_0::glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value)
{
    ::glClearBufferiv(buffer, drawbuffer, value);
}

inline void QOpenGLFunctions_ES3_0::glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value)
{
    ::glClearBufferuiv(buffer, drawbuffer, value);
}

inline void QOpenGLFunctions_ES3_0::glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value)
{
    ::glClearBufferfv(buffer, drawbuffer, value);
}

inline void QOpenGLFunctions_ES3_0::glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
    ::glClearBufferfi(buffer, drawbuffer, depth, stencil);
}

inline const GLubyte *QOpenGLFunctions_ES3_0::glGetStringi(GLenum name, GLuint index)
{
    return ::glGetStringi(name, index);
}

inline void QOpenGLFunctions_ES3_0::glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    ::glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

inline void QOpenGLFunctions_ES3_0::glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)
{
    ::glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

inline void QOpenGLFunctions_ES3_0::glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
{
    ::glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

inline GLuint QOpenGLFunctions_ES3_0::glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
{
    return ::glGetUniformBlockIndex(program, uniformBlockName);
}

inline void QOpenGLFunctions_ES3_0::glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
{
    ::glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{
    ::glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
}

inline void QOpenGLFunctions_ES3_0::glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
    ::glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

inline void QOpenGLFunctions_ES3_0::glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
    ::glDrawArraysInstanced(mode, first, count, instancecount);
}

inline void QOpenGLFunctions_ES3_0::glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
{
    ::glDrawElementsInstanced(mode, count, type, indices, instancecount);
}

inline GLsync QOpenGLFunctions_ES3_0::glFenceSync(GLenum condition, GLbitfield flags)
{
    return ::glFenceSync(condition, flags);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsSync(GLsync sync)
{
    return ::glIsSync(sync);
}

inline void QOpenGLFunctions_ES3_0::glDeleteSync(GLsync sync)
{
    ::glDeleteSync(sync);
}

inline GLenum QOpenGLFunctions_ES3_0::glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
    return ::glClientWaitSync(sync, flags, timeout);
}

inline void QOpenGLFunctions_ES3_0::glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
    ::glWaitSync(sync, flags, timeout);
}

inline void QOpenGLFunctions_ES3_0::glGetInteger64v(GLenum pname, GLint64 *data)
{
    ::glGetInteger64v(pname, data);
}

inline void QOpenGLFunctions_ES3_0::glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values)
{
    ::glGetSynciv(sync, pname, bufSize, length, values);
}

inline void QOpenGLFunctions_ES3_0::glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data)
{
    ::glGetInteger64i_v(target, index, data);
}

inline void QOpenGLFunctions_ES3_0::glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params)
{
    ::glGetBufferParameteri64v(target, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGenSamplers(GLsizei count, GLuint *samplers)
{
    ::glGenSamplers(count, samplers);
}

inline void QOpenGLFunctions_ES3_0::glDeleteSamplers(GLsizei count, const GLuint *samplers)
{
    ::glDeleteSamplers(count, samplers);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsSampler(GLuint sampler)
{
    return ::glIsSampler(sampler);
}

inline void QOpenGLFunctions_ES3_0::glBindSampler(GLuint unit, GLuint sampler)
{
    ::glBindSampler(unit, sampler);
}

inline void QOpenGLFunctions_ES3_0::glSamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
    ::glSamplerParameteri(sampler, pname, param);
}

inline void QOpenGLFunctions_ES3_0::glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param)
{
    ::glSamplerParameteriv(sampler, pname, param);
}

inline void QOpenGLFunctions_ES3_0::glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
    ::glSamplerParameterf(sampler, pname, param);
}

inline void QOpenGLFunctions_ES3_0::glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *param)
{
    ::glSamplerParameterfv(sampler, pname, param);
}

inline void QOpenGLFunctions_ES3_0::glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params)
{
    ::glGetSamplerParameteriv(sampler, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params)
{
    ::glGetSamplerParameterfv(sampler, pname, params);
}

inline void QOpenGLFunctions_ES3_0::glVertexAttribDivisor(GLuint index, GLuint divisor)
{
    ::glVertexAttribDivisor(index, divisor);
}

inline void QOpenGLFunctions_ES3_0::glBindTransformFeedback(GLenum target, GLuint id)
{
    ::glBindTransformFeedback(target, id);
}

inline void QOpenGLFunctions_ES3_0::glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids)
{
    ::glDeleteTransformFeedbacks(n, ids);
}

inline void QOpenGLFunctions_ES3_0::glGenTransformFeedbacks(GLsizei n, GLuint *ids)
{
    ::glGenTransformFeedbacks(n, ids);
}

inline GLboolean QOpenGLFunctions_ES3_0::glIsTransformFeedback(GLuint id)
{
    return ::glIsTransformFeedback(id);
}

inline void QOpenGLFunctions_ES3_0::glPauseTransformFeedback(void)
{
    ::glPauseTransformFeedback();
}

inline void QOpenGLFunctions_ES3_0::glResumeTransformFeedback(void)
{
    ::glResumeTransformFeedback();
}

inline void QOpenGLFunctions_ES3_0::glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    ::glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
}

inline void QOpenGLFunctions_ES3_0::glProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)
{
    ::glProgramBinary(program, binaryFormat, binary, length);
}

inline void QOpenGLFunctions_ES3_0::glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
    ::glProgramParameteri(program, pname, value);
}

inline void QOpenGLFunctions_ES3_0::glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments)
{
    ::glInvalidateFramebuffer(target, numAttachments, attachments);
}

inline void QOpenGLFunctions_ES3_0::glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ::glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
}

inline void QOpenGLFunctions_ES3_0::glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
    ::glTexStorage2D(target, levels, internalformat, width, height);
}

inline void QOpenGLFunctions_ES3_0::glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
    ::glTexStorage3D(target, levels, internalformat, width, height, depth);
}

inline void QOpenGLFunctions_ES3_0::glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params)
{
    ::glGetInternalformativ(target, internalformat, pname, bufSize, params);
}

QT_END_NAMESPACE

#endif // QT_OPENGL_ES_3

#endif
