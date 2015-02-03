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

#include <KAbstractObjParser>
#include <KFileReader>

class TimedParser : public KAbstractObjParser
{
public:
  TimedParser(KAbstractReader *reader) : KAbstractObjParser(reader) {}
  virtual void onVertex(const float vertex[4]) { (void)vertex; }
  virtual void onTexture(const float texture[3]) { (void)texture; }
  virtual void onNormal(const float normal[3]) { (void)normal; }
  virtual void onParameter(const float parameter[3]) { (void)parameter; }
  virtual void onFace(const index_type indices[3], size_type count) { (void)indices; (void)count; }
  virtual void onGroup(const char *group) { (void)group; }
  virtual void onMaterial(const char *file) { (void)file; }
  virtual void onUseMaterial(const char *file) { (void)file; }
  virtual void onObject(const char *obj) { (void)obj; }
  virtual void onSmooth(const char *obj) { (void)obj; }
};
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QElapsedTimer>
int main(int argc, char *argv[])
{
  QElapsedTimer entireTimer;
  QElapsedTimer overallTimer;
  QElapsedTimer fileTimer;
  TimedParser *parser;
  KFileReader *reader;
  QFileInfoList files = QDir(":/objects").entryInfoList();

  entireTimer.start();
  for (int i = 0; i < 10; ++i)
  {
    overallTimer.start();
    foreach (const QFileInfo &fileInfo, files)
    {
      QFile file = fileInfo.absoluteFilePath();
      reader = new KFileReader(&file);
      parser = new TimedParser(reader);
      fileTimer.start();
      if (parser->parse() == false)
      {
        qFatal("File '%s' did not parse correctly!", qPrintable(file.fileName()));
      }
      quint64 ms = fileTimer.elapsed();
      qDebug() << qPrintable(file.fileName()) << " => " << ms / 1e3f << "secs.";
    }
    quint64 ms = overallTimer.elapsed();
    qDebug() << " Overall => " << ms / 1e6f << "secs.";
  }
  quint64 ms = entireTimer.elapsed();
  qDebug() << " Entire => " << ms / 1e3f << "secs.";
  return 0;

  QApplication app(argc, argv);
  std::vector<QSurfaceFormat> formats;

#if !defined(QT_OPENGL_ES)
  {
    QSurfaceFormat glFormat;
    glFormat.setRenderableType(QSurfaceFormat::OpenGL);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    glFormat.setVersion(3,3);
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
  format->setDepthBufferSize(8);

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
