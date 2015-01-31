#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugMessage>
#include <QKeyEvent>
#include <QElapsedTimer>
#include "halfedgemesh.h"
#include "vertex.h"
#include "input.h"
#include "profiler.h"
#include "debugdraw.h"
#include "frameresult.h"
#include <QDropEvent>
#include <QTouchEvent>

// Custom includes
#include <OpenGLError>
#include <OpenGLShaderProgram>

// Query Macros
#define from [&](){ std::vector<uint64_t> results; for(const auto &
#define in :
#define where ) if(
#define select ) { results.push_back(
#define end ); } return std::move(results); }

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

Window::Window()
{
#ifdef    GL_DEBUG
  m_profiler = new Profiler(this);
  m_profiler->setBorder(10, 10, 0, 10);
  m_profiler->setOffset(0.0f, 0.0f, 0.95f, 0.0f);
  Profiler::setProfiler(m_profiler);
#endif // GL_DEBUG
  m_transform.translate(0.0f, 0.0f, -5.0f);
  m_transform.scale(15.0f);
  OpenGLError::pushErrorHandler(this);
  m_halfEdgeMesh = Q_NULLPTR;
  m_mesh = Q_NULLPTR;
  m_wait = false;
}

Window::~Window()
{
  makeCurrent();
  OpenGLError::popErrorHandler();
}

void Window::initializeGL()
{
  // Initialize OpenGL Backend
  initializeOpenGLFunctions();
  connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  printVersionInformation();

  // Set global information
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glLineWidth(10.0f);
  glClearDepthf(1.0f);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef    GL_DEBUG
  m_debugLogger = new QOpenGLDebugLogger(this);
  if (m_debugLogger->initialize())
  {
    qDebug() << "GL_DEBUG Debug Logger" << m_debugLogger << "\n";
    connect(m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
    m_debugLogger->startLogging();
  }
  else
  {
    qDebug() << "GL_DEBUG Debug Logger (NONE)\n";
  }
  connect(m_profiler, SIGNAL(onFrameResult(FrameResult)), this, SLOT(onFrameResult(FrameResult)));
#endif // GL_DEBUG

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    m_program = new OpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    m_program->link();
    m_program->bind();

    loadObj(":/objects/bunny.obj");

    // Cache Uniform Locations
    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");

    // Release (unbind) all
    m_program->release();
  }

  DebugDraw::initialize(this);
}

void Window::resizeGL(int width, int height)
{
  m_projection.setToIdentity();
  m_projection.perspective(45.0f, width / float(height), 0.1f, 1000.0f);
  PROFILER_RESIZE_GL(width, height);
}

void Window::paintGL()
{

  // Clear
  PROFILER_SYNC_FRAME();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render the scene
  PROFILER_PUSH_GPU_MARKER("Render Scene");
  {
    PROFILER_PUSH_GPU_MARKER("Prepare Scene");
    m_program->bind();
    m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToView, m_projection);
    PROFILER_POP_GPU_MARKER();
    {
      PROFILER_PUSH_GPU_MARKER("Draw Object");
      m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
      if (m_mesh && !m_wait)
      {
        m_mesh->draw();
        HalfEdgeMesh::HalfEdge *edge, *twin;
        for (size_t i = 0; i < m_queryResults.size(); ++i)
        {
          edge = m_halfEdgeMesh->getEdge(m_queryResults[i]);
          twin = m_halfEdgeMesh->getTwin(m_queryResults[i]);
          QVector3D origin = m_projection * m_camera.toMatrix() * m_transform.toMatrix() * m_halfEdgeMesh->getVertex(twin->i_to)->position;
          QVector3D to   = m_projection * m_camera.toMatrix() * m_transform.toMatrix() * m_halfEdgeMesh->getVertex(edge->i_to)->position;
          if (origin.x() > 1.0 || origin.x() < -1.0f || origin.y() > 1.0 || origin.y() < -1.0f)
            if  (to.x() > 1.0 || to.x() < -1.0f || to.y() > 1.0 || to.y() < -1.0f)
              continue;
          DebugDraw::World::drawLine(origin, to, Qt::red);
        }
      }
      PROFILER_POP_GPU_MARKER();
    }
    m_program->release();
  }
  PROFILER_POP_GPU_MARKER();
  PROFILER_EMIT_RESULTS();

  // Render the profiler
  PROFILER_PAINT_GL();
  DebugDraw::draw();

}

void Window::teardownGL()
{
}

