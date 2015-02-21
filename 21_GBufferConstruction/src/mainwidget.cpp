#include "mainwidget.h"

#include <cmath>

#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLInstanceGroup>
#include <OpenGLShaderProgram>
#include <OpenGLUniformBufferObject>
#include <OpenGLVertexArrayObject>
#include <OpenGLFramebufferObject>
#include <OpenGLRenderbufferObject>
#include <OpenGLTexture>
#include <OpenGLInstance>
#include <OpenGLMaterial>

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

static uint64_t sg_count = 0;
#define DEFERRED_TEXTURES 3

enum DeferredData
{
  DepthData,
  LinearDepthData,
  PositionData,
  NormalData,
  DiffuseData,
  SpecularData,
  VelocityData,
  DeferredDataCount
};

/*******************************************************************************
 * MainWidgetPrivate
 ******************************************************************************/
class MainWidgetPrivate : protected OpenGLFunctions
{
public:
  MainWidgetPrivate(MainWidget *parent);
  void initializeGL();
  void loadObj(const QString &fileName);
  void openObj();
  void drawBoundaries();
  void updateBackbuffer(int w, int h);
  void drawBackbuffer();

  // Transformations
  KCamera3D m_camera;
  KCamera3D m_cameraPrev;
  KTransform3D m_transform;
  KMatrix4x4 m_projection;

  // OpenGL State Information
  int m_width;
  int m_height;
  OpenGLMesh *m_openGLMesh;
  KHalfEdgeMesh *m_halfEdgeMesh;
  KHalfEdgeMesh *m_quad;
  OpenGLMesh *m_quadGL;
  typedef std::tuple<KVector3D,KVector3D> QueryResultType;
  std::vector<QueryResultType> m_boundaries;
  OpenGLShaderProgram *m_program;
  OpenGLShaderProgram *m_textureDrawer;
  OpenGLUniformBufferObject m_matrixBlock;
  OpenGLInstanceGroup *m_instanceGroup;

  // GBuffer
  DeferredData m_buffer;
  OpenGLShaderProgram *m_deferredPrograms[DeferredDataCount];
  OpenGLRenderbufferObject *m_depthBuffer;
  OpenGLFramebufferObject *m_deferredBuffer;
  OpenGLTexture *m_deferredTextures[DEFERRED_TEXTURES];
  OpenGLTexture *m_backBuffer;
  std::vector<OpenGLInstance*> m_instances;

  // Touch Information
  float m_dragVelocity;
  KVector3D m_dragAxis;

  // Parent
  MainWidget *m_parent;
};

MainWidgetPrivate::MainWidgetPrivate(MainWidget *parent) :
  m_openGLMesh(Q_NULLPTR), m_halfEdgeMesh(Q_NULLPTR),
  m_program(Q_NULLPTR), m_parent(parent), m_width(1), m_height(1),
  m_buffer(PositionData)
{
  m_depthBuffer = 0;
  m_deferredBuffer = 0;
  for (int i = 0; i < DEFERRED_TEXTURES; ++i)
    m_deferredTextures[i] = 0;
  m_backBuffer = 0;
}

void MainWidgetPrivate::initializeGL()
{
  initializeOpenGLFunctions();
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
      m_instanceGroup->setMesh(m_openGLMesh);
      ms = timer.elapsed();
      qDebug() << "Create OpenGLMesh (sec)  :" << float(ms) / 1e3f;
    }
    auto query =
      SELECT
        FROM ( edge : m_halfEdgeMesh->halfEdges() )
        WHERE ( edge.face == 0 )
        JOIN ( m_halfEdgeMesh->vertex(edge.to)->position,
               m_halfEdgeMesh->vertex(m_halfEdgeMesh->halfEdge(edge.next)->to)->position );
    {
      timer.start();
      m_boundaries = query();
      ms = timer.elapsed();
      qDebug() << "Mesh Query Time (sec)    :" << float(ms) / 1e3f;
    }
    qDebug() << "--------------------------------------";
    qDebug() << "Mesh Vertexes  :" << m_halfEdgeMesh->vertices().size();
    qDebug() << "Mesh Faces     :" << m_halfEdgeMesh->faces().size();
    qDebug() << "Mesh HalfEdges :" << m_halfEdgeMesh->halfEdges().size();
    qDebug() << "Boundary Edges :" << m_boundaries.size();
    qDebug() << "Polygons /Frame:" << m_halfEdgeMesh->faces().size() * sg_count;
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
    QVector3D origin = modelToView * std::get<0>(line);
    QVector3D to   = modelToView * std::get<1>(line);

    // If it's outside of the visible realm, don't send it to the GPU
    if (origin.x() > 1.0 || origin.x() < -1.0f || origin.y() > 1.0 || origin.y() < -1.0f)
      if  (to.x() > 1.0 || to.x() < -1.0f || to.y() > 1.0 || to.y() < -1.0f)
        continue;

    KDebugDraw::World::drawLine(origin, to, Qt::red);
  }
}

