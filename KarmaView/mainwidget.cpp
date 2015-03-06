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
#include <OpenGLPointLightGroup>
#include <OpenGLSpotLightGroup>
#include <OpenGLDirectionLightGroup>

#include <KCamera3D>
#include <OpenGLDebugDraw>
#include <KInputManager>
#include <KMatrix4x4>
#include <KPanGesture>
#include <KPinchGesture>
#include <KPointF>
#include <KTransform3D>
#include <KVector2D>
#include <KVertex>
#include <KStaticGeometry>

// Bounding Volumes
#include <KAabbBoundingVolume>
#include <KSphereBoundingVolume>
#include <KEllipsoidBoundingVolume>
#include <KOrientedBoundingVolume>

#include <Qt>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QOpenGLFramebufferObject>
#include <OpenGLMesh>
#include <KHalfEdgeMesh>
#include <KLinq>
#include <OpenGLUniformBufferManager>
#include <QMainWindow>
#include <QApplication>

static uint64_t sg_count = 0;
#define DEFERRED_TEXTURES 4

enum DeferredData
{
  DepthData,
  LinearDepthData,
  PositionData,
  NormalData,
  DiffuseData,
  SpecularData,
  VelocityData,
  AmbientPass,
  MotionBlurPass,
  LightPass,
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
  void constructDeferredTexture(OpenGLTexture &t, OpenGLInternalFormat f);
  void checkFramebuffer(char const *name, OpenGLFramebufferObject &fbo);

  // Transformations
  KCamera3D m_camera;
  KCamera3D m_cameraPrev;
  KTransform3D m_transform;
  KMatrix4x4 m_projection;
  float m_depthFar;
  float m_depthNear;
  float m_depthDiff;

  // OpenGL State Information
  float m_width;
  float m_height;
  OpenGLMesh m_openGLMesh;
  KHalfEdgeMesh *m_halfEdgeMesh;
  KHalfEdgeMesh *m_quad;
  KHalfEdgeMesh *m_floor;
  OpenGLMesh m_quadGL;
  OpenGLMesh m_floorGL;
  typedef std::tuple<KVector3D,KVector3D> QueryResultType;
  std::vector<QueryResultType> m_boundaries;
  OpenGLShaderProgram *m_program;
  OpenGLShaderProgram *m_textureDrawer;
  OpenGLUniformBufferObject m_matrixBlock;
  OpenGLInstanceGroup m_instanceGroup;
  OpenGLInstanceGroup m_floorGroup;
  OpenGLInstance *m_floorInstance;
  OpenGLShaderProgram *m_pointLightProgram;
  OpenGLShaderProgram *m_directionLightProgram;
  OpenGLShaderProgram *m_spotLightProgram;
  OpenGLPointLightGroup m_pointLightGroup;
  OpenGLDirectionLightGroup m_directionLightGroup;
  OpenGLSpotLightGroup m_spotLightGroup;
  bool m_paused;
  DeferredData m_buffer;
  OpenGLShaderProgram *m_ambientProgram;
  OpenGLShaderProgram *m_deferredPrograms[DeferredDataCount];

  // Bounding Volumes
  KAabbBoundingVolume *m_aabbBV;
  KSphereBoundingVolume *m_sphereCentroidBV;
  KSphereBoundingVolume *m_sphereRittersBV;
  KSphereBoundingVolume *m_sphereLarssonsBV;
  KSphereBoundingVolume *m_spherePcaBV;
  KEllipsoidBoundingVolume *m_ellipsoidPcaBV;
  KOrientedBoundingVolume *m_orientedPcaBV;
  KStaticGeometry *m_staticGeometryBottomUp7;
  KStaticGeometry *m_staticGeometryBottomUp500;
  KStaticGeometry *m_staticGeometryTopDown7;
  KStaticGeometry *m_staticGeometryTopDown500;
  KStaticGeometry *m_staticGeometry;

  // GBuffer
  OpenGLTexture m_gDepth;    // depth
  OpenGLTexture m_gGeometry; // normal normal vel vel
  OpenGLTexture m_gMaterial; // diff diff diff spec
  OpenGLTexture m_gSurface;  // exp
  OpenGLFramebufferObject m_deferredBuffer;

  // Light Accumulation
  OpenGLTexture m_gLighting;
  OpenGLFramebufferObject m_lightBuffer;

  std::vector<OpenGLInstance*> m_instances;
  float m_ambientColor[4];
  float m_atmosphericColor[4];

  // Touch Information
  float m_dragVelocity;
  KVector3D m_dragAxis;

