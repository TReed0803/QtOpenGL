#include "mainwidget.h"

// Qt Helpers
#include <QElapsedTimer>
#include <QFileDialog>

// Karma Abstractions
#include <KMath>
#include <KLinq>
#include <KMacros>
#include <KCamera3D>
#include <KInputManager>
#include <KHalfEdgeMesh>

// OpenGL Functionality
#include <OpenGLRenderBlock>
#include <OpenGLRenderer>
#include <OpenGLUniformManager>
#include <OpenGLTexture>
#include <OpenGLSpotLight>
#include <OpenGLPointLight>
#include <OpenGLDirectionLight>
#include <OpenGLSpotLightGroup>
#include <OpenGLPointLightGroup>
#include <OpenGLDirectionLightGroup>
#include <OpenGLInstance>
#include <OpenGLMaterial>
#include <OpenGLMesh>
#include <OpenGLShaderProgram>

// Render Passes
#include <GBufferPass>
#include <LightPass>
#include <CompositionPass>

/*******************************************************************************
 * MainWidgetPrivate
 ******************************************************************************/
class MainWidgetPrivate
{
public:

  // GL Methods
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void teardownGL();

  // Object Manipulation
  void openObj();
  void loadObj(const QString &fileName);

  // Helper Methods
  OpenGLRenderBlock &currentRenderBlock();
  OpenGLRenderBlock &previousRenderBlock();
  void swapRenderBlocks();
  void fixRenderBlocks();
  void updateRenderBlocks();

  // Render Data
  OpenGLRenderBlock m_renderBlocks[2];
  int m_renderBlockIndex[2];
  OpenGLRenderer m_renderer;

  // Scene Data
  KCamera3D m_camera;
  std::vector<OpenGLInstance *> m_instances;
};

/*******************************************************************************
 * MainWidgetPrivate::OpenGL Methods
 ******************************************************************************/
void MainWidgetPrivate::initializeGL()
{
  // Set Uniform Buffers
  OpenGLUniformManager::setUniformBufferIndex("CurrentRenderBlock"  , 1);
  OpenGLUniformManager::setUniformBufferIndex("PreviousRenderBlock" , 2);
  OpenGLUniformManager::setUniformBufferIndex("SpotLightProperties" , 3);

  // Set Texture Samplers
  OpenGLUniformManager::setTextureSampler("depthTexture"      , OpenGLTexture::numTextureUnits() - 1);
  OpenGLUniformManager::setTextureSampler("geometryTexture"   , OpenGLTexture::numTextureUnits() - 2);
  OpenGLUniformManager::setTextureSampler("materialTexture"   , OpenGLTexture::numTextureUnits() - 3);
  OpenGLUniformManager::setTextureSampler("surfaceTexture"    , OpenGLTexture::numTextureUnits() - 4);
  OpenGLUniformManager::setTextureSampler("lightbufferTexture", OpenGLTexture::numTextureUnits() - 5);

  // Global Setup (Rarely Changed)
  GL::glEnable(GL_CULL_FACE);
  GL::glEnable(GL_DEPTH_TEST);
  GL::glClearDepthf(1.0f);
  GL::glDepthFunc(GL_LEQUAL);

  // Create Renderer
  m_renderer.create();
  m_renderer.addPass<GBufferPass>();
  m_renderer.addPass<LightPass>();
  m_renderer.addPass<CompositionPass>();
  m_renderer.initialize();

  // Scene Initialization
  m_camera.setTranslation(0.0f, 3.0f, 10.0f);
  m_camera.setRotation(-20.0f, 1.0f, 0.0f, 0.0f);
  m_renderBlockIndex[0] = 0; // Current Index
  m_renderBlockIndex[1] = 1; // Previous Index

  for (int i = 0; i < 2; ++i)
  {
    m_renderBlocks[i].create();
    m_renderBlocks[i].setUsagePattern(OpenGLBuffer::DynamicDraw);
    m_renderBlocks[i].bind();
    m_renderBlocks[i].allocate();
    m_renderBlocks[i].release();
    m_renderBlocks[i].setViewMatrix(m_camera.toMatrix());
  }
  m_renderBlocks[0].bindBase(1);
  m_renderBlocks[0].bindBase(2);

  {

    // Initialize the Direction Light Group
    for (int i = 0; i < 1; ++i)
    {
      OpenGLDirectionLight *light = m_renderer.createDirectionLight();
      light->setDiffuse(0.1f, 0.1f, 0.1f);
      light->setSpecular(0.1f, 0.1f, 0.1f);
    }

    // Initialize the Point Light Group
    for (int i = 0; i < 5; ++i)
    {
      OpenGLPointLight *light = m_renderer.createPointLight();
      light->setRadius(25.0f);
    }

    // Initialize the Spot Light Group
    for (int i = 0; i < 3; ++i)
    {
      OpenGLSpotLight *light = m_renderer.createSpotLight();
      light->setInnerAngle(40.0f);
      light->setOuterAngle(45.0f);
      light->setDepth(25.0f);
    }

    // Note: Currently there is no Material System.
    //       All material properties are per-instance.
    OpenGLInstance *floor = m_renderer.createInstance();
    floor->setMesh(":/resources/objects/floor.obj");
    floor->material().setDiffuse(0.0f, 0.0f, 1.0f);
    floor->material().setSpecular(0.25f, 0.25f, 0.25f, 1.0f);
    floor->transform().setScale(1000.0f);
    floor->transform().setTranslation(0.0f, -1.0f, 0.0f);

    // Create Instance Data
    static const int total = 4;
    static const float arcLength = Karma::TwoPi / float(total);
    for (int i = 0; i < total; ++i)
    {
      const float radius = 10.0f;
      const float radians = i * arcLength;
      OpenGLInstance * instance = m_renderer.createInstance();
      instance->currentTransform().setScale(1.0f);
      instance->material().setDiffuse(0.0f, 1.0f, 0.0f);
      instance->material().setSpecular(1.0f, 1.0f, 1.0f, 32.0f);
      instance->currentTransform().setTranslation(std::cos(radians) * radius, 0.0f, std::sin(radians) * radius);
      m_instances.push_back(instance);
    }
    OpenGLInstance * instance = m_renderer.createInstance();
    instance->currentTransform().setScale(1.0f);
    instance->material().setDiffuse(0.0f, 1.0f, 0.0f);
    instance->material().setSpecular(1.0f, 1.0f, 1.0f, 32.0f);
    m_instances.push_back(instance);
    loadObj(":/resources/objects/sphere.obj");
  }
}

