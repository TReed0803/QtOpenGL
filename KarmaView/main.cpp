#include "mainwidget.h"
#include <vector>
#include <QApplication>
#include <QSurfaceFormat>
#include <QMainWindow>
#include "mainwindow.h"
#include <QPlainTextEdit>
#include <QMessageBox>

QPlainTextEdit *sg_console = nullptr;

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

static void handleMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  //in this function, you can write the message to any stream!
  switch (type) {
  case QtDebugMsg:
    if (sg_console) sg_console->appendPlainText(msg);
      break;
  case QtWarningMsg:
    if (sg_console) sg_console->appendPlainText(msg);
      break;
  case QtCriticalMsg:
    if (sg_console) sg_console->appendPlainText(msg);
      break;
  case QtFatalMsg:
    if (sg_console) sg_console->appendPlainText(msg);
      abort();
  }
}

int main(int argc, char *argv[])
{
  qInstallMessageHandler(&handleMessageOutput);
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
    QMessageBox::critical(0, "Critical Error", "No valid supported version of OpenGL found on device!");
    exit(-1);
  }

#ifdef    GL_DEBUG
  format->setOption(QSurfaceFormat::DebugContext);
#endif // GL_DEBUG
  format->setDepthBufferSize(0);

  // Set the widget up
  MainWidget *widget = new MainWidget;
  widget->setFormat(*format);

  // Set the window up
  MainWindow *window = new MainWindow;
  window->show();

  return app.exec();
}