  // Runtime
  bool b_rX, b_rY, b_rZ;
  bool b_bv[8];
  int m_minDraw, m_maxDraw;

  // Parent
  MainWidget *m_parent;
};

MainWidgetPrivate::MainWidgetPrivate(MainWidget *parent) :
  m_halfEdgeMesh(Q_NULLPTR),
  m_program(Q_NULLPTR), m_parent(parent), m_width(1), m_height(1),
  m_buffer(LightPass), m_paused(false), m_staticGeometry(0), m_minDraw(0), m_maxDraw(std::numeric_limits<size_t>::max())
{
  m_ambientColor[0] = m_ambientColor[1] = m_ambientColor[2] = 0.2f;
  m_ambientColor[3] = 1.0f;
  m_atmosphericColor[0] = m_atmosphericColor[1] = m_atmosphericColor[2] = 0.0f;
  m_atmosphericColor[3] = 1.0f;
  m_camera.setTranslation(0.0f, 3.0f, 10.0f);
  m_camera.setRotation(-20.0f, 1.0f, 0.0f, 0.0f);
  b_rX = b_rY = b_rZ = false;
  for (int i = 0; i < 8; ++i)
    b_bv[i] = false;
  m_staticGeometryBottomUp7 = m_staticGeometryBottomUp500 = m_staticGeometryTopDown7 = m_staticGeometryTopDown500 = 0;
}

void MainWidgetPrivate::initializeGL()
{
  initializeOpenGLFunctions();
}

void MainWidgetPrivate::loadObj(const QString &fileName)
{
  // Remove old mesh
  bool oldValue = m_paused;
  m_paused = true;
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
      qDebug() << "Create HalfEdgeMesh (sec)    :" << float(ms) / 1e3f;
    }
    {
      timer.start();
      m_halfEdgeMesh->calculateVertexNormals();
      ms = timer.elapsed();
      qDebug() << "Calculate Normals (sec)      :" << float(ms) / 1e3f;
    }
    {
      timer.start();
      m_aabbBV = new KAabbBoundingVolume(*m_halfEdgeMesh, KAabbBoundingVolume::MinMaxMethod);
      m_sphereCentroidBV = new KSphereBoundingVolume(*m_halfEdgeMesh, KSphereBoundingVolume::CentroidMethod);
      m_sphereRittersBV = new KSphereBoundingVolume(*m_halfEdgeMesh, KSphereBoundingVolume::RittersMethod);
      m_sphereLarssonsBV = new KSphereBoundingVolume(*m_halfEdgeMesh, KSphereBoundingVolume::LarssonsMethod);
      m_spherePcaBV = new KSphereBoundingVolume(*m_halfEdgeMesh, KSphereBoundingVolume::PcaMethod);
      m_ellipsoidPcaBV = new KEllipsoidBoundingVolume(*m_halfEdgeMesh, KEllipsoidBoundingVolume::PcaMethod);
      m_orientedPcaBV = new KOrientedBoundingVolume(*m_halfEdgeMesh, KOrientedBoundingVolume::PcaMethod);
      ms = timer.elapsed();
      qDebug() << "Create Bounding Volumes (sec):" << float(ms) / 1e3f;
    }
    {
      m_parent->makeCurrent();
      timer.start();
      m_openGLMesh.create(*m_halfEdgeMesh);
      m_instanceGroup.setMesh(m_openGLMesh);
      ms = timer.elapsed();
      qDebug() << "Create OpenGLMesh (sec)      :" << float(ms) / 1e3f;
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
      qDebug() << "Mesh Query Time (sec)        :" << float(ms) / 1e3f;
    }
    {
      delete m_staticGeometryTopDown500;
      delete m_staticGeometryTopDown7;
      delete m_staticGeometryBottomUp7;
      delete m_staticGeometryBottomUp500;
      m_staticGeometryTopDown500 = new KStaticGeometry();
      m_staticGeometryTopDown7 = new KStaticGeometry();
      m_staticGeometryBottomUp7 = new KStaticGeometry();
      m_staticGeometryBottomUp500 = new KStaticGeometry();
      KTransform3D geomTrans;
      for (int i = 0; i < 4; ++i)
      {
        const float radius = 10.0f;
        float radians = i * Karma::TwoPi / 4.0f;
        geomTrans.setTranslation(std::cos(radians) * radius, 0.0f, std::sin(radians) * radius);
        m_staticGeometryTopDown500->addGeometry(*m_halfEdgeMesh, geomTrans);
        m_staticGeometryTopDown7->addGeometry(*m_halfEdgeMesh, geomTrans);
        m_staticGeometryBottomUp7->addGeometry(*m_halfEdgeMesh, geomTrans);
        m_staticGeometryBottomUp500->addGeometry(*m_halfEdgeMesh, geomTrans);
      }
      m_staticGeometry = 0;
    }
    qDebug() << "--------------------------------------";
    qDebug() << "Mesh Vertexes  :" << m_halfEdgeMesh->vertices().size();
    qDebug() << "Mesh Faces     :" << m_halfEdgeMesh->faces().size();
    qDebug() << "Mesh HalfEdges :" << m_halfEdgeMesh->halfEdges().size();
    qDebug() << "Boundary Edges :" << m_boundaries.size();
    qDebug() << "Polygons /Frame:" << m_halfEdgeMesh->faces().size() * sg_count;
  }

  m_paused = oldValue;
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
  KMatrix4x4 const &modelToWorld = m_transform.toMatrix();
  for (QueryResultType const &line : m_boundaries)
  {
    QVector3D origin = modelToWorld * std::get<0>(line);
    QVector3D to     = modelToWorld * std::get<1>(line);
    OpenGLDebugDraw::World::drawLine(origin, to, Qt::red);
  }
}