void Window::update()
{
  // Update input
  Input::update();

  // Camera Transformation
  if (Input::buttonPressed(Qt::RightButton))
  {
    static const float transSpeed = 0.5f;
    static const float rotSpeed   = 0.5f;

    // Handle rotations
    m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
    m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());

    // Handle translations
    QVector3D translation;
    if (Input::keyPressed(Qt::Key_W))
    {
      translation += m_camera.forward();
    }
    if (Input::keyPressed(Qt::Key_S))
    {
      translation -= m_camera.forward();
    }
    if (Input::keyPressed(Qt::Key_A))
    {
      translation -= m_camera.right();
    }
    if (Input::keyPressed(Qt::Key_D))
    {
      translation += m_camera.right();
    }
    if (Input::keyPressed(Qt::Key_Q))
    {
      translation -= Camera3D::LocalUp;
    }
    if (Input::keyPressed(Qt::Key_E))
    {
      translation += Camera3D::LocalUp;
    }
    m_camera.translate(transSpeed * translation);
  }

  if (Input::keyPressed(Qt::Key_Control))
  {
    if (Input::keyTriggered(Qt::Key_O))
    {
      openObj();
    }
  }

  // Schedule a redraw
  QOpenGLWindow::update();
}

void Window::messageLogged(const QOpenGLDebugMessage &msg)
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

void Window::onFrameResult(const FrameResult &result)
{
  (void)result;
}

bool Window::event(QEvent *e)
{
  if (e->type() == OpenGLError::type())
  {
    errorEventGL(static_cast<OpenGLError*>(e));
    return true;
  }
  return QOpenGLWindow::event(e);
}

void Window::errorEventGL(OpenGLError *event)
{
  qFatal("%s::%s => Returned an error!", event->callerName(), event->functionName());
}

void Window::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyPress(event->key());
  }
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyRelease(event->key());
  }
}

void Window::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}

void Window::moveEvent(QMoveEvent *ev)
{
  PROFILER_MOVE_EVENT(ev);
  QOpenGLWindow::moveEvent(ev);
}

void Window::wheelEvent(QWheelEvent *ev)
{
  m_transform.grow(ev->delta());
}

void Window::touchEvent(QTouchEvent *ev)
{
  auto te = ev->touchPoints();
  auto p = te[0];
  QPointF begin = p.lastScreenPos();
  QPointF endd = p.screenPos();
  QPointF delta = endd - begin;
  QVector3D v(delta.y(), delta.x(), 0.0f);
  if (v.length() > 0.001f)
  {
    v.normalize();
    m_transform.rotate(5.0f, v);
  }
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printVersionInformation()
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

void Window::loadObj(const QString &fileName)
{
  // Remove old mesh
  delete m_mesh;
  delete m_halfEdgeMesh;
  m_queryResults.clear();

  // Initialize an object
  quint64 ms;
  QElapsedTimer timer;
  {
    {
      timer.start();
      m_halfEdgeMesh = new HalfEdgeMesh(this, fileName);
      ms = timer.elapsed();
      qDebug() << "Create HalfEdgeMesh (sec):" << float(ms) / 1e3f;
    }
    {
      makeCurrent();
      timer.start();
      m_mesh = m_halfEdgeMesh->createOpenGLMesh(OpenGLMesh::Contiguous | OpenGLMesh::Interleaved);
      ms = timer.elapsed();
      qDebug() << "Create OpenGLMesh (sec)  :" << float(ms) / 1e3f;
    }
    auto query = from a in m_halfEdgeMesh->edges() where a.i_face == 0 select m_halfEdgeMesh->getIndex(&a) end;
    {
      timer.start();
      m_queryResults = query();
      ms = timer.elapsed();
      qDebug() << "Mesh Query Time (sec)    :" << float(ms) / 1e3f;
    }
    qDebug() << "--------------------------------------";
    qDebug() << "Mesh Vertexes:" << m_halfEdgeMesh->countVertexes();
    qDebug() << "Mesh Faces:" << m_halfEdgeMesh->countFaces();
    qDebug() << "Mesh HalfEdges:" << m_halfEdgeMesh->countHalfEdges();
    qDebug() << "Boundary Edges:" << m_queryResults.size();
  }
}

#include <QFileDialog>
void Window::openObj()
{
  m_wait = true;
  QString fileName = QFileDialog::getOpenFileName(
        Q_NULLPTR, tr("Open Model"), ".",
        tr("Wavefront Object File (*.obj))"));
  if (!fileName.isNull())
  {
    loadObj(fileName);
  }
  m_wait = false;
}
