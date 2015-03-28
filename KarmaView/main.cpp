#include "mainwidget.h"
#include <vector>
#include <QApplication>
#include <QSurfaceFormat>
#include <QMainWindow>

static bool checkVersion(QOpenGLContext &context, QSurfaceFormat &format)
{
  QSurfaceFormat currSurface = context.format();
  QPair<int,int> currVersion = currSurface.version();
  QPair<int,int> reqVersion = format.version();
  if (currVersion.first > reqVersion.first)
    return true;
  return (currVersion.first == reqVersion.first && currVersion.second >= reqVersion.second);
}

static QSurfaceFormat* getFirstSupported(std::vector<QSurfaceFormat> &formats)
{
  QOpenGLContext context;
  for (QSurfaceFormat &format : formats)
  {
    context.setFormat(format);
    if (context.create())
    {
      if (checkVersion(context, format)) return &format;
    }
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  std::vector<QSurfaceFormat> formats;

#if !defined(QT_OPENGL_ES)
  {
    QSurfaceFormat glFormat;
    glFormat.setRenderableType(QSurfaceFormat::OpenGL);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(4,3);
    formats.push_back(glFormat);
  }
#endif

#if defined(QT_OPENGL_ES)
  {
    QSurfaceFormat glesFormat;
    glesFormat.setRenderableType(QSurfaceFormat::OpenGLES);
    glesFormat.setProfile(QSurfaceFormat::CoreProfile);
    glesFormat.setVersion(3,0);
    formats.push_back(glesFormat);
  }
#endif

  // Find out which version we support
  QSurfaceFormat *format = getFirstSupported(formats);
  if (format == NULL)
  {
    qFatal("No valid supported version of OpenGL found on device!");
  }

#ifdef    GL_DEBUG
  format->setOption(QSurfaceFormat::DebugContext);
#endif // GL_DEBUG
  format->setDepthBufferSize(0);

  // Set the widget up
  MainWidget *widget = new MainWidget;
  widget->setFormat(*format);

  // Set the window up
  QMainWindow window;
  window.resize(800, 600);
  window.setCentralWidget(widget);
  window.show();

  return app.exec();
}
