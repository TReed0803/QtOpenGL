#include <QSurfaceFormat>
#include <QFile>
#include <QOpenGLContext>
#include "openglshaderprogram.h"

bool OpenGLShaderProgramWrapped::addShaderFromSourceCode(QOpenGLShader::ShaderType type, const char *source)
{
  QString version = getVersionComment();
  return OpenGLShaderProgramChecked::addShaderFromSourceCode(type, version + source);
}

bool OpenGLShaderProgramWrapped::addShaderFromSourceCode(QOpenGLShader::ShaderType type, const QByteArray &source)
{
  QString version = getVersionComment();
  return OpenGLShaderProgramChecked::addShaderFromSourceCode(type, version + source);
}

bool OpenGLShaderProgramWrapped::addShaderFromSourceCode(QOpenGLShader::ShaderType type, const QString &source)
{
  QString version = getVersionComment();
  return OpenGLShaderProgramChecked::addShaderFromSourceCode(type, version + source);
}

bool OpenGLShaderProgramWrapped::addShaderFromSourceFile(QOpenGLShader::ShaderType type, const QString &fileName)
{
  QString version = getVersionComment();
  QFile file(fileName);
  if (file.open(QFile::ReadOnly | QFile::Text))
  {
    QTextStream in(&file);
    QString source = version + in.readAll();
    return OpenGLShaderProgramChecked::addShaderFromSourceCode(type, source);
  }
  return false;
}

QString OpenGLShaderProgramWrapped::getVersionComment()
{
  QString comment = "#version ";
  QOpenGLContext *ctx = QOpenGLContext::currentContext();
  QSurfaceFormat format = ctx->format();
  QPair<int,int> version = format.version();
  comment += QString::number(version.first) + QString::number(version.second) + "0";
  if (ctx->isOpenGLES())
  {
    comment += " es";
  }
  return comment + "\n";
}