void MainWidgetPrivate::updateBackbuffer(int w, int h)
{
  m_width = w;
  m_height = h;

  // GBuffer Texture Storage
  constructDeferredTexture(m_gDepth, OpenGLInternalFormat::Depth32F);   // Depth
  constructDeferredTexture(m_gGeometry, OpenGLInternalFormat::Rgba32F); // Normal Normal Velocity Velocity
  constructDeferredTexture(m_gMaterial, OpenGLInternalFormat::Rgba8);   // Diffuse Diffuse Diffuse SpecularColor
  constructDeferredTexture(m_gSurface, OpenGLInternalFormat::R8);       // SpecularExp

  // Other Texture Storage
  constructDeferredTexture(m_gLighting, OpenGLInternalFormat::Rgba16);

  // GBuffer Framebuffer
  m_deferredBuffer.bind();
  m_deferredBuffer.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, m_gGeometry);
  m_deferredBuffer.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment1, m_gMaterial);
  m_deferredBuffer.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment2, m_gSurface);
  m_deferredBuffer.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::DepthAttachment,  m_gDepth);
  m_deferredBuffer.drawBuffers(OpenGLFramebufferObject::ColorAttachment0, OpenGLFramebufferObject::ColorAttachment1, OpenGLFramebufferObject::ColorAttachment2);
  checkFramebuffer("Deferred Buffer", m_deferredBuffer);
  m_deferredBuffer.release();

  // Light Buffer
  m_lightBuffer.bind();
  m_lightBuffer.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, m_gLighting);
  m_lightBuffer.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::DepthAttachment, m_gDepth);
  m_lightBuffer.drawBuffers(OpenGLFramebufferObject::ColorAttachment0);
  checkFramebuffer("Light Buffer", m_lightBuffer);
  m_lightBuffer.release();

  // Activate Backbuffers
  glActiveTexture(GL_TEXTURE0);
  m_gGeometry.bind();
  glActiveTexture(GL_TEXTURE1);
  m_gMaterial.bind();
  glActiveTexture(GL_TEXTURE2);
  m_gSurface.bind();
  glActiveTexture(GL_TEXTURE4);
  m_gLighting.bind();
  glActiveTexture(GL_TEXTURE5);
  m_gDepth.bind();
}

void MainWidgetPrivate::drawBackbuffer()
{
  OpenGLMarkerScoped _("Present G Buffer");
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  if (m_buffer == LightPass || m_buffer == MotionBlurPass)
  {
    OpenGLMarkerScoped _("Light Pass");
    m_lightBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    glDepthFunc(GL_GREATER);
    m_pointLightProgram->bind();
    m_pointLightGroup.draw();
    m_spotLightProgram->bind();
    m_spotLightGroup.draw();
    m_directionLightProgram->bind();
    m_directionLightGroup.draw();
    m_ambientProgram->bind();
    m_quadGL.draw();
    glDepthFunc(GL_LESS);
  }
  {
    OpenGLMarkerScoped _("Composition Pass");
    glBindFramebuffer(GL_FRAMEBUFFER, m_parent->defaultFramebufferObject());
    glClear(GL_COLOR_BUFFER_BIT);
    m_deferredPrograms[m_buffer]->bind();
    m_quadGL.draw();
    m_deferredPrograms[m_buffer]->release();
  }
  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
}

