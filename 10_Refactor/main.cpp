#include <QApplication>
#include <QSurfaceFormat>
#include "window.h"

static bool checkVersion(QOpenGLContext &context, QSurfaceFormat *fmt)
{
  QSurfaceFormat currSurface = context.format();
  QPair<int,int> currVersion = currSurface.version();
  QPair<int,int> reqVersion = fmt->version();
  if (currVersion.first > reqVersion.first)
    return true;
  return (currVersion.first == reqVersion.first && currVersion.second >= reqVersion.second);
}

static QSurfaceFormat* getFirstSupported(QSurfaceFormat *fmt, va_list ap)
{
  QOpenGLContext context;
  while (fmt)
  {
    context.setFormat(*fmt);
    if (context.create())
    {
      if (checkVersion(context, fmt)) return fmt;
    }
    fmt = va_arg(ap,QSurfaceFormat*);
  }
  return NULL;
}

static QSurfaceFormat* getFirstSupported(QSurfaceFormat *fmt, ...)
{
  va_list ap;
  va_start(ap,fmt);
  QSurfaceFormat *first = getFirstSupported(fmt, ap);
  va_end(ap);
  return first;
}

#include "halfedgemesh.h"
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // Set OpenGL Version information
  // Note: This format must be set before show() is called.
  QSurfaceFormat glFormat;
  glFormat.setRenderableType(QSurfaceFormat::OpenGL);
  glFormat.setProfile(QSurfaceFormat::CoreProfile);
  glFormat.setVersion(3,3);

  QSurfaceFormat glesFormat;
  glesFormat.setRenderableType(QSurfaceFormat::OpenGLES);
  glesFormat.setProfile(QSurfaceFormat::CoreProfile);
  glesFormat.setVersion(3,0);

  // Find out which version we support
  QSurfaceFormat *format = getFirstSupported(&glFormat, &glesFormat, NULL);
  if (format == NULL)
  {
    qFatal("No valid supported version of OpenGL found on device!");
  }

#ifdef    GL_DEBUG
  format->setOption(QSurfaceFormat::DebugContext);
#endif // GL_DEBUG
  format->setDepthBufferSize(8);

  // Set the window up
  Window window;
  window.setFormat(*format);
  window.resize(QSize(800, 600));
  window.show();

  return app.exec();
}
