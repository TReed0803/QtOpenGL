#include "mainwidget.h"

// Karma Framework
#include <KInputManager>

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
#include <ShadowedLightAccumulationPass>
#include <MotionBlurPass>
#include <ViewportPresentationPass>
#include <DebugGBufferPass>
#include <EnvironmentPass>
#include <ScreenSpaceAmbientOcclusion>

// Scenes
#include <SampleScene>

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
  bool m_started;
  bool m_initialized;
  OpenGLRenderer m_renderer;
  OpenGLSceneManager m_sceneManager;
};

/*******************************************************************************
 * MainWidgetPrivate::OpenGL Methods
 ******************************************************************************/
MainWidgetPrivate::MainWidgetPrivate() :
  m_started(false),
  m_initialized(false)
{
  // Intentionally Empty
}

void MainWidgetPrivate::initializeGL()
{
  // Global Setup (Rarely Changed)
  GL::glEnable(GL_CULL_FACE);
  GL::glEnable(GL_DEPTH_TEST);
  GL::glClearDepthf(1.0f);
  GL::glDepthFunc(GL_LEQUAL);

  // Create Renderer
  m_renderer.create();
  m_renderer.bind();
  m_renderer.addPass<GBufferPass>();                      // => Nothing (Constructs Globals)
  m_renderer.addPass<ScreenSpaceAmbientOcclusion>();      // => SSAO Buffer
  m_renderer.addPass<PreparePresentationPass>();          // => RenderBuffer
  m_renderer.addPass<EnvironmentPass>();                  // => Ambient Term
  m_renderer.addPass<LightAccumulationPass>();            // => Non-Shadowed Lights
  m_renderer.addPass<ShadowedLightAccumulationPass>();    // => RenderBuffer
  m_renderer.addPass<MotionBlurPass>();                   // => RenderBuffer
  m_renderer.addPass<ViewportPresentationPass>();         // => Nothing (Displays RenderBuffer)

  m_initialized = true;
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

bool MainWidget::isReady() const
{
  P(const MainWidgetPrivate);
  return p.m_initialized && p.m_started;
}

SampleScene* MainWidget::sampleScene()
{
  return static_cast<SampleScene*>(m_private->m_sceneManager.currentScene());
}

OpenGLRenderer *MainWidget::renderer()
{
  return &m_private->m_renderer;
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
void MainWidget::updateEvent(OpenGLUpdateEvent *event)
{
  P(MainWidgetPrivate);
  makeCurrent();
  p.m_sceneManager.update(event);
  p.m_started = true;
}