void MainWidgetPrivate::constructDeferredTexture(OpenGLTexture &t, OpenGLInternalFormat f)
{
  t.create(OpenGLTexture::Texture2D);
  t.bind();
  t.setInternalFormat(f);
  t.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  t.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  t.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  t.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  t.setSize(m_width, m_height);
  t.allocate();
  t.release();
}

void MainWidgetPrivate::checkFramebuffer(char const *name, OpenGLFramebufferObject &fbo)
{
  switch(fbo.status())
  {
  case OpenGLFramebufferObject::Complete:
    break;
  case OpenGLFramebufferObject::IncompleteAttachment:
    qFatal("%s: Incomplete Attachment", name);
    break;
  case OpenGLFramebufferObject::IncompleteMissingAttachment:
    qFatal("%s: Incomplete Missing Attachment", name);
    break;
  case OpenGLFramebufferObject::IncompleteDrawBuffer:
    qFatal("%s: Incomplete Draw Buffer", name);
    break;
  case OpenGLFramebufferObject::IncompleteReadBuffer:
    qFatal("%s: Incomplete Read Buffer", name);
    break;
  case OpenGLFramebufferObject::Unsupported:
    qFatal("%s: Unsupported", name);
    break;
  }
}

/*******************************************************************************
 * MainWidget
 ******************************************************************************/
