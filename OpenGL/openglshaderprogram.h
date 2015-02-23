#ifndef OPENGLSHADERPROGRAM_H
#define OPENGLSHADERPROGRAM_H OpenGLShaderProgram

#include <OpenGLCommon>
#include <QOpenGLShaderProgram>

class OpenGLUniformBufferObject;

// Register to check OpenGLBuffer
class OpenGLShaderProgramChecked : public QOpenGLShaderProgram
{
public:
  explicit OpenGLShaderProgramChecked(QObject *parent = 0) : QOpenGLShaderProgram(parent) {}
  GL_CHECK(QOpenGLShaderProgram,addShader,QOpenGLShader*)
  GL_CHECK(QOpenGLShaderProgram,addShaderFromSourceCode,QOpenGLShader::ShaderType,const char*)
  GL_CHECK(QOpenGLShaderProgram,addShaderFromSourceCode,QOpenGLShader::ShaderType,const QByteArray&)
  GL_CHECK(QOpenGLShaderProgram,addShaderFromSourceCode,QOpenGLShader::ShaderType,const QString&)
  GL_CHECK(QOpenGLShaderProgram,addShaderFromSourceFile,QOpenGLShader::ShaderType,const QString&)
  GL_CHECK(QOpenGLShaderProgram,bind)
  GL_CHECK(QOpenGLShaderProgram,create)
  GL_CHECK(QOpenGLShaderProgram,link)
};

// Register to add version number to shader source.
class OpenGLShaderProgramWrappedPrivate;
class OpenGLShaderProgramWrapped : public OpenGLShaderProgramChecked
{
public:
  explicit OpenGLShaderProgramWrapped(QObject *parent = 0);
  ~OpenGLShaderProgramWrapped();
  void addIncludePath(char const *path);
  static void addSharedIncludePath(char const *path);
  bool addShaderFromSourceFile(QOpenGLShader::ShaderType type, const QString & fileName);
  void uniformBlockBinding(char const* location, OpenGLUniformBufferObject const &ubo);
  void uniformBlockBinding(unsigned location, OpenGLUniformBufferObject const &ubo);
  unsigned uniformBlockLocation(char const* location);
  QString getVersionComment();
  bool link();
private:
  OpenGLShaderProgramWrappedPrivate *m_private;
};

// Register to profile OpenGLShaderProgram
class OpenGLShaderProgramProfiled : public OpenGLShaderProgramWrapped
{
public:
  explicit OpenGLShaderProgramProfiled(QObject *parent = 0) : OpenGLShaderProgramWrapped(parent) {}
  GL_PROFILE(OpenGLShaderProgramWrapped,addShader,QOpenGLShader *)
  GL_PROFILE(OpenGLShaderProgramWrapped,addShaderFromSourceCode,QOpenGLShader::ShaderType, const char *)
  GL_PROFILE(OpenGLShaderProgramWrapped,addShaderFromSourceCode,QOpenGLShader::ShaderType, const QByteArray &)
  GL_PROFILE(OpenGLShaderProgramWrapped,addShaderFromSourceCode,QOpenGLShader::ShaderType, const QString &)
  GL_PROFILE(OpenGLShaderProgramWrapped,addShaderFromSourceFile,QOpenGLShader::ShaderType, const QString &)
  GL_PROFILE_CONST(OpenGLShaderProgramWrapped,attributeLocation,const char *)
  GL_PROFILE_CONST(OpenGLShaderProgramWrapped,attributeLocation,const QByteArray &)
  GL_PROFILE_CONST(OpenGLShaderProgramWrapped,attributeLocation,const QString &)
  GL_PROFILE(OpenGLShaderProgramWrapped,bind)
  GL_PROFILE(OpenGLShaderProgramWrapped,bindAttributeLocation,const char *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,bindAttributeLocation,const QByteArray &, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,bindAttributeLocation,const QString &, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,create)
  GL_PROFILE(OpenGLShaderProgramWrapped,disableAttributeArray,int)
  GL_PROFILE(OpenGLShaderProgramWrapped,disableAttributeArray,const char *)
  GL_PROFILE(OpenGLShaderProgramWrapped,enableAttributeArray,int)
  GL_PROFILE(OpenGLShaderProgramWrapped,enableAttributeArray,const char *)
  GL_PROFILE(OpenGLShaderProgramWrapped,isLinked)
  GL_PROFILE(OpenGLShaderProgramWrapped,link)
  GL_PROFILE(OpenGLShaderProgramWrapped,maxGeometryOutputVertices)
  GL_PROFILE(OpenGLShaderProgramWrapped,patchVertexCount)
  GL_PROFILE(OpenGLShaderProgramWrapped,release)
  GL_PROFILE(OpenGLShaderProgramWrapped,removeAllShaders)
  GL_PROFILE(OpenGLShaderProgramWrapped,removeShader,QOpenGLShader *)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,int, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,int, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,int, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,int, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,int, GLenum, const void *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,const char *, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,const char *, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,const char *, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,const char *, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeArray,const char *, GLenum, const void *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeBuffer,int, GLenum, int, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeBuffer,const char *, GLenum, int, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, const QColor &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,int, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, const QColor &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setAttributeValue,const char *, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setDefaultInnerTessellationLevels,const QVector<float> &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setDefaultOuterTessellationLevels,const QVector<float> &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setPatchVertexCount,int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int,GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QPointF &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QSize &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QSizeF &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix2x2 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix2x3 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix2x4 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix3x2 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix3x3 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix3x4 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix4x2 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix4x3 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QMatrix4x4 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QTransform &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, GLint)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, GLuint)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QColor &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QPoint &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QPointF &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QSize &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QSizeF &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix2x2 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix2x3 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix2x4 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix3x2 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix3x3 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix3x4 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix4x2 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix4x3 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QMatrix4x4 &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,int, const QTransform &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, GLint)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, GLuint)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QColor &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValue,const char *, const QPoint &)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const GLint *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const GLuint *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix2x2 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix2x3 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix2x4 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix3x2 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix3x3 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix3x4 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix4x2 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix4x3 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,int, const QMatrix4x4 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const GLint *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const GLuint *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix2x2 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix2x3 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix2x4 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix3x2 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix3x3 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix3x4 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix4x2 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix4x3 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,setUniformValueArray,const char *, const QMatrix4x4 *, int)
  GL_PROFILE(OpenGLShaderProgramWrapped,uniformLocation,const char *)
  GL_PROFILE(OpenGLShaderProgramWrapped,uniformLocation,const QByteArray &)
  GL_PROFILE(OpenGLShaderProgramWrapped,uniformLocation,const QString &)
};

// Final class
class OpenGLShaderProgram : public OpenGLShaderProgramProfiled
{
public:
  explicit OpenGLShaderProgram(QObject *parent = 0) : OpenGLShaderProgramProfiled(parent) {}
};

#endif // OPENGLSHADERPROGRAM_H
