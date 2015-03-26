#include "openglwidget.h"
#include "openglframeresults.h"
#include "openglframetimer.h"
#include "openglprofiler.h"
#include "openglprofilervisualizer.h"

#include <QApplication>
#include <QFileDialog>
#include <QGestureEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <OpenGLDebugDraw>
#include <OpenGLFramebufferObject>

#include <KCommon>
#include <KInputManager>
#include <KUpdateEvent>

static OpenGLWidget *sg_widget;

/*******************************************************************************
 * OpenGLWidgetPrivate
 ******************************************************************************/
class OpenGLWidgetPrivate
{
public:
  OpenGLWidgetPrivate(QObject *parent = 0);

  // Rendering Statistics
  bool m_profilerVisible;
  OpenGLProfiler m_profiler;
  OpenGLProfilerVisualizer m_profilerVisualizer;
  OpenGLFrameTimer m_frameTimer;
  QOpenGLDebugLogger *m_debugLogger;
};

OpenGLWidgetPrivate::OpenGLWidgetPrivate(QObject *parent) :
  m_profilerVisible(false), m_profiler(parent), m_profilerVisualizer(parent), m_frameTimer(parent), m_debugLogger(Q_NULLPTR)
{
  // Intentionally Empty
}

/*******************************************************************************
 * OpenGLWidget
 ******************************************************************************/


OpenGLWidget::OpenGLWidget(QWidget *parent) :
  QOpenGLWidget(parent), m_private(new OpenGLWidgetPrivate(this))
{
  sg_widget = this;
  P(OpenGLWidgetPrivate);
  connect(&p.m_frameTimer, SIGNAL(timeout(float)), this, SLOT(frameTimeout(float)));
  OpenGLError::pushErrorHandler(this);
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::PinchGesture);
  setAttribute(Qt::WA_AcceptTouchEvents);
  grabKeyboard();
  grabMouse();
  setMouseTracking(true);
}

OpenGLWidget::~OpenGLWidget()
{
  makeCurrent();
  delete m_private;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/
void OpenGLWidget::printVersionInformation()
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

void OpenGLWidget::setProfilerVisible(bool visible)
{
  P(OpenGLWidgetPrivate);
  p.m_profilerVisible = visible;
}

bool OpenGLWidget::profilerVisible() const
{
  P(OpenGLWidgetPrivate);
  return p.m_profilerVisible;
}

/*******************************************************************************
 * OpenGL Protected Methods
 ******************************************************************************/
void OpenGLWidget::initializeGL()
{
  P(OpenGLWidgetPrivate);

  // Initialize
  initializeOpenGLFunctions();
  GL::setInstance(this);
  if (p.m_profiler.initialize())
  {
    connect(&p.m_profiler, SIGNAL(frameResultsAvailable(OpenGLFrameResults)), &p.m_profilerVisualizer, SLOT(frameResultsAvailable(OpenGLFrameResults)));
  }
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

  // Add a debug messanger if running in debug mode.
#ifdef    GL_DEBUG
  printVersionInformation();
  p.m_debugLogger = new QOpenGLDebugLogger(this);
  if (p.m_debugLogger->initialize())
  {
    qDebug() << "GL_DEBUG Debug Logger" << p.m_debugLogger;
    connect(p.m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
    p.m_debugLogger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType,  QOpenGLDebugMessage::NotificationSeverity);
    p.m_debugLogger->startLogging();
  }
  else
  {
    qDebug() << "GL_DEBUG Debug Logger (NONE)";
  }
#endif // GL_DEBUG

  connect(&p.m_profiler, SIGNAL(frameResultsAvailable(OpenGLFrameResults)), this, SLOT(frameResultAvailable(OpenGLFrameResults)));
  QOpenGLWidget::initializeGL();
  OpenGLDebugDraw::initialize();
}

void OpenGLWidget::resizeGL(int width, int height)
{
  P(OpenGLWidgetPrivate);
  p.m_profilerVisualizer.resizeGL(width, height);
  QOpenGLWidget::resizeGL(width, height);
  OpenGLFramebufferObject::setRelease(defaultFramebufferObject());
}

void OpenGLWidget::paintGL()
{
  P(OpenGLWidgetPrivate);
  if (p.m_profilerVisible)
  {
    p.m_profilerVisualizer.paintGL();
  }
  OpenGLDebugDraw::draw();
  QOpenGLWidget::paintGL();
}

void OpenGLWidget::teardownGL()
{
  OpenGLDebugDraw::teardown();
}

/*******************************************************************************
 * Event Methods
 ******************************************************************************/
bool OpenGLWidget::event(QEvent *e)
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
  else if (e->type() == QEvent::Gesture)
  {
    gestureEvent(static_cast<QGestureEvent*>(e));
    return true;
  }
  else if (e->type() == QEvent::Move)
  {
    moveEvent(static_cast<QMoveEvent*>(e));
    return true;
  }
  else if (e->type() == QEvent::KeyPress)
  {
    keyPressEvent(static_cast<QKeyEvent*>(e));
    return true;
  }
  else if (e->type() == QEvent::KeyRelease)
  {
    keyReleaseEvent(static_cast<QKeyEvent*>(e));
    return true;
  }
  else if (e->type() == QEvent::TouchBegin)
  {
    touchEvent(static_cast<QTouchEvent*>(e));
    return true;
  }
  else if (e->type() == QEvent::TouchUpdate)
  {
    touchEvent(static_cast<QTouchEvent*>(e));
    return true;
  }
  else if (e->type() == QEvent::TouchEnd)
  {
    touchEvent(static_cast<QTouchEvent*>(e));
    return true;
  }
  return QOpenGLWidget::event(e);
}

