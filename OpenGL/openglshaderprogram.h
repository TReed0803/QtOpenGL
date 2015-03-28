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

// Register to profile OpenGLShaderProgram
class OpenGLShaderProgramProfiled : public OpenGLShaderProgramChecked
{
public:
  explicit OpenGLShaderProgramProfiled(QObject *parent = 0) : OpenGLShaderProgramChecked(parent) {}
  GL_PROFILE(OpenGLShaderProgramChecked,addShader,QOpenGLShader *)
  GL_PROFILE(OpenGLShaderProgramChecked,addShaderFromSourceCode,QOpenGLShader::ShaderType, const char *)
  GL_PROFILE(OpenGLShaderProgramChecked,addShaderFromSourceCode,QOpenGLShader::ShaderType, const QByteArray &)
  GL_PROFILE(OpenGLShaderProgramChecked,addShaderFromSourceCode,QOpenGLShader::ShaderType, const QString &)
  GL_PROFILE(OpenGLShaderProgramChecked,addShaderFromSourceFile,QOpenGLShader::ShaderType, const QString &)
  GL_PROFILE_CONST(OpenGLShaderProgramChecked,attributeLocation,const char *)
  GL_PROFILE_CONST(OpenGLShaderProgramChecked,attributeLocation,const QByteArray &)
  GL_PROFILE_CONST(OpenGLShaderProgramChecked,attributeLocation,const QString &)
  GL_PROFILE(OpenGLShaderProgramChecked,bind)
  GL_PROFILE(OpenGLShaderProgramChecked,bindAttributeLocation,const char *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,bindAttributeLocation,const QByteArray &, int)
  GL_PROFILE(OpenGLShaderProgramChecked,bindAttributeLocation,const QString &, int)
  GL_PROFILE(OpenGLShaderProgramChecked,create)
  GL_PROFILE(OpenGLShaderProgramChecked,disableAttributeArray,int)
  GL_PROFILE(OpenGLShaderProgramChecked,disableAttributeArray,const char *)
  GL_PROFILE(OpenGLShaderProgramChecked,enableAttributeArray,int)
  GL_PROFILE(OpenGLShaderProgramChecked,enableAttributeArray,const char *)
  GL_PROFILE(OpenGLShaderProgramChecked,isLinked)
  GL_PROFILE(OpenGLShaderProgramChecked,link)
  GL_PROFILE(OpenGLShaderProgramChecked,maxGeometryOutputVertices)
  GL_PROFILE(OpenGLShaderProgramChecked,patchVertexCount)
  GL_PROFILE(OpenGLShaderProgramChecked,release)
  GL_PROFILE(OpenGLShaderProgramChecked,removeAllShaders)
  GL_PROFILE(OpenGLShaderProgramChecked,removeShader,QOpenGLShader *)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,int, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,int, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,int, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,int, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,int, GLenum, const void *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,const char *, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,const char *, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,const char *, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,const char *, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeArray,const char *, GLenum, const void *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeBuffer,int, GLenum, int, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeBuffer,const char *, GLenum, int, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, const QColor &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,int, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, const QColor &)
  GL_PROFILE(OpenGLShaderProgramChecked,setAttributeValue,const char *, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setDefaultInnerTessellationLevels,const QVector<float> &)
  GL_PROFILE(OpenGLShaderProgramChecked,setDefaultOuterTessellationLevels,const QVector<float> &)
  GL_PROFILE(OpenGLShaderProgramChecked,setPatchVertexCount,int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int,GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QPointF &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QSize &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QSizeF &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix2x2 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix2x3 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix2x4 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix3x2 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix3x3 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix3x4 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix4x2 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix4x3 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QMatrix4x4 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QTransform &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, GLint)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, GLuint)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QColor &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QPoint &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QPointF &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QSize &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QSizeF &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix2x2 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix2x3 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix2x4 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix3x2 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix3x3 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix3x4 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix4x2 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix4x3 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QMatrix4x4 &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,int, const QTransform &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, GLint)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, GLuint)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, GLfloat, GLfloat, GLfloat, GLfloat)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QVector2D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QVector3D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QVector4D &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QColor &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValue,const char *, const QPoint &)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const GLint *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const GLuint *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix2x2 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix2x3 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix2x4 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix3x2 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix3x3 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix3x4 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix4x2 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix4x3 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,int, const QMatrix4x4 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const GLint *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const GLuint *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const GLfloat *, int, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QVector2D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QVector3D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QVector4D *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix2x2 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix2x3 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix2x4 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix3x2 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix3x3 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix3x4 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix4x2 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix4x3 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,setUniformValueArray,const char *, const QMatrix4x4 *, int)
  GL_PROFILE(OpenGLShaderProgramChecked,uniformLocation,const char *)
  GL_PROFILE(OpenGLShaderProgramChecked,uniformLocation,const QByteArray &)
  GL_PROFILE(OpenGLShaderProgramChecked,uniformLocation,const QString &)
};

// Final class
class OpenGLShaderProgramPrivate;
class OpenGLShaderProgram : public OpenGLShaderProgramProfiled
{
public:

  explicit OpenGLShaderProgram(QObject *parent = 0);
  ~OpenGLShaderProgram();
  void addIncludePath(char const *path);
  static void addSharedIncludePath(char const *path);
  bool addShaderFromSourceFile(QOpenGLShader::ShaderType type, const QString & fileName);
  void uniformBlockBinding(char const* location, unsigned index);
  void uniformBlockBinding(unsigned location, unsigned index);
  unsigned uniformBlockLocation(char const* location);
  void scheduleUniformBlockUpdate(unsigned location, unsigned index);
  void scheduleUniformUpdate(unsigned location, unsigned index);
  QString getVersionComment();
  void addShaderDefines(char const *defs);
  bool link();
  bool bind();
private:
  OpenGLShaderProgramPrivate *m_private;
};

#endif // OPENGLSHADERPROGRAM_H