void MainWidgetPrivate::updateBackbuffer(int w, int h)
{
  m_width = w;
  m_height = h;

  // GBuffer Texture Storage
  for (int i = 0; i < DEFERRED_TEXTURES; ++i)
  {
    OpenGLTexture *&currTexture = m_deferredTextures[i];
    delete currTexture;
    currTexture = new OpenGLTexture();
    currTexture->create(OpenGLTexture::Texture2D);
    currTexture->bind();
    currTexture->setStorage((OpenGLStorage)GL_RGBA32F);
    currTexture->setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
    currTexture->setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
    currTexture->setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
    currTexture->setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
    currTexture->setSize(m_width, m_height);
    currTexture->allocate();
    currTexture->release();
  }

  // Backbuffer Texture
  delete m_backBuffer;
  m_backBuffer = new OpenGLTexture();
  m_backBuffer->create(OpenGLTexture::Texture2D);
  m_backBuffer->bind();
  m_backBuffer->setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  m_backBuffer->setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  m_backBuffer->setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  m_backBuffer->setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  m_backBuffer->setSize(m_width, m_height);
  m_backBuffer->allocate();
  m_backBuffer->release();

  // Depth Renderbuffer
  delete m_depthBuffer;
  m_depthBuffer = new OpenGLRenderbufferObject();
  m_depthBuffer->create();
  m_depthBuffer->bind();
  m_depthBuffer->setStorage(OpenGLStorage::Depth16);
  m_depthBuffer->setSize(m_width, m_height);
  m_depthBuffer->allocate();
  m_depthBuffer->release();

  // GBuffer Framebuffer
  m_deferredBuffer->bind();
  m_deferredBuffer->attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, *m_backBuffer);
  m_deferredBuffer->attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment1, *m_deferredTextures[0]);
  m_deferredBuffer->attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment2, *m_deferredTextures[1]);
  m_deferredBuffer->attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment3, *m_deferredTextures[2]);
  m_deferredBuffer->attachRenderbuffer(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::DepthAttachment, *m_depthBuffer);
  m_deferredBuffer->drawBuffers(OpenGLFramebufferObject::ColorAttachment0, OpenGLFramebufferObject::ColorAttachment1, OpenGLFramebufferObject::ColorAttachment2, OpenGLFramebufferObject::ColorAttachment3);

  // Check Framebuffer validity
  switch(m_deferredBuffer->status())
  {
  case OpenGLFramebufferObject::Complete:
    break;
  case OpenGLFramebufferObject::IncompleteAttachment:
    qFatal("Incomplete Attachment");
    break;
  case OpenGLFramebufferObject::IncompleteMissingAttachment:
    qFatal("Incomplete Missing Attachment");
    break;
  case OpenGLFramebufferObject::IncompleteDrawBuffer:
    qFatal("Incomplete Draw Buffer");
    break;
  case OpenGLFramebufferObject::IncompleteReadBuffer:
    qFatal("Incomplete Read Buffer");
    break;
  case OpenGLFramebufferObject::Unsupported:
    qFatal("Unsupported");
    break;
  }

  m_deferredBuffer->release();
}