void MainWidgetPrivate::resizeGL(int width, int height)
{
  // Calculate the new render information
  float depthNear = 0.1f;
  float depthFar  = 1000.0f;
  KMatrix4x4 perspective;
  perspective.perspective(45.0f, width / float(height), depthNear, depthFar);

  // Update renderblocks
  OpenGLRenderBlock &currRenderBlock = m_renderBlocks[m_renderBlockIndex[0]];
  OpenGLRenderBlock &prevRenderBlock = m_renderBlocks[m_renderBlockIndex[1]];
  currRenderBlock.setNearFar(depthNear, depthFar);
  currRenderBlock.setPerspectiveMatrix(perspective);
  currRenderBlock.setDimensions(width, height);
  prevRenderBlock.setNearFar(depthNear, depthFar);
  prevRenderBlock.setPerspectiveMatrix(perspective);
  prevRenderBlock.setDimensions(width, height);

  m_renderer.resize(width, height);
}

void MainWidgetPrivate::paintGL()
{
  OpenGLProfiler::BeginFrame();
  {
    OpenGLMarkerScoped _("Total Render Time");
    {
      OpenGLMarkerScoped _("Prepare Scene");

      // Update the previous/current render block bindings
      if (m_camera.dirty())
      {
        swapRenderBlocks();
        currentRenderBlock().setViewMatrix(m_camera.toMatrix());
      }
      else
      {
        fixRenderBlocks();
      }
      updateRenderBlocks();

      // Update the GPU instance data
      m_renderer.update(currentRenderBlock(), previousRenderBlock());
    }
    // Render Scene
    m_renderer.render();
  }
  OpenGLProfiler::EndFrame();
}

void MainWidgetPrivate::teardownGL()
{
  m_renderer.teardown();
}

/*******************************************************************************
 * MainWidgetPrivate::Object Manipulation
 ******************************************************************************/
void MainWidgetPrivate::loadObj(const QString &fileName)
{
  OpenGLMesh openGLMesh;
  KHalfEdgeMesh halfEdgeMesh;
  KCountResult boundaries;

  // Boundary Query
  auto query =
    COUNT
      FROM  ( edge : halfEdgeMesh.halfEdges() )
      WHERE ( edge.face == 0 )
      INCREMENT (1);

  // Initialize an object
  quint64 ms;
  QElapsedTimer timer;
  {
    // Load Half Edge Mesh
    {
      timer.start();
      halfEdgeMesh.create(qPrintable(fileName));
      ms = timer.elapsed();
      qDebug() << "Create HalfEdgeMesh (sec)    :" << float(ms) / 1e3f;
    }
    // Calculate Normals
    {
      timer.start();
      halfEdgeMesh.calculateVertexNormals();
      ms = timer.elapsed();
      qDebug() << "Calculate Normals (sec)      :" << float(ms) / 1e3f;
    }
    // Calculate OpenGLMesh
    {
      timer.start();
      openGLMesh.create(halfEdgeMesh);
      ms = timer.elapsed();
      qDebug() << "Create OpenGLMesh (sec)      :" << float(ms) / 1e3f;
    }
    {
      timer.start();
      boundaries = query();
      ms = timer.elapsed();
      qDebug() << "Mesh Query Time (sec)        :" << float(ms) / 1e3f;
    }
    qDebug() << "--------------------------------------";
    qDebug() << "Mesh Vertexes  :" << halfEdgeMesh.vertices().size();
    qDebug() << "Mesh Faces     :" << halfEdgeMesh.faces().size();
    qDebug() << "Mesh HalfEdges :" << halfEdgeMesh.halfEdges().size();
    qDebug() << "Boundary Edges :" << boundaries;
  }

  // Set all instances to have the same mesh
  for (OpenGLInstance *instance : m_instances)
  {
    instance->setMesh(qPrintable(fileName));
  }
}

