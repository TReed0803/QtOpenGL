#include "mainwidget.h"

#include <cmath>

#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLShaderProgram>
#include <OpenGLVertexArrayObject>

#include <KCamera3D>
#include <KDebugDraw>
#include <KInputManager>
#include <KMatrix4x4>
#include <KPanGesture>
#include <KPinchGesture>
#include <KPointF>
#include <KTransform3D>
#include <KVector2D>
#include <KVertex>

#include <Qt>
#include <QElapsedTimer>
#include <QFileDialog>
#include <OpenGLMesh>
#include <KHalfEdgeMesh>
#include <KLinq>


/*******************************************************************************
 * MainWidgetPrivate
 ******************************************************************************/
class MainWidgetPrivate
{
public:
  MainWidgetPrivate(MainWidget *parent);
  void loadObj(const QString &fileName);
  void openObj();
  void drawBoundaries();

  // Cached Uniforms
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;

  // Transformations
  KCamera3D m_camera;
  KTransform3D m_transform;
  KMatrix4x4 m_projection;

  // OpenGL State Information
  OpenGLMesh *m_openGLMesh;
  KHalfEdgeMesh *m_halfEdgeMesh;
  typedef std::tuple<KHalfEdgeMesh::VertexIndex,KHalfEdgeMesh::VertexIndex> QueryResultType;
  std::vector<QueryResultType> m_boundaries;
  OpenGLShaderProgram *m_program;

  // Touch Information
  float m_dragVelocity;
  KVector3D m_dragAxis;

  // Parent
  MainWidget *m_parent;
};

MainWidgetPrivate::MainWidgetPrivate(MainWidget *parent) :
  m_openGLMesh(Q_NULLPTR), m_halfEdgeMesh(Q_NULLPTR),
  m_program(Q_NULLPTR), m_parent(parent)
{
  // Intentionally Empty
}

void MainWidgetPrivate::loadObj(const QString &fileName)
{
  // Remove old mesh
  delete m_openGLMesh;
  delete m_halfEdgeMesh;
  m_boundaries.clear();

  // Initialize an object
  quint64 ms;
  QElapsedTimer timer;
  {
    {
      timer.start();
      m_halfEdgeMesh = new KHalfEdgeMesh(m_parent, fileName);
      ms = timer.elapsed();
      qDebug() << "Create HalfEdgeMesh (sec):" << float(ms) / 1e3f;
    }
    {
      m_parent->makeCurrent();
      timer.start();
      m_openGLMesh = m_halfEdgeMesh->createOpenGLMesh(OpenGLMesh::Contiguous | OpenGLMesh::Interleaved);
      ms = timer.elapsed();
      qDebug() << "Create OpenGLMesh (sec)  :" << float(ms) / 1e3f;
    }
    auto query =
      SELECT
        FROM ( edge : m_halfEdgeMesh->edges() )
        WHERE ( edge.a.face == 0 || edge.b.face == 0 )
        JOIN ( edge.a.to, edge.b.to );
    {
      timer.start();
      m_boundaries = query();
      ms = timer.elapsed();
      qDebug() << "Mesh Query Time (sec)    :" << float(ms) / 1e3f;
    }
    qDebug() << "--------------------------------------";
    qDebug() << "Mesh Vertexes:" << m_halfEdgeMesh->vertices().size();
    qDebug() << "Mesh Faces:" << m_halfEdgeMesh->faces().size();
    qDebug() << "Mesh Edges:" << m_halfEdgeMesh->edges().size();
    qDebug() << "Mesh HalfEdges:" << m_halfEdgeMesh->halfEdges().size();
    qDebug() << "Boundary Edges:" << m_boundaries.size();
  }
}

void MainWidgetPrivate::openObj()
{
  QString fileName = QFileDialog::getOpenFileName(
    m_parent, m_parent->tr("Open Model"),
    ".",
    m_parent->tr("Wavefront Object File (*.obj))")
  );
  if (!fileName.isNull())
  {
    loadObj(fileName);
  }
}

void MainWidgetPrivate::drawBoundaries()
{
  KMatrix4x4 modelToView = m_projection * m_camera.toMatrix() * m_transform.toMatrix();
  for (QueryResultType const &line : m_boundaries)
  {
    QVector3D origin = modelToView * m_halfEdgeMesh->vertex(std::get<0>(line))->position;
    QVector3D to   = modelToView * m_halfEdgeMesh->vertex(std::get<1>(line))->position;

    // If it's outside of the visible realm, don't send it to the GPU
    if (origin.x() > 1.0 || origin.x() < -1.0f || origin.y() > 1.0 || origin.y() < -1.0f)
      if  (to.x() > 1.0 || to.x() < -1.0f || to.y() > 1.0 || to.y() < -1.0f)
        continue;

    KDebugDraw::World::drawLine(origin, to, Qt::red);
  }
}


/*******************************************************************************
 * MainWidget
 ******************************************************************************/
#define P(c) c &p = *m_private

MainWidget::MainWidget(QWidget *parent) :
  OpenGLWidget(parent), m_private(new MainWidgetPrivate(this))
{
  P(MainWidgetPrivate);
  p.m_transform.scale(50.0f);
  p.m_transform.translate(0.0f, 0.0f, -150.0f);
  p.m_dragVelocity = 0.0f;
}