MainWidget::MainWidget(QWidget *parent) :
  OpenGLWidget(parent)
{
  OpenGLShaderProgram::addSharedIncludePath(":/resources/shaders");
  OpenGLShaderProgram::addSharedIncludePath(":/resources/shaders/ubo");
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
  m_private = new MainWidgetPrivate(this);
  P(MainWidgetPrivate);
  p.m_transform.scale(50.0f);
  p.m_transform.translate(0.0f, 0.0f, -150.0f);
  p.m_dragVelocity = 0.0f;

  p.initializeGL();
  OpenGLWidget::initializeGL();
  printVersionInformation();

  // Set global information
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  p.m_quad = new KHalfEdgeMesh(this, ":/resources/objects/quad.obj");
  p.m_quadGL.create(*p.m_quad);

  // Application-specific initialization
  {
    // Uniform Block Object
    p.m_matrixBlock.create();
    p.m_matrixBlock.bind(1);
    p.m_matrixBlock.setUsagePattern(OpenGLBuffer::DynamicDraw);
    p.m_matrixBlock.allocate(sizeof(GLfloat) * (16 * 10 + 4 + 5 + 4));
    OpenGLUniformBufferManager::addUniformBufferObject("GlobalBuffer", &p.m_matrixBlock);

    // Create Shader for GBuffer Pass
    p.m_program = new OpenGLShaderProgram(this);
    p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer.vert");
    p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/gbuffer.frag");
    p.m_program->link();

    // Create Shader for point light pass
    p.m_pointLightProgram = new OpenGLShaderProgram(this);
    p.m_pointLightProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/pointLight.vert");
    p.m_pointLightProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/pointLight.frag");
    p.m_pointLightProgram->link();
    p.m_pointLightProgram->bind();
    p.m_pointLightProgram->setUniformValue("geometryTexture", 0);
    p.m_pointLightProgram->setUniformValue("materialTexture", 1);
    p.m_pointLightProgram->setUniformValue("dynamicsTexture", 2);
    p.m_pointLightProgram->setUniformValue("backbufferTexture", 3);
    p.m_pointLightProgram->setUniformValue("lightbufferTexture", 4);
    p.m_pointLightProgram->setUniformValue("depthTexture", 5);
    p.m_pointLightProgram->release();

    // Create Shader for direction light pass
    p.m_directionLightProgram = new OpenGLShaderProgram(this);
    p.m_directionLightProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/directionLight.vert");
    p.m_directionLightProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/directionLight.frag");
    p.m_directionLightProgram->link();
    p.m_directionLightProgram->bind();
    p.m_directionLightProgram->setUniformValue("geometryTexture", 0);
    p.m_directionLightProgram->setUniformValue("materialTexture", 1);
    p.m_directionLightProgram->setUniformValue("dynamicsTexture", 2);
    p.m_directionLightProgram->setUniformValue("backbufferTexture", 3);
    p.m_directionLightProgram->setUniformValue("lightbufferTexture", 4);
    p.m_directionLightProgram->setUniformValue("depthTexture", 5);
    p.m_directionLightProgram->release();

    // Create Shader for spot light pass
    p.m_spotLightProgram = new OpenGLShaderProgram(this);
    p.m_spotLightProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/spotLight.vert");
    p.m_spotLightProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/spotLight.frag");
    p.m_spotLightProgram->link();
    p.m_spotLightProgram->bind();
    p.m_spotLightProgram->setUniformValue("geometryTexture", 0);
    p.m_spotLightProgram->setUniformValue("materialTexture", 1);
    p.m_spotLightProgram->setUniformValue("dynamicsTexture", 2);
    p.m_spotLightProgram->setUniformValue("backbufferTexture", 3);
    p.m_spotLightProgram->setUniformValue("lightbufferTexture", 4);
    p.m_spotLightProgram->setUniformValue("depthTexture", 5);
    p.m_spotLightProgram->release();

    // Create Shader for ambient light pass
    p.m_ambientProgram = new OpenGLShaderProgram(this);
    p.m_ambientProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/ambient.vert");
    p.m_ambientProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/ambient.frag");
    p.m_ambientProgram->link();
    p.m_ambientProgram->bind();
    p.m_ambientProgram->setUniformValue("geometryTexture", 0);
    p.m_ambientProgram->setUniformValue("materialTexture", 1);
    p.m_ambientProgram->setUniformValue("dynamicsTexture", 2);
    p.m_ambientProgram->setUniformValue("backbufferTexture", 3);
    p.m_ambientProgram->setUniformValue("lightbufferTexture", 4);
    p.m_ambientProgram->setUniformValue("depthTexture", 5);
    p.m_ambientProgram->release();

    char const* fragFiles[DeferredDataCount] = {
      ":/resources/shaders/gbuffer/depth.frag",
      ":/resources/shaders/gbuffer/linearDepth.frag",
      ":/resources/shaders/gbuffer/position.frag",
      ":/resources/shaders/gbuffer/normal.frag",
      ":/resources/shaders/gbuffer/diffuse.frag",
      ":/resources/shaders/gbuffer/specular.frag",
      ":/resources/shaders/gbuffer/velocity.frag",
      ":/resources/shaders/gbuffer/ambient.frag",
      ":/resources/shaders/gbuffer/motion.frag",
      ":/resources/shaders/gbuffer/backbuffer.frag"
    };
    for (int i = 0; i < DeferredDataCount; ++i)
    {
      p.m_deferredPrograms[i] = new OpenGLShaderProgram(this);
      p.m_deferredPrograms[i]->addIncludePath(":/resources/shaders");
      p.m_deferredPrograms[i]->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer/main.vert");
      p.m_deferredPrograms[i]->addShaderFromSourceFile(QOpenGLShader::Fragment, fragFiles[i]);
      p.m_deferredPrograms[i]->link();
      p.m_deferredPrograms[i]->bind();
      p.m_deferredPrograms[i]->setUniformValue("geometryTexture", 0);
      p.m_deferredPrograms[i]->setUniformValue("materialTexture", 1);
      p.m_deferredPrograms[i]->setUniformValue("dynamicsTexture", 2);
      p.m_deferredPrograms[i]->setUniformValue("backbufferTexture", 3);
      p.m_deferredPrograms[i]->setUniformValue("lightbufferTexture", 4);
      p.m_deferredPrograms[i]->setUniformValue("depthTexture", 5);
      p.m_deferredPrograms[i]->release();
    }

    // Framebuffer Object
    p.m_deferredBuffer.create();
    p.m_lightBuffer.create();

    // Initialize the Direction Light Group
    p.m_directionLightGroup.create();
    p.m_directionLightGroup.setMesh(p.m_quadGL);
    for (int i = 0; i < 1; ++i)
    {
      OpenGLDirectionLight *light = p.m_directionLightGroup.createLight();
      light->setDiffuse(0.1f, 0.1f, 0.1f);
      light->setSpecular(0.1f, 0.1f, 0.1f);
    }

    // Initialize the Point Light Group
    p.m_pointLightGroup.create();
    p.m_pointLightGroup.setMesh(":/resources/objects/pointLight.obj");
    for (int i = 0; i < 5; ++i)
    {
      OpenGLPointLight *light = p.m_pointLightGroup.createLight();
      light->setRadius(25.0f);
    }

    // Initialize the Spot Light Group
    p.m_spotLightGroup.create();
    p.m_spotLightGroup.setMesh(":/resources/objects/spotLight.obj");
    for (int i = 0; i < 3; ++i)
    {
      OpenGLSpotLight *light = p.m_spotLightGroup.createLight();
      light->setDiffuse(0.4f, 0.4f, 0.4f);
      light->setAttenuation(1.0f, 0.0f, 0.0f);
      light->setDirection(0.0f, -1.0f, 0.0f);
      light->setInnerAngle(0.0f);
      light->setOuterAngle(45.0f);
      light->setDepth(25.0f);
    }

    p.m_floorGroup.create();
    p.m_instanceGroup.create();
    // Open OBJ
    KHalfEdgeMesh *mesh = new KHalfEdgeMesh(this, ":/resources/objects/floor.obj");
    mesh->calculateVertexNormals();
    p.m_floorGL.create(*mesh);
    p.m_floorGroup.setMesh(p.m_floorGL);
    p.m_floorInstance = p.m_floorGroup.createInstance();
    p.m_floorInstance->material().setDiffuse(0.0f, 0.0f, 1.0f);
    p.m_floorInstance->material().setSpecular(0.25f, 0.25f, 0.25f, 1.0f);
    p.m_floorInstance->transform().setScale(10000.0f);
    p.m_floorInstance->transform().setTranslation(0.0f, -1.0f, 0.0f);
    p.loadObj(":/resources/objects/sphere.obj");

    // Initialize instances
    //*
    for (int i = 0; i < 4; ++i)
    {
      const float radius = 10.0f;
      float radians = i * Karma::TwoPi / 4.0f;
      OpenGLInstance * instance = p.m_instanceGroup.createInstance();
      instance->currentTransform().setScale(1.0f);
      instance->material().setDiffuse(0.0f, 1.0f, 0.0f);
      instance->material().setSpecular(1.0f, 1.0f, 1.0f, 32.0f);
      instance->currentTransform().setTranslation(std::cos(radians) * radius, 0.0f, std::sin(radians) * radius);
    }
    OpenGLInstance * instance = p.m_instanceGroup.createInstance();
    instance->currentTransform().setScale(1.0f);
    instance->material().setDiffuse(0.0f, 1.0f, 0.0f);
    instance->material().setSpecular(1.0f, 1.0f, 1.0f, 32.0f);
    p.m_instances.push_back(instance);
    //*/
    qDebug() << "Instances: " << sg_count;

    // Release (unbind) all
    p.m_program->release();
  }

  OpenGLDebugDraw::initialize();
}