/*******************************************************************************
 * MainWidgetPrivate::Helper Methods
 ******************************************************************************/
OpenGLRenderBlock &MainWidgetPrivate::currentRenderBlock()
{
  return m_renderBlocks[m_renderBlockIndex[0]];
}

OpenGLRenderBlock &MainWidgetPrivate::previousRenderBlock()
{
  if (OpenGLUniformBufferObject::boundBufferId(1) != OpenGLUniformBufferObject::boundBufferId(2))
  {
    return m_renderBlocks[m_renderBlockIndex[1]];
  }
  return m_renderBlocks[m_renderBlockIndex[0]];
}

void MainWidgetPrivate::swapRenderBlocks()
{
  // Get the render blocks in their current state
  OpenGLRenderBlock &currRenderBlock = m_renderBlocks[m_renderBlockIndex[0]];
  OpenGLRenderBlock &prevRenderBlock = m_renderBlocks[m_renderBlockIndex[1]];

  // Swap the binding indices of the render blocks
  std::swap(m_renderBlockIndex[0], m_renderBlockIndex[1]);

  // Update the binding indices of each render block
  currRenderBlock.bindBase(m_renderBlockIndex[0] + 1);
  prevRenderBlock.bindBase(m_renderBlockIndex[1] + 1);
}

void MainWidgetPrivate::fixRenderBlocks()
{
  // Current = Previous (No camera motion applied)
  if (OpenGLUniformBufferObject::boundBufferId(1) != OpenGLUniformBufferObject::boundBufferId(2))
  {
    OpenGLRenderBlock &currRenderBlock = m_renderBlocks[m_renderBlockIndex[0]];
    currRenderBlock.bindBase(m_renderBlockIndex[0] + 1);
  }
}

void MainWidgetPrivate::updateRenderBlocks()
{
  // Update previous/current render block data (if needed)
  for (int i = 0; i < 2; ++i)
  {
    if (m_renderBlocks[i].dirty())
    {
      m_renderBlocks[i].bind();
      m_renderBlocks[i].update();
      m_renderBlocks[i].release();
    }
  }
}

/*******************************************************************************
 * MainWidget
 ******************************************************************************/
MainWidget::MainWidget(QWidget *parent) :
  OpenGLWidget(parent)
{
  // Set Shader Includes
  OpenGLShaderProgram::addSharedIncludePath(":/resources/shaders");
  OpenGLShaderProgram::addSharedIncludePath(":/resources/shaders/ubo");
}

MainWidget::~MainWidget()
{
  P(MainWidgetPrivate);
  makeCurrent();
  p.teardownGL();
  OpenGLWidget::teardownGL();
  delete m_private;
}

/*******************************************************************************
 * OpenGL Methods
 ******************************************************************************/
void MainWidget::initializeGL()
{
  OpenGLWidget::initializeGL();
  m_private = new MainWidgetPrivate;
  P(MainWidgetPrivate);
  p.initializeGL();
}

void MainWidget::resizeGL(int width, int height)
{
  P(MainWidgetPrivate);
  p.resizeGL(width, height);
  OpenGLWidget::resizeGL(width, height);
}

void MainWidget::paintGL()
{
  P(MainWidgetPrivate);
  if (!p.m_renderer.isPaused())
  {
    p.paintGL();
    OpenGLWidget::paintGL();
  }
}

/*******************************************************************************
 * Events
 ******************************************************************************/
void MainWidget::updateEvent(KUpdateEvent *event)
{
  P(MainWidgetPrivate);
  (void)event;

  // Update Lights (Scene update)
  static float f = 0.0f;
  f += 0.0016f;
  float angle = f;
  for (OpenGLDirectionLight *light : p.m_renderer.directionLights())
  {
    light->setDirection(std::cos(angle), -1, std::sin(angle));
  }
  for (OpenGLPointLight *instance : p.m_renderer.pointLights())
  {
    static const float radius = 5.0f;
    instance->setTranslation(cos(angle) * radius, 0.0f, sin(angle) * radius);
    angle += 2 * 3.1415926 / p.m_renderer.pointLights().size();
  }
  angle = f;

  for (OpenGLSpotLight *instance : p.m_renderer.spotLights())
  {
    static const float radius = 5.0f;
    instance->setTranslation(cos(angle) * radius, 5.0f + std::sin(angle * 15.0f) * 5.0f, sin(angle) * radius);
    instance->setDirection(-instance->translation().normalized());
    angle += 2 * 3.1415926 / p.m_renderer.spotLights().size();
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
      p.m_renderer.pause(true);
      QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Model"),
        ".",
        tr("Wavefront Object File (*.obj))")
      );
      if (!fileName.isNull())
      {
        makeCurrent();
        p.loadObj(fileName);
      }
      p.m_renderer.pause(false);
    }
  }
}
