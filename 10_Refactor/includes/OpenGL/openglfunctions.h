#ifndef OPENGLFUNCTIONS_H
#define OPENGLFUNCTIONS_H

#include <OpenGLCommon>
#include <QOpenGLFunctions>

class OpenGLFunctions : public QOpenGLFunctions
{
public:
  inline OpenGLFunctions() : QOpenGLFunctions() {}
  inline explicit OpenGLFunctions(QOpenGLContext *context) : QOpenGLFunctions(context) {}
  inline ~OpenGLFunctions() {}

  GL_PROFILE(QOpenGLFunctions,glBindTexture,GLenum,GLuint);
  GL_PROFILE(QOpenGLFunctions,glBlendFunc,GLenum,GLenum);
  GL_PROFILE(QOpenGLFunctions,glClear,GLbitfield);
  GL_PROFILE(QOpenGLFunctions,glClearColor,GLclampf,GLclampf,GLclampf,GLclampf);
  GL_PROFILE(QOpenGLFunctions,glClearStencil,GLint);
  GL_PROFILE(QOpenGLFunctions,glColorMask,GLboolean,GLboolean,GLboolean,GLboolean);
  GL_PROFILE(QOpenGLFunctions,glCopyTexImage2D,GLenum,GLint,GLenum,GLint,GLint,GLsizei,GLsizei,GLint);
  GL_PROFILE(QOpenGLFunctions,glCopyTexSubImage2D,GLenum,GLint,GLint,GLint,GLint,GLint,GLsizei,GLsizei);
  GL_PROFILE(QOpenGLFunctions,glCullFace,GLenum);
  GL_PROFILE(QOpenGLFunctions,glDeleteTextures,GLsizei,const GLuint*);
  GL_PROFILE(QOpenGLFunctions,glDepthFunc,GLenum);
  GL_PROFILE(QOpenGLFunctions,glDepthMask,GLboolean);
  GL_PROFILE(QOpenGLFunctions,glDisable,GLenum);
  GL_PROFILE(QOpenGLFunctions,glDrawArrays,GLenum,GLint,GLsizei);
  GL_PROFILE(QOpenGLFunctions,glDrawElements,GLenum,GLsizei,GLenum,const GLvoid*);
  GL_PROFILE(QOpenGLFunctions,glEnable,GLenum);
  GL_PROFILE(QOpenGLFunctions,glFinish);
  GL_PROFILE(QOpenGLFunctions,glFlush);
  GL_PROFILE(QOpenGLFunctions,glFrontFace,GLenum);
  GL_PROFILE(QOpenGLFunctions,glGenTextures,GLsizei,GLuint*);
  GL_PROFILE(QOpenGLFunctions,glGetBooleanv,GLenum,GLboolean*);
  GL_PROFILE(QOpenGLFunctions,glGetError);
  GL_PROFILE(QOpenGLFunctions,glGetFloatv,GLenum,GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glGetIntegerv,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetString,GLenum);
  GL_PROFILE(QOpenGLFunctions,glGetTexParameterfv,GLenum,GLenum,GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glGetTexParameteriv,GLenum,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glHint,GLenum,GLenum);
  GL_PROFILE(QOpenGLFunctions,glIsEnabled,GLenum);
  GL_PROFILE(QOpenGLFunctions,glIsTexture,GLuint);
  GL_PROFILE(QOpenGLFunctions,glLineWidth,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glPixelStorei,GLenum,GLint);
  GL_PROFILE(QOpenGLFunctions,glPolygonOffset,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glReadPixels,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,GLvoid*);
  GL_PROFILE(QOpenGLFunctions,glScissor,GLint,GLint,GLsizei,GLsizei);
  GL_PROFILE(QOpenGLFunctions,glStencilFunc,GLenum,GLint,GLuint);
  GL_PROFILE(QOpenGLFunctions,glStencilMask,GLuint);
  GL_PROFILE(QOpenGLFunctions,glStencilOp,GLenum,GLenum,GLenum);
  GL_PROFILE(QOpenGLFunctions,glTexImage2D,GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*);
  GL_PROFILE(QOpenGLFunctions,glTexParameterf,GLenum,GLenum,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glTexParameterfv,GLenum,GLenum,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glTexParameteri,GLenum,GLenum,GLint);
  GL_PROFILE(QOpenGLFunctions,glTexParameteriv,GLenum,GLenum,const GLint*);
  GL_PROFILE(QOpenGLFunctions,glTexSubImage2D,GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*);
  GL_PROFILE(QOpenGLFunctions,glViewport,GLint,GLint,GLsizei,GLsizei);

