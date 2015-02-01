#include "openglwindow.h"
#include "openglprofiler.h"
#include "openglprofilervisualizer.h"
#include "openglframetimer.h"
#include "openglframeresults.h"

#include <QKeyEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QCoreApplication>

#include <KCommon>
#include <KDebugDraw>
#include <KInputManager>
#include <KUpdateEvent>

/*******************************************************************************
 * OpenGLWindowPrivate
 ******************************************************************************/
class OpenGLWindowPrivate
{
public:
  OpenGLWindowPrivate(QObject *parent = 0);

  // Rendering Statistics
  OpenGLProfiler m_profiler;
  OpenGLProfilerVisualizer m_profilerVisualizer;
  OpenGLFrameTimer m_frameTimer;
  QOpenGLDebugLogger *m_debugLogger;
};

OpenGLWindowPrivate::OpenGLWindowPrivate(QObject *parent) :
  m_profiler(parent), m_profilerVisualizer(parent), m_frameTimer(parent), m_debugLogger(Q_NULLPTR)
{
  // Intentionally Empty
}

/*******************************************************************************
 * OpenGLWindow
 ******************************************************************************/
#define P(c) c &p = *m_private

OpenGLWindow::OpenGLWindow(UpdateBehavior updateBehavior, QWindow *parent) :
  QOpenGLWindow(updateBehavior, parent), m_private(new OpenGLWindowPrivate(this))
{
  P(OpenGLWindowPrivate);
  connect(&p.m_frameTimer, SIGNAL(timeout(float)), this, SLOT(frameTimeout(float)));
  OpenGLError::pushErrorHandler(this);
}

OpenGLWindow::~OpenGLWindow()
{
  makeCurrent();
  delete m_private;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/
void OpenGLWindow::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

/*******************************************************************************
 * OpenGL Protected Methods
 ******************************************************************************/
void OpenGLWindow::initializeGL()
{
  P(OpenGLWindowPrivate);

  // Initialize
  initializeOpenGLFunctions();
  KDebugDraw::initialize();
  if (p.m_profiler.initialize())
  {
    connect(&p.m_profiler, SIGNAL(frameResultsAvailable(OpenGLFrameResults)), &p.m_profilerVisualizer, SLOT(frameResultsAvailable(OpenGLFrameResults)));
  }
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  connect(this, SIGNAL(frameSwapped()), &p.m_frameTimer, SLOT(frameSwapped()));

  // Add a debug messanger if running in debug mode.
#ifdef    GL_DEBUG
  p.m_debugLogger = new QOpenGLDebugLogger(this);
  if (p.m_debugLogger->initialize())
  {
    qDebug() << "GL_DEBUG Debug Logger" << p.m_debugLogger;
    connect(p.m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
    p.m_debugLogger->startLogging();
  }
  else
  {
    qDebug() << "GL_DEBUG Debug Logger (NONE)";
  }
#endif // GL_DEBUG

  connect(&p.m_profiler, SIGNAL(frameResultsAvailable(OpenGLFrameResults)), this, SLOT(frameResultAvailable(OpenGLFrameResults)));
  QOpenGLWindow::initializeGL();
}

void OpenGLWindow::resizeGL(int width, int height)
{
  P(OpenGLWindowPrivate);
  p.m_profilerVisualizer.resizeGL(width, height);
  QOpenGLWindow::resizeGL(width, height);
}

void OpenGLWindow::paintGL()
{
  P(OpenGLWindowPrivate);
  p.m_profilerVisualizer.paintGL();
  QOpenGLWindow::paintGL();
}

void OpenGLWindow::teardownGL()
{
  KDebugDraw::teardown();
}

/*******************************************************************************
 * Event Methods
 ******************************************************************************/
bool OpenGLWindow::event(QEvent *e)
{
  if (e->type() == OpenGLError::type())
  {
    errorEventGL(static_cast<OpenGLError*>(e));
    return true;
  }
  else if (e->type() == KUpdateEvent::type())
  {
    updateEvent(static_cast<KUpdateEvent*>(e));
    return true;
  }
  return QOpenGLWindow::event(e);
}

void OpenGLWindow::errorEventGL(OpenGLError *event)
{
  qFatal(
    "%s::%s => Returned an error!",
    event->callerName(),
    event->functionName()
  );
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    KInputManager::registerKeyPress(event->key());
  }
}

void OpenGLWindow::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    KInputManager::registerKeyRelease(event->key());
  }
}

void OpenGLWindow::mousePressEvent(QMouseEvent *event)
{
  KInputManager::registerMousePress(event->button());
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event)
{
  KInputManager::registerMouseRelease(event->button());
}

void OpenGLWindow::moveEvent(QMoveEvent *event)
{
  P(OpenGLWindowPrivate);
  p.m_profilerVisualizer.moveEvent(event);
  QOpenGLWindow::moveEvent(event);
}

void OpenGLWindow::updateEvent(KUpdateEvent *event)
{
  (void)event;
}

/*******************************************************************************
 * Public Slots
 ******************************************************************************/
void OpenGLWindow::update()
{
  KInputManager::update();

  // Update Logic
  {
    KUpdateEvent ev;
    QCoreApplication::sendEvent(this, &ev);
  }

  QOpenGLWindow::update();
}

void OpenGLWindow::frameTimeout(float fps)
{
  QString format("FPS: %1");
  setTitle(
    format.arg( Karma::round(fps) )
  );
}

void OpenGLWindow::messageLogged(const QOpenGLDebugMessage &msg)
{
  QString error;

  // Format based on severity
  switch (msg.severity())
  {
  case QOpenGLDebugMessage::NotificationSeverity:
    error += "--";
    break;
  case QOpenGLDebugMessage::HighSeverity:
    error += "!!";
    break;
  case QOpenGLDebugMessage::MediumSeverity:
    error += "!~";
    break;
  case QOpenGLDebugMessage::LowSeverity:
    error += "~~";
    break;
  case QOpenGLDebugMessage::InvalidSeverity:
  case QOpenGLDebugMessage::AnySeverity:
    error += "??";
    break;
  }

  error += " (";

  // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
  switch (msg.source())
  {
    CASE(APISource);
    CASE(WindowSystemSource);
    CASE(ShaderCompilerSource);
    CASE(ThirdPartySource);
    CASE(ApplicationSource);
    CASE(OtherSource);
    CASE(InvalidSource);
    CASE(AnySource);
  }
#undef CASE

  error += " : ";

  // Format based on type
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
  switch (msg.type())
  {
    CASE(ErrorType);
    CASE(DeprecatedBehaviorType);
    CASE(UndefinedBehaviorType);
    CASE(PortabilityType);
    CASE(PerformanceType);
    CASE(OtherType);
    CASE(MarkerType);
    CASE(GroupPushType);
    CASE(GroupPopType);
    CASE(AnyType);
    CASE(InvalidType);
  }
#undef CASE

  error += ")";
  qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
}

void OpenGLWindow::frameResultAvailable(const OpenGLFrameResults &result)
{
  (void)result;
}
