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

  // Concatenate version number
  // Note: Version number calculation is different based on GLES/GL and
  //       current context version number. It's a little more difficult than
  //       just concatenating some numbers together.
  if (ctx->isOpenGLES())
  {
    comment += QString::number(version.first) + QString::number(version.second) + "0 es";
  }
  else
  {
    switch (version.first)
    {
      case 2:
        switch (version.second)
        {
          case 0:
            comment += "110";
            break;
          case 1:
            comment += "120";
            break;
          default:
            comment += "120";
            break;
        }
        break;
      case 3:
        switch (version.second)
        {
          case 0:
            comment += "130";
            break;
          case 1:
            comment += "140";
            break;
          case 2:
            comment += "150";
            break;
          default:
            comment += QString::number(version.first) + QString::number(version.second) + "0";
            break;
        }
        break;
      default:
        comment += QString::number(version.first) + QString::number(version.second) + "0";
        break;
    }
  }

  return comment + "\n";
}