  // GL(ES)2
  GL_PROFILE(QOpenGLFunctions,glActiveTexture,GLenum);
  GL_PROFILE(QOpenGLFunctions,glAttachShader,GLuint,GLuint);
  GL_PROFILE(QOpenGLFunctions,glBindAttribLocation,GLuint,GLuint,const char*);
  GL_PROFILE(QOpenGLFunctions,glBindBuffer,GLenum,GLuint);
  GL_PROFILE(QOpenGLFunctions,glBindFramebuffer,GLenum,GLuint);
  GL_PROFILE(QOpenGLFunctions,glBindRenderbuffer,GLenum,GLuint);
  GL_PROFILE(QOpenGLFunctions,glBlendColor,GLclampf,GLclampf,GLclampf,GLclampf);
  GL_PROFILE(QOpenGLFunctions,glBlendEquation,GLenum);
  GL_PROFILE(QOpenGLFunctions,glBlendEquationSeparate,GLenum,GLenum);
  GL_PROFILE(QOpenGLFunctions,glBlendFuncSeparate,GLenum,GLenum,GLenum,GLenum);
  GL_PROFILE(QOpenGLFunctions,glBufferData,GLenum,qopengl_GLsizeiptr,const void*,GLenum);
  GL_PROFILE(QOpenGLFunctions,glBufferSubData,GLenum,qopengl_GLintptr,qopengl_GLsizeiptr,const void*);
  GL_PROFILE(QOpenGLFunctions,glCheckFramebufferStatus,GLenum);
  GL_PROFILE(QOpenGLFunctions,glClearDepthf,GLclampf);
  GL_PROFILE(QOpenGLFunctions,glCompileShader,GLuint);
  //GL_PROFILE(QOpenGLFunctions,glCompressedTexImage2D,GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const void*);
  GL_PROFILE(QOpenGLFunctions,glCompressedTexSubImage2D,GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLsizei,const void*);
  GL_PROFILE(QOpenGLFunctions,glCreateProgram);
  GL_PROFILE(QOpenGLFunctions,glCreateShader,GLenum);
  GL_PROFILE(QOpenGLFunctions,glDeleteBuffers,GLsizei,const GLuint*);
  GL_PROFILE(QOpenGLFunctions,glDeleteFramebuffers,GLsizei,const GLuint*);
  GL_PROFILE(QOpenGLFunctions,glDeleteProgram,GLuint);
  GL_PROFILE(QOpenGLFunctions,glDeleteRenderbuffers,GLsizei,const GLuint*);
  GL_PROFILE(QOpenGLFunctions,glDeleteShader,GLuint);
  GL_PROFILE(QOpenGLFunctions,glDepthRangef,GLclampf,GLclampf);
  GL_PROFILE(QOpenGLFunctions,glDetachShader,GLuint,GLuint);
  GL_PROFILE(QOpenGLFunctions,glDisableVertexAttribArray,GLuint);
  GL_PROFILE(QOpenGLFunctions,glEnableVertexAttribArray,GLuint);
  GL_PROFILE(QOpenGLFunctions,glFramebufferRenderbuffer,GLenum,GLenum,GLenum,GLuint);
  GL_PROFILE(QOpenGLFunctions,glFramebufferTexture2D,GLenum,GLenum,GLenum,GLuint,GLint);
  GL_PROFILE(QOpenGLFunctions,glGenBuffers,GLsizei,GLuint*);
  GL_PROFILE(QOpenGLFunctions,glGenerateMipmap,GLenum);
  GL_PROFILE(QOpenGLFunctions,glGenFramebuffers,GLsizei,GLuint*);
  GL_PROFILE(QOpenGLFunctions,glGenRenderbuffers,GLsizei,GLuint*);
  GL_PROFILE(QOpenGLFunctions,glGetActiveAttrib,GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,char*);
  GL_PROFILE(QOpenGLFunctions,glGetActiveUniform,GLuint,GLuint,GLsizei,GLsizei*,GLint*,GLenum*,char*);
  GL_PROFILE(QOpenGLFunctions,glGetAttachedShaders,GLuint,GLsizei,GLsizei*,GLuint*);
  GL_PROFILE(QOpenGLFunctions,glGetAttribLocation,GLuint,const char*);
  GL_PROFILE(QOpenGLFunctions,glGetBufferParameteriv,GLenum,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetFramebufferAttachmentParameteriv,GLenum,GLenum,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetProgramiv,GLuint,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetProgramInfoLog,GLuint,GLsizei,GLsizei*,char*);
  GL_PROFILE(QOpenGLFunctions,glGetRenderbufferParameteriv,GLenum,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetShaderiv,GLuint,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetShaderInfoLog,GLuint,GLsizei,GLsizei*,char*);
  GL_PROFILE(QOpenGLFunctions,glGetShaderPrecisionFormat,GLenum,GLenum,GLint*,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetShaderSource,GLuint,GLsizei,GLsizei*,char*);
  GL_PROFILE(QOpenGLFunctions,glGetUniformfv,GLuint,GLint,GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glGetUniformiv,GLuint,GLint,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetUniformLocation,GLuint,const char*);
  GL_PROFILE(QOpenGLFunctions,glGetVertexAttribfv,GLuint,GLenum,GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glGetVertexAttribiv,GLuint,GLenum,GLint*);
  GL_PROFILE(QOpenGLFunctions,glGetVertexAttribPointerv,GLuint,GLenum,void**);
  GL_PROFILE(QOpenGLFunctions,glIsBuffer,GLuint);
  GL_PROFILE(QOpenGLFunctions,glIsFramebuffer,GLuint);
  GL_PROFILE(QOpenGLFunctions,glIsProgram,GLuint);
  GL_PROFILE(QOpenGLFunctions,glIsRenderbuffer,GLuint);
  GL_PROFILE(QOpenGLFunctions,glIsShader,GLuint);
  GL_PROFILE(QOpenGLFunctions,glLinkProgram,GLuint);
  GL_PROFILE(QOpenGLFunctions,glReleaseShaderCompiler);
  GL_PROFILE(QOpenGLFunctions,glRenderbufferStorage,GLenum,GLenum,GLsizei,GLsizei);
  GL_PROFILE(QOpenGLFunctions,glSampleCoverage,GLclampf,GLboolean);
  GL_PROFILE(QOpenGLFunctions,glShaderBinary,GLint,const GLuint*,GLenum,const void*,GLint);
  GL_PROFILE(QOpenGLFunctions,glShaderSource,GLuint,GLsizei,const char**,const GLint*);
  GL_PROFILE(QOpenGLFunctions,glStencilFuncSeparate,GLenum,GLenum,GLint,GLuint);
  GL_PROFILE(QOpenGLFunctions,glStencilMaskSeparate,GLenum,GLuint);
  GL_PROFILE(QOpenGLFunctions,glStencilOpSeparate,GLenum,GLenum,GLenum,GLenum);
  GL_PROFILE(QOpenGLFunctions,glUniform1f,GLint,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glUniform1fv,GLint,GLsizei,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUniform1i,GLint,GLint);
  GL_PROFILE(QOpenGLFunctions,glUniform1iv,GLint,GLsizei,const GLint*);
  GL_PROFILE(QOpenGLFunctions,glUniform2f,GLint,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glUniform2fv,GLint,GLsizei,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUniform2i,GLint,GLint,GLint);
  GL_PROFILE(QOpenGLFunctions,glUniform2iv,GLint,GLsizei,const GLint*);
  GL_PROFILE(QOpenGLFunctions,glUniform3f,GLint,GLfloat,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glUniform3fv,GLint,GLsizei,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUniform3i,GLint,GLint,GLint,GLint);
  GL_PROFILE(QOpenGLFunctions,glUniform3iv,GLint,GLsizei,const GLint*);
  GL_PROFILE(QOpenGLFunctions,glUniform4f,GLint,GLfloat,GLfloat,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glUniform4fv,GLint,GLsizei,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUniform4i,GLint,GLint,GLint,GLint,GLint);
  GL_PROFILE(QOpenGLFunctions,glUniform4iv,GLint,GLsizei,const GLint*);
  GL_PROFILE(QOpenGLFunctions,glUniformMatrix2fv,GLint,GLsizei,GLboolean,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUniformMatrix3fv,GLint,GLsizei,GLboolean,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUniformMatrix4fv,GLint,GLsizei,GLboolean,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glUseProgram,GLuint);
  GL_PROFILE(QOpenGLFunctions,glValidateProgram,GLuint);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib1f,GLuint,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib1fv,GLuint,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib2f,GLuint,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib2fv,GLuint,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib3f,GLuint,GLfloat,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib3fv,GLuint,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib4f,GLuint,GLfloat,GLfloat,GLfloat,GLfloat);
  GL_PROFILE(QOpenGLFunctions,glVertexAttrib4fv,GLuint,const GLfloat*);
  GL_PROFILE(QOpenGLFunctions,glVertexAttribPointer,GLuint,GLint,GLenum,GLboolean,GLsizei,const void*);

};

#endif // OPENGLFUNCTIONS_H