void MainWidget::resizeGL(int width, int height)
{
  P(MainWidgetPrivate);
  p.m_depthFar = 1000.0f;
  p.m_depthNear = 0.1f;
  p.m_projection.setToIdentity();
  p.m_projection.perspective(45.0f, width / float(height), p.m_depthNear, p.m_depthFar);
  p.updateBackbuffer(width, height);
  p.m_depthDiff = p.m_depthFar - p.m_depthNear;
  OpenGLWidget::resizeGL(width, height);
}

void MainWidget::paintGL()
{
  P(MainWidgetPrivate);

  if (!p.m_paused)
  {
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
        p.m_matrixBlock.write(sizeof(float) * 16 * 10, p.m_ambientColor, sizeof(float) * 4);
        p.m_matrixBlock.write(sizeof(float) * (16 * 10 + 4), p.m_atmosphericColor, sizeof(float) * 4);
        p.m_matrixBlock.write(sizeof(float) * (16 * 10 + 4 * 2), &p.m_depthFar, sizeof(float));
        p.m_matrixBlock.write(sizeof(float) * (16 * 10 + 4 * 2 + 1), &p.m_depthNear, sizeof(float));
        p.m_matrixBlock.write(sizeof(float) * (16 * 10 + 4 * 2 + 2), &p.m_depthDiff, sizeof(float));
        p.m_matrixBlock.write(sizeof(float) * (16 * 10 + 4 * 2 + 3), &p.m_width, sizeof(float));
        p.m_matrixBlock.write(sizeof(float) * (16 * 10 + 4 * 2 + 4), &p.m_height, sizeof(float));
        p.m_instanceGroup.update(p.m_camera.toMatrix(), p.m_cameraPrev.toMatrix());
        p.m_floorGroup.update(p.m_camera.toMatrix(), p.m_cameraPrev.toMatrix());
        p.m_pointLightGroup.update(p.m_projection, p.m_camera.toMatrix());
        p.m_directionLightGroup.update(p.m_projection, p.m_camera.toMatrix());
        p.m_spotLightGroup.update(p.m_projection, p.m_camera.toMatrix());
      }
      {
        OpenGLMarkerScoped _("Generate G Buffer");
        p.m_deferredBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        p.m_floorGroup.draw();
        p.m_instanceGroup.draw();
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
      }
      p.m_program->release();
      p.drawBackbuffer();
    }
    OpenGLProfiler::EndFrame();

    // Draw BV
    for (OpenGLInstance *i : p.m_instances)
    {
      //*
      if (p.b_bv[0]) p.m_aabbBV->draw(i->currentTransform(), Qt::red);
      if (p.b_bv[1]) p.m_sphereCentroidBV->draw(i->currentTransform(), Qt::red);
      if (p.b_bv[2]) p.m_sphereRittersBV->draw(i->currentTransform(), Qt::green);
      if (p.b_bv[3]) p.m_sphereLarssonsBV->draw(i->currentTransform(), Qt::blue);
      if (p.b_bv[4]) p.m_spherePcaBV->draw(i->currentTransform(), Qt::yellow);
      if (p.b_bv[5]) p.m_ellipsoidPcaBV->draw(i->currentTransform(), Qt::red);
      if (p.b_bv[6]) p.m_orientedPcaBV->draw(i->currentTransform(), Qt::red);
      //*/
    }
    if (p.m_staticGeometry)
      p.m_staticGeometry->drawAabbs(KTransform3D(), Qt::red, p.m_minDraw, p.m_maxDraw);

    OpenGLDebugDraw::draw();
    OpenGLWidget::paintGL();
  }
}

