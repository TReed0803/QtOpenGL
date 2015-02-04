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

// Front Verticies
#define VERTEX_FTR KVertex( KVector3D( 0.5f,  0.5f,  0.5f), KVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL KVertex( KVector3D(-0.5f,  0.5f,  0.5f), KVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL KVertex( KVector3D(-0.5f, -0.5f,  0.5f), KVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR KVertex( KVector3D( 0.5f, -0.5f,  0.5f), KVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR KVertex( KVector3D( 0.5f,  0.5f, -0.5f), KVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL KVertex( KVector3D(-0.5f,  0.5f, -0.5f), KVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL KVertex( KVector3D(-0.5f, -0.5f, -0.5f), KVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR KVertex( KVector3D( 0.5f, -0.5f, -0.5f), KVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const KVertex sg_vertexes[] = {
  // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

/*******************************************************************************
 * MainWidgetPrivate
 ******************************************************************************/
class MainWidgetPrivate
{
public:
  // Cached Uniforms
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;

  // Transformations
  KCamera3D m_camera;
  KTransform3D m_transform;
  KMatrix4x4 m_projection;

  // OpenGL State Information
  OpenGLBuffer m_vertex;
  OpenGLVertexArrayObject *m_object;
  OpenGLShaderProgram *m_program;

  // Touch Information
  float m_dragVelocity;
  KVector3D m_dragAxis;
};

/*******************************************************************************
 * MainWidget
 ******************************************************************************/
#define P(c) c &p = *m_private

MainWidget::MainWidget(QWidget *parent) :
  OpenGLWidget(parent), m_private(new MainWidgetPrivate)
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

    // Create Vertex Array Object
    p.m_object = new OpenGLVertexArrayObject(this);
    p.m_object->create();
    p.m_object->bind();

    // Setup VAO Information
    p.m_vertex.create();
    p.m_vertex.bind();
    p.m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    p.m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));
    p.m_program->enableAttributeArray(0);
    p.m_program->enableAttributeArray(1);
    p.m_program->setAttributeBuffer(0, GL_FLOAT, KVertex::positionOffset(), KVertex::PositionTupleSize, KVertex::stride());
    p.m_program->setAttributeBuffer(1, GL_FLOAT, KVertex::colorOffset(), KVertex::ColorTupleSize, KVertex::stride());

    // Release (unbind) all
    p.m_object->release();
    p.m_program->release();
  }
}

void MainWidget::resizeGL(int width, int height)
{
  P(MainWidgetPrivate);
  p.m_projection.setToIdentity();
  p.m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
  OpenGLWidget::resizeGL(width, height);
}

void MainWidget::paintGL()
{
  P(MainWidgetPrivate);

  OpenGLProfiler::BeginFrame();
  {
    OpenGLMarkerScoped _("Total Render Time");
    glClear(GL_COLOR_BUFFER_BIT);
    {
      OpenGLMarkerScoped _("Prepare Scene");
      p.m_program->bind();
      p.m_program->setUniformValue(p.u_worldToCamera, p.m_camera.toMatrix());
      p.m_program->setUniformValue(p.u_cameraToView, p.m_projection);
    }
    {
      OpenGLMarkerScoped _("Render Scene");
      p.m_object->bind();
      p.m_program->setUniformValue(p.u_modelToWorld, p.m_transform.toMatrix());
      glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
      p.m_object->release();
    }
    p.m_program->release();
  }
  OpenGLProfiler::EndFrame();

  KDebugDraw::draw(p.m_camera, p.m_projection);
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