MainWidget::~MainWidget()
{
  makeCurrent();
  delete m_private;
}

/*******************************************************************************
 * OpenGL Methods
 ******************************************************************************/
void MainWidget::initializeGL()
{
  P(MainWidgetPrivate);
  OpenGLWidget::initializeGL();
  printVersionInformation();

  // Set global information
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glLineWidth(10.0f);
  glClearDepthf(1.0f);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    p.m_program = new OpenGLShaderProgram(this);
    p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    p.m_program->link();
    p.m_program->bind();

    // Cache Uniform Locations
    p.u_modelToWorld = p.m_program->uniformLocation("modelToWorld");
    p.u_worldToCamera = p.m_program->uniformLocation("worldToCamera");
    p.u_cameraToView = p.m_program->uniformLocation("cameraToView");

    // Open OBJ
    p.loadObj(":/objects/teapot.obj");

    // Release (unbind) all
    p.m_program->release();
  }
}

void MainWidget::resizeGL(int width, int height)
{
  P(MainWidgetPrivate);
  p.m_projection.setToIdentity();
  p.m_projection.perspective(45.0f, width / float(height), 0.1f, 1000.0f);
  OpenGLWidget::resizeGL(width, height);
}

void MainWidget::paintGL()
{
  P(MainWidgetPrivate);

  OpenGLProfiler::BeginFrame();
  {
    OpenGLMarkerScoped _("Total Render Time");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
      OpenGLMarkerScoped _("Prepare Scene");
      p.m_program->bind();
      p.m_program->setUniformValue(p.u_worldToCamera, p.m_camera.toMatrix());
      p.m_program->setUniformValue(p.u_cameraToView, p.m_projection);
    }
    {
      OpenGLMarkerScoped _("Render Scene");
      p.m_program->setUniformValue(p.u_modelToWorld, p.m_transform.toMatrix());
      p.m_openGLMesh->draw();
      p.drawBoundaries();
    }
    p.m_program->release();
  }
  OpenGLProfiler::EndFrame();

  KDebugDraw::draw();
  OpenGLWidget::paintGL();
}

void MainWidget::teardownGL()
{
  OpenGLWidget::teardownGL();
}

/*******************************************************************************
 * Events
 ******************************************************************************/
void MainWidget::updateEvent(KUpdateEvent *event)
{
  P(MainWidgetPrivate);
  (void)event;

  // Camera Transformation
  if (KInputManager::buttonPressed(Qt::RightButton))
  {
    float transSpeed = 3.0f;
    float rotSpeed   = 0.5f;

    if (KInputManager::keyPressed(Qt::Key_Control))
    {
      transSpeed = 1.0f;
    }

    // Handle rotations
    p.m_camera.rotate(-rotSpeed * KInputManager::mouseDelta().x(), KCamera3D::LocalUp);
    p.m_camera.rotate(-rotSpeed * KInputManager::mouseDelta().y(), p.m_camera.right());

    // Handle translations
    QVector3D translation;
    if (KInputManager::keyPressed(Qt::Key_W))
    {
      translation += p.m_camera.forward();
    }
    if (KInputManager::keyPressed(Qt::Key_S))
    {
      translation -= p.m_camera.forward();
    }
    if (KInputManager::keyPressed(Qt::Key_A))
    {
      translation -= p.m_camera.right();
    }
    if (KInputManager::keyPressed(Qt::Key_D))
    {
      translation += p.m_camera.right();
    }
    if (KInputManager::keyPressed(Qt::Key_E))
    {
      translation -= p.m_camera.up();
    }
    if (KInputManager::keyPressed(Qt::Key_Q))
    {
      translation += p.m_camera.up();
    }
    p.m_camera.translate(transSpeed * translation);
  }

  if (KInputManager::keyPressed(Qt::Key_Control))
  {
    if (KInputManager::keyTriggered(Qt::Key_O))
    {
      p.openObj();
    }
  }

  // Pinching will grow/shrink
  KPinchGesture pinch;
  if (KInputManager::pinchGesture(&pinch))
  {
    p.m_transform.scale(pinch.scaleFactor());
    p.m_transform.rotate(pinch.lastRotationAngle() - pinch.rotationAngle(), 0.0f, 0.0f, 1.0f);
  }

  // Panning will translate
  KPanGesture pan;
  if (KInputManager::panGesture(&pan))
  {
    KVector3D delta = KVector3D(pan.delta().x(), -pan.delta().y(), 0.0f) * 0.1f;
    p.m_transform.translate(delta);
  }

  // Touching will rotate
  if (KInputManager::touchCount() == 1)
  {
    KTouchPoint touch = KInputManager::touchPoint(0);
    KPointF delta = touch.pos() - touch.lastPos();
    KVector3D axis(delta.y(), delta.x(), 0.0f);
    switch (touch.state())
    {
    case Qt::TouchPointPressed:
      p.m_dragVelocity = 0.0f;
      break;
    case Qt::TouchPointMoved:
      p.m_dragAxis = p.m_camera.rotation().rotatedVector(axis);
      p.m_dragVelocity = axis.length() * 0.1f;
      p.m_dragAxis.normalize();
      break;
    default:
      break;
    }
  }

  // Rotate from drag gesture
  p.m_dragVelocity *= 0.9f;
  p.m_transform.rotate(p.m_dragVelocity, p.m_dragAxis);
}