void MainWidgetPrivate::drawBackbuffer()
{
  OpenGLMarkerScoped _("Present G Buffer");
  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  m_deferredTextures[0]->bind();
  glActiveTexture(GL_TEXTURE1);
  m_deferredTextures[1]->bind();
  glActiveTexture(GL_TEXTURE2);
  m_deferredTextures[2]->bind();
  m_deferredPrograms[m_buffer]->bind();
  m_quadGL->draw();
  m_deferredPrograms[m_buffer]->release();
  m_backBuffer->release();
  glEnable(GL_DEPTH_TEST);
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

static GLuint buffers[1];

/*******************************************************************************
 * OpenGL Methods
 ******************************************************************************/
void MainWidget::initializeGL()
{
  P(MainWidgetPrivate);
  p.initializeGL();
  OpenGLWidget::initializeGL();
  printVersionInformation();

  // Set global information
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  p.m_quad = new KHalfEdgeMesh(this, ":/quad.obj");
  p.m_quadGL = p.m_quad->createOpenGLMesh(OpenGLMesh::Contiguous);

  // Application-specific initialization
  {
    // Uniform Block Object
    p.m_matrixBlock.create();
    p.m_matrixBlock.bind(1);
    p.m_matrixBlock.setUsagePattern(OpenGLBuffer::DynamicDraw);
    p.m_matrixBlock.allocate(sizeof(GLfloat) * 16 * 10); // 6 * mat4

    // Create Shader (Do not release until VAO is created)
    p.m_program = new OpenGLShaderProgram(this);
    p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/gbuffer.vert");
    p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/gbuffer.frag");
    p.m_program->link();
    p.m_program->bind();
    p.m_program->uniformBlockBinding("GlobalMatrices", p.m_matrixBlock);
    p.m_program->release();

    char const* fragFiles[DeferredDataCount] = {
      ":/shaders/gbuffer/depth.frag",
      ":/shaders/gbuffer/linearDepth.frag",
      ":/shaders/gbuffer/position.frag",
      ":/shaders/gbuffer/normal.frag",
      ":/shaders/gbuffer/diffuse.frag",
      ":/shaders/gbuffer/specular.frag",
      ":/shaders/gbuffer/velocity.frag"
    };
    for (int i = 0; i < DeferredDataCount; ++i)
    {
      p.m_deferredPrograms[i] = new OpenGLShaderProgram(this);
      p.m_deferredPrograms[i]->addIncludePath(":/shaders");
      p.m_deferredPrograms[i]->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/gbuffer/main.vert");
      p.m_deferredPrograms[i]->addShaderFromSourceFile(QOpenGLShader::Fragment, fragFiles[i]);
      p.m_deferredPrograms[i]->link();
      p.m_deferredPrograms[i]->bind();
      p.m_deferredPrograms[i]->setUniformValue("geometryTexture", 0);
      p.m_deferredPrograms[i]->setUniformValue("materialTexture", 1);
      p.m_deferredPrograms[i]->setUniformValue("dynamicsTexture", 2);
      p.m_deferredPrograms[i]->uniformBlockBinding("GlobalMatrices", p.m_matrixBlock);
      p.m_deferredPrograms[i]->release();
    }

    // Framebuffer Object
    p.m_deferredBuffer = new OpenGLFramebufferObject;
    p.m_deferredBuffer->create();

    // Open OBJ
    p.m_instanceGroup = new OpenGLInstanceGroup(this);
    p.loadObj(":/objects/sphere.obj");

    // Initialize instances
    for (int level = 0; level < 1; ++level)
    {
      for (int deg = 0; deg < 360; deg += 10)
      {
        float cosine = std::cos(deg * 3.14159f / 180.0f);
        float sine = std::sin(deg * 3.14159f / 180.0f);
        OpenGLInstance * instance = p.m_instanceGroup->createInstance();
        instance->material().setDiffuse(deg / 360.0f, 1.0f - deg / 360.0f, 0.0f);
        instance->material().setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
        instance->currentTransform().setTranslation(cosine * 15, level * 5, sine * 15);
        p.m_instances.push_back(instance);
        ++sg_count;
      }
    }
    qDebug() << "Instances: " << sg_count;

    // Release (unbind) all
    p.m_program->release();
  }
}

void MainWidget::resizeGL(int width, int height)
{
  P(MainWidgetPrivate);
  p.m_projection.setToIdentity();
  p.m_projection.perspective(45.0f, width / float(height), 0.1f, 1000.0f);
  p.updateBackbuffer(width, height);
  OpenGLWidget::resizeGL(width, height);
}

void MainWidget::paintGL()
{
  P(MainWidgetPrivate);

  OpenGLProfiler::BeginFrame();
  {
    OpenGLMarkerScoped _("Total Render Time");
    p.m_program->bind();
    {
      OpenGLMarkerScoped _("Prepare Scene");
      p.m_matrixBlock.write(0, p.m_camera.toMatrix().constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16, p.m_projection.constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 2, (p.m_projection * p.m_camera.toMatrix()).constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 3, p.m_camera.toMatrix().inverted().constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 4, p.m_projection.inverted().constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 5, (p.m_projection * p.m_camera.toMatrix()).inverted().constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 6,  p.m_cameraPrev.toMatrix().constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 7,  (p.m_projection * p.m_cameraPrev.toMatrix()).constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 8, p.m_cameraPrev.toMatrix().inverted().constData(), sizeof(float) * 16);
      p.m_matrixBlock.write(sizeof(float) * 16 * 9, (p.m_projection * p.m_cameraPrev.toMatrix()).inverted().constData(), sizeof(float) * 16);
      p.m_instanceGroup->update();
    }
    {
      OpenGLMarkerScoped _("Generate G Buffer");
      p.m_deferredBuffer->bind();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      p.m_instanceGroup->draw();
      glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    }
    p.m_program->release();
    p.drawBackbuffer();
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
  p.m_cameraPrev = p.m_camera;

  // Update instances
  float angle = 0.0f;
  for (OpenGLInstance *instance : p.m_instances)
  {
    instance->currentTransform().rotate(angle, 0.0f, 0.0, 1.0f);
    angle += 1.0f;
  }

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

  // Change Buffer
  if (KInputManager::keyPressed(Qt::Key_0))
  {
    p.m_buffer = (DeferredData)0;
  }
  if (KInputManager::keyPressed(Qt::Key_1))
  {
    p.m_buffer = (DeferredData)0;
  }
  if (KInputManager::keyPressed(Qt::Key_2))
  {
    p.m_buffer = (DeferredData)1;
  }
  if (KInputManager::keyPressed(Qt::Key_3))
  {
    p.m_buffer = (DeferredData)2;
  }
  if (KInputManager::keyPressed(Qt::Key_4))
  {
    p.m_buffer = (DeferredData)3;
  }
  if (KInputManager::keyPressed(Qt::Key_5))
  {
    p.m_buffer = (DeferredData)4;
  }
  if (KInputManager::keyPressed(Qt::Key_6))
  {
    p.m_buffer = (DeferredData)5;
  }
  if (KInputManager::keyPressed(Qt::Key_7))
  {
    p.m_buffer = (DeferredData)6;
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
