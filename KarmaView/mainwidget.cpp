#include "mainwidget.h"

// OpenGL Framework
#include <OpenGLRenderer>
#include <OpenGLViewport>
#include <OpenGLShaderProgram>
#include <OpenGLTexture>
#include <OpenGLUniformManager>
#include <OpenGLSceneManager>

// Render Passes
#include <GBufferPass>
#include <PreparePresentationPass>
#include <LightAccumulationPass>
#include <MotionBlurPass>
#include <ViewportPresentationPass>

// Scenes
#include <SampleScene>

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

  // Helper Methods
  OpenGLRenderBlock &currentRenderBlock();
  OpenGLRenderBlock &previousRenderBlock();
  void swapRenderBlocks();
  void fixRenderBlocks();
  void updateRenderBlocks();

  // Render Data
  OpenGLRenderer m_renderer;
  OpenGLSceneManager m_sceneManager;
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
  m_renderer.bind();
  m_renderer.addPass<GBufferPass>();              // => Nothing (Constructs Globals)
  m_renderer.addPass<PreparePresentationPass>();  // => RenderBuffer
  m_renderer.addPass<LightAccumulationPass>();    // => RenderBuffer
  m_renderer.addPass<MotionBlurPass>();           // => RenderBuffer
  m_renderer.addPass<ViewportPresentationPass>(); // => Nothing (Displays RenderBuffer)
}

void MainWidgetPrivate::resizeGL(int width, int height)
{
  m_renderer.resize(width, height);
}

void MainWidgetPrivate::paintGL()
{
  OpenGLProfiler::BeginFrame();
  if (m_sceneManager.activeScene())
  {
    m_renderer.render(*m_sceneManager.currentScene());
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

/*******************************************************************************
 * OpenGL Methods
 ******************************************************************************/
void MainWidget::initializeGL()
{
  OpenGLWidget::initializeGL();
  m_private = new MainWidgetPrivate;
  P(MainWidgetPrivate);
  p.initializeGL();
  p.m_sceneManager.pushScene(new SampleScene);
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
  makeCurrent();
  p.m_sceneManager.update(event);
}
