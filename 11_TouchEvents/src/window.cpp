#include "window.h"

#include <cmath>

#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLShaderProgram>
#include <OpenGLVertexArrayObject>

#include <KCamera3D>
#include <KDebugDraw>
#include <KInputManager>
#include <KMatrix4x4>
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
 * WindowPrivate
 ******************************************************************************/
class WindowPrivate
{
public:
  // Cached Uniforms
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;

  // Transformations
  KCamera3D m_worldToCamera;
  KTransform3D m_modelToWorld;
  KMatrix4x4 m_cameraToView;

  // OpenGL State Information
  OpenGLBuffer m_vertex;
  OpenGLVertexArrayObject *m_object;
  OpenGLShaderProgram *m_program;
};

/*******************************************************************************
 * Window
 ******************************************************************************/
#define P(c) c &p = *m_private

Window::Window(UpdateBehavior updateBehavior, QWindow *parent) :
  OpenGLWindow(updateBehavior, parent), m_private(new WindowPrivate)
{
  P(WindowPrivate);
  p.m_modelToWorld.scale(50.0f);
  p.m_modelToWorld.translate(0.0f, 0.0f, -150.0f);
}

Window::~Window()
{
  makeCurrent();
  delete m_private;
}

/*******************************************************************************
 * OpenGL Methods
 ******************************************************************************/
void Window::initializeGL()
{
  P(WindowPrivate);
  OpenGLWindow::initializeGL();
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

void Window::resizeGL(int width, int height)
{
  P(WindowPrivate);
  p.m_cameraToView.setToIdentity();
  p.m_cameraToView.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
  OpenGLWindow::resizeGL(width, height);
}

void Window::paintGL()
{
  P(WindowPrivate);

  OpenGLProfiler::BeginFrame();
  {
    OpenGLMarkerScoped _("Total Render Time");
    glClear(GL_COLOR_BUFFER_BIT);
    {
      OpenGLMarkerScoped _("Prepare Scene");
      p.m_program->bind();
      p.m_program->setUniformValue(p.u_worldToCamera, p.m_worldToCamera.toMatrix());
      p.m_program->setUniformValue(p.u_cameraToView, p.m_cameraToView);
    }
    {
      OpenGLMarkerScoped _("Render Scene");
      p.m_object->bind();
      p.m_program->setUniformValue(p.u_modelToWorld, p.m_modelToWorld.toMatrix());
      glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
      p.m_object->release();
    }
    p.m_program->release();
  }
  OpenGLProfiler::EndFrame();

  KDebugDraw::draw(p.m_worldToCamera, p.m_cameraToView);
  OpenGLWindow::paintGL();
}

void Window::teardownGL()
{
  OpenGLWindow::teardownGL();
}

/*******************************************************************************
 * Events
 ******************************************************************************/
void Window::updateEvent(KUpdateEvent *event)
{
  P(WindowPrivate);
  (void)event;

  // Camera Transformation
  if (KInputManager::buttonPressed(Qt::RightButton))
  {
    static const float transSpeed = 0.5f;
    static const float rotSpeed   = 0.5f;

    // Handle rotations
    p.m_worldToCamera.rotate(-rotSpeed * KInputManager::mouseDelta().x(), KCamera3D::LocalUp);
    p.m_worldToCamera.rotate(-rotSpeed * KInputManager::mouseDelta().y(), p.m_worldToCamera.right());

    // Handle translations
    QVector3D translation;
    if (KInputManager::keyPressed(Qt::Key_W))
    {
      translation += p.m_worldToCamera.forward();
    }
    if (KInputManager::keyPressed(Qt::Key_S))
    {
      translation -= p.m_worldToCamera.forward();
    }
    if (KInputManager::keyPressed(Qt::Key_A))
    {
      translation -= p.m_worldToCamera.right();
    }
    if (KInputManager::keyPressed(Qt::Key_D))
    {
      translation += p.m_worldToCamera.right();
    }
    if (KInputManager::keyPressed(Qt::Key_Q))
    {
      translation -= p.m_worldToCamera.up();
    }
    if (KInputManager::keyPressed(Qt::Key_E))
    {
      translation += p.m_worldToCamera.up();
    }
    p.m_worldToCamera.translate(transSpeed * translation);
  }

  // If we are touching with 1 finger, move the object
  static const float scale = 0.16f;
  switch (KInputManager::touchCount())
  {
  case 1:
    {
      qDebug() << "Rotating";

      KTouchPoint touch = KInputManager::touchPoint(0);
      KPointF deltaPos = touch.screenPos() - touch.lastScreenPos();
      KVector3D axis(deltaPos.y(), deltaPos.x(), 0.0f);
      p.m_modelToWorld.rotate(axis.length() * scale, axis.normalized());
    }
    break;
  case 2:
    {
      qDebug() << "Scaling / Translating";

      KTouchPoint first = KInputManager::touchPoint(0);
      KTouchPoint second = KInputManager::touchPoint(1);
      if (first.state() != Qt::TouchPointMoved) return;
      if (first.state() != second.state()) return;

      KPointF firstPos = first.screenPos();
      KPointF firstPosPrime = first.lastScreenPos();
      KPointF secondPos = second.screenPos();
      KPointF secondPosPrime = second.lastScreenPos();
      KPointF delta = secondPos - firstPos;
      KPointF deltaPrime = secondPosPrime - firstPosPrime;
      KPointF sum = secondPos + firstPos;
      KPointF sumPrime = secondPosPrime + firstPosPrime;
      KVector3D deltaVector(delta.x(), delta.y(), 0.0f);
      KVector3D deltaPrimeVector(deltaPrime.x(), deltaPrime.y(), 0.0f);
      KVector3D crossProd = KVector3D::crossProduct(deltaVector, deltaPrimeVector);
      float dotProd = KVector3D::dotProduct(deltaVector, deltaPrimeVector);
      float magnitude = deltaVector.length() * deltaPrimeVector.length();
      float cosAngle = dotProd / magnitude;
      float angle = std::acos(cosAngle) * 2.0f * 180.0f / M_PI;
      if (crossProd.z() < 0.0f)
        angle *= -1.0f;

      float distance = deltaVector.length();
      float distancePrime = deltaPrimeVector.length();
      KVector2D centroid = KVector2D(sum.x(), sum.y()) / 2.0f;
      KVector2D centroidPrime = KVector2D(sumPrime.x(), sumPrime.y()) / 2.0f;

      KVector2D centroidDelta = centroid - centroidPrime;
      p.m_modelToWorld.rotate(angle, 0.0f, 0.0f, 1.0f);
      p.m_modelToWorld.grow((distance - distancePrime) * scale);
      p.m_modelToWorld.translate(centroidDelta.x() * scale, -centroidDelta.y() * scale, 0.0f);
    }
    break;
  default:
    break;
  }
}
