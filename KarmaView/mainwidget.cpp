#include "mainwidget.h"

#include <KScene> // Note: OpenGLScene?
#include <OpenGLRenderer>
#include <OpenGLViewport>
#include <OpenGLShaderProgram>
#include <OpenGLTexture>
#include <OpenGLUniformManager>

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
  MainWidgetPrivate();

  // GL Methods
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void teardownGL();

  // Helper Methods
  OpenGLRenderBlock &currentRenderBlock();
  OpenGLRenderBlock &previousRenderBlock();
  void swapRenderBlocks();
  void fixRenderBlocks();
  void updateRenderBlocks();

  // Render Data
  OpenGLRenderer m_renderer;
  KScene *m_scene;
  OpenGLViewport *m_view;
};

MainWidgetPrivate::MainWidgetPrivate() :
  m_scene(0), m_view(0)
{
  // Intentionally Empty
}

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
}

void MainWidgetPrivate::resizeGL(int width, int height)
{
  if (m_view)
  {
    m_view->setWidthHeight(width, height);
  }
  m_renderer.resize(width, height);
}

void MainWidgetPrivate::paintGL()
{
  OpenGLProfiler::BeginFrame();
  if (m_view && m_scene)
  {
    OpenGLMarkerScoped _("Total Render Time");
    {
      OpenGLMarkerScoped _("Prepare Scene");
      m_renderer.commit(*m_view);
      m_scene->commit(*m_view);
    }
    {
      OpenGLMarkerScoped _("Render Scene");
      m_renderer.render(*m_scene);
    }
  }
  OpenGLProfiler::EndFrame();
}

void MainWidgetPrivate::teardownGL()
{
  m_renderer.teardown();
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

void MainWidget::setScene(KScene *scene)
{
  P(MainWidgetPrivate);
  p.m_scene = scene;
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
  if (p.m_scene)
  {
    p.m_scene->update(event);
  }
}