void MainWidget::teardownGL()
{
  OpenGLDebugDraw::teardown();
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

  static float f = 0.0f;
  f += 0.0016f;
  float angle = f;
  for (OpenGLDirectionLight *light : p.m_directionLightGroup)
  {
    light->setDirection(std::cos(angle), -1, std::sin(angle));
  }
  for (OpenGLPointLight *instance : p.m_pointLightGroup)
  {
    static const float radius = 5.0f;
    instance->setTranslation(cos(angle) * radius, 0.0f, sin(angle) * radius);
    angle += 2 * 3.1415926 / p.m_pointLightGroup.size();
  }
  angle = f;

  for (OpenGLSpotLight *instance : p.m_spotLightGroup)
  {
    static const float radius = 5.0f;
    instance->setTranslation(cos(angle) * radius, 5.0f + std::sin(angle * 15.0f) * 5.0f, sin(angle) * radius);
    instance->setDirection(-instance->translation().normalized());
    angle += 2 * 3.1415926 / p.m_spotLightGroup.size();
  }

  if (KInputManager::keyTriggered(Qt::Key_Plus))
  {
    for (OpenGLInstance *instance : p.m_instances)
    {
      instance->currentTransform().grow(1.0f);
    }
  }

  if (KInputManager::keyTriggered(Qt::Key_Underscore))
  {
    for (OpenGLInstance *instance : p.m_instances)
    {
      instance->currentTransform().grow(-1.0f);
    }
  }

  bool triggered = false;
  if (KInputManager::keyTriggered(Qt::Key_BracketLeft))
  {
    --p.m_maxDraw;
    triggered = true;
  }

  if (KInputManager::keyTriggered(Qt::Key_BracketRight))
  {
    ++p.m_maxDraw;
    triggered = true;
  }


  if (KInputManager::keyTriggered(Qt::Key_BraceLeft))
  {
    --p.m_minDraw;
    triggered = true;
  }

  if (KInputManager::keyTriggered(Qt::Key_BraceRight))
  {
    ++p.m_minDraw;
    triggered = true;
  }

  if (p.m_staticGeometry)
  {
    if (p.m_minDraw < 0)
    {
      p.m_minDraw = 0;
    }
    if (p.m_minDraw > p.m_staticGeometry->depth())
    {
      p.m_minDraw = static_cast<int>(p.m_staticGeometry->depth());
    }
    if (p.m_maxDraw < p.m_minDraw)
    {
      p.m_maxDraw = p.m_minDraw;
    }
    if (p.m_maxDraw > p.m_staticGeometry->depth())
    {
      p.m_maxDraw = static_cast<int>(p.m_staticGeometry->depth());
    }

    if (triggered)
    {
      QString format("MinMaxBounds [%1,%2]");
      QMainWindow* window = NULL;
      foreach(QWidget *widget, qApp->topLevelWidgets())
      {
        if(widget->inherits("QMainWindow"))
        {
          window = static_cast<QMainWindow*>(widget);
          window->setWindowTitle( format.arg(p.m_minDraw).arg(p.m_maxDraw) );
          break;
        }
      }
    }
  }

  for (OpenGLInstance *instance : p.m_instances)
  {
    if (p.b_rZ) instance->currentTransform().rotate(0.5f, 0.0f, 0.0f, 1.0f);
    if (p.b_rY) instance->currentTransform().rotate(0.25f, 0.0f, 1.0f, 0.0f);
    if (p.b_rX) instance->currentTransform().rotate(-1.25f, 1.0f, 0.0f, 0.0f);
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
  else
  {
    if (KInputManager::keyTriggered(Qt::Key_X)) p.b_rX = !p.b_rX;
    if (KInputManager::keyTriggered(Qt::Key_Y)) p.b_rY = !p.b_rY;
    if (KInputManager::keyTriggered(Qt::Key_Z)) p.b_rZ = !p.b_rZ;
  }

  if (KInputManager::keyPressed(Qt::Key_Control))
  {
    if (KInputManager::keyTriggered(Qt::Key_O))
    {
      p.openObj();
    }
  }

  if (KInputManager::keyPressed(Qt::Key_Shift))
  {
    auto depthPred = [](size_t numTriangles, size_t depth)->bool { (void)numTriangles; return depth >= 7; };
    if (KInputManager::keyTriggered(Qt::Key_B))
    {
      p.m_staticGeometry = p.m_staticGeometryBottomUp7;
      p.m_staticGeometryBottomUp7->build(KStaticGeometry::BottomUpMethod, depthPred);
      p.m_maxDraw = static_cast<int>(p.m_staticGeometry->depth());
    }
    if (KInputManager::keyTriggered(Qt::Key_T))
    {
      p.m_staticGeometry = p.m_staticGeometryTopDown7;
      p.m_staticGeometryTopDown7->build(KStaticGeometry::TopDownMethod, depthPred);
      p.m_maxDraw = static_cast<int>(p.m_staticGeometry->depth());
    }
  }
  else
  {
    auto trianglePred = [](size_t numTriangles, size_t depth)->bool { (void)depth; return numTriangles < 500; };
    if (KInputManager::keyTriggered(Qt::Key_B))
    {
      p.m_staticGeometry = p.m_staticGeometryBottomUp500;
      p.m_staticGeometryBottomUp500->build(KStaticGeometry::BottomUpMethod, trianglePred);
      p.m_maxDraw = static_cast<int>(p.m_staticGeometry->depth());
    }
    if (KInputManager::keyTriggered(Qt::Key_T))
    {
      p.m_staticGeometry = p.m_staticGeometryTopDown500;
      p.m_staticGeometryTopDown500->build(KStaticGeometry::TopDownMethod, trianglePred);
      p.m_maxDraw = static_cast<int>(p.m_staticGeometry->depth());
    }
  }

  // Change Buffer
  if (KInputManager::keyPressed(Qt::Key_Shift))
  {
    if (KInputManager::keyTriggered(Qt::Key_ParenRight))
    {
      p.m_buffer = (DeferredData)0;
    }
    if (KInputManager::keyTriggered(Qt::Key_Exclam))
    {
      p.m_buffer = (DeferredData)1;
    }
    if (KInputManager::keyTriggered(Qt::Key_At))
    {
      p.m_buffer = (DeferredData)2;
    }
    if (KInputManager::keyTriggered(Qt::Key_NumberSign))
    {
      p.m_buffer = (DeferredData)3;
    }
    if (KInputManager::keyTriggered(Qt::Key_Dollar))
    {
      p.m_buffer = (DeferredData)4;
    }
    if (KInputManager::keyTriggered(Qt::Key_Percent))
    {
      p.m_buffer = (DeferredData)5;
    }
    if (KInputManager::keyTriggered(Qt::Key_Ampersand))
    {
      p.m_buffer = (DeferredData)6;
    }
  }
  else
  {
    if (KInputManager::keyTriggered(Qt::Key_0))
    {
      p.b_bv[0] = !p.b_bv[0];
    }
    if (KInputManager::keyTriggered(Qt::Key_1))
    {
      p.b_bv[1] = !p.b_bv[1];
    }
    if (KInputManager::keyTriggered(Qt::Key_2))
    {
      p.b_bv[2] = !p.b_bv[2];
    }
    if (KInputManager::keyTriggered(Qt::Key_3))
    {
      p.b_bv[3] = !p.b_bv[3];
    }
    if (KInputManager::keyTriggered(Qt::Key_4))
    {
      p.b_bv[4] = !p.b_bv[4];
    }
    if (KInputManager::keyTriggered(Qt::Key_5))
    {
      p.b_bv[5] = !p.b_bv[5];
    }
    if (KInputManager::keyTriggered(Qt::Key_6))
    {
      p.b_bv[6] = !p.b_bv[6];
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