void OpenGLWidget::errorEventGL(OpenGLError *event)
{
  qFatal(
    "%s::%s => Returned an error!",
    event->callerName(),
    event->functionName()
  );
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
  KInputManager::registerKeyPressEvent(event);
  QOpenGLWidget::keyPressEvent(event);
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
  KInputManager::registerKeyReleaseEvent(event);
  QOpenGLWidget::keyReleaseEvent(event);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
  KInputManager::registerMousePressEvent(event);
  QOpenGLWidget::mousePressEvent(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  KInputManager::registerMouseReleaseEvent(event);
  QOpenGLWidget::mouseReleaseEvent(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
  KInputManager::registerMouseMoveEvent(event);
  QOpenGLWidget::mouseMoveEvent(event);
}

void OpenGLWidget::moveEvent(QMoveEvent *event)
{
  P(OpenGLWidgetPrivate);
  p.m_profilerVisualizer.moveEvent(event);
  QOpenGLWidget::moveEvent(event);
}

void OpenGLWidget::touchEvent(QTouchEvent *event)
{
  KInputManager::registerTouchEvent(event);
}

void OpenGLWidget::gestureEvent(QGestureEvent *event)
{
  KInputManager::registerGestureEvent(event);
}

void OpenGLWidget::updateEvent(KUpdateEvent *event)
{
  (void)event;
}

void OpenGLWidget::sMakeCurrent()
{
  sg_widget->makeCurrent();
}

KString OpenGLWidget::openFileName(const char *title, const char *dir, const char *fileTypes)
{
  return QFileDialog::getOpenFileName(sg_widget, title, dir, fileTypes);
}

/*******************************************************************************
 * Public Slots
 ******************************************************************************/
void OpenGLWidget::update()
{
  KInputManager::update();

  // Update Logic
  {
    KUpdateEvent ev;
    QCoreApplication::sendEvent(this, &ev);
  }

  QOpenGLWidget::update();
}

void OpenGLWidget::frameTimeout(float fps)
{
  QString format("FPS: %1");
}

void OpenGLWidget::messageLogged(const QOpenGLDebugMessage &msg)
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

void OpenGLWidget::frameResultAvailable(const OpenGLFrameResults &result)
{
  (void)result;
}
