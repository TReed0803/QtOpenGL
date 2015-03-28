#include "debuggbufferpass.h"

#include <KMacros>
#include <OpenGLScene>
#include <OpenGLMesh>
#include <OpenGLTexture>
#include <OpenGLShaderProgram>
#include <OpenGLFramebufferObject>
#include <OpenGLRenderer>
#include <OpenGLRenderBlock>
#include <OpenGLViewport>
#include <OpenGLBindings>
#include <KInputManager>

enum DisplayBuffer
{
  PositionBuffer,
  NormalBuffer,
  VelocityBuffer,
  DiffuseBuffer,
  FresnelBuffer,
  RoughnessBuffer,
  BufferCount
};

class DebugGBufferPassPrivate
{
public:
  // Render Helpers
  OpenGLMesh m_quad;
  OpenGLShaderProgram *m_program[BufferCount];
  DisplayBuffer m_display;
};

DebugGBufferPass::DebugGBufferPass() :
  m_private(0)
{
  // Intentionally Empty
}

void DebugGBufferPass::initialize()
{
  m_private = new DebugGBufferPassPrivate;
  P(DebugGBufferPassPrivate);

  char const *possibleFragmentShaders[] =
  {
    ":/resources/shaders/gbuffer/position.frag",
    ":/resources/shaders/gbuffer/normal.frag",
    ":/resources/shaders/gbuffer/velocity.frag",
    ":/resources/shaders/gbuffer/diffuse.frag",
    ":/resources/shaders/gbuffer/fresnel.frag",
    ":/resources/shaders/gbuffer/roughness.frag"
  };

  for (int i = 0; i < BufferCount; ++i)
  {
    p.m_program[i] = new OpenGLShaderProgram();
    p.m_program[i]->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer/main.vert");
    p.m_program[i]->addShaderFromSourceFile(QOpenGLShader::Fragment, possibleFragmentShaders[i]);
    p.m_program[i]->link();
  }

  p.m_display = PositionBuffer;
  p.m_quad.create(":/resources/objects/quad.obj");
}

void DebugGBufferPass::resize(int width, int height)
{
  (void)width;
  (void)height;
}

void DebugGBufferPass::commit(OpenGLViewport &view)
{
  (void)view;
}

void DebugGBufferPass::render(OpenGLScene &scene)
{
  (void)scene;
  P(DebugGBufferPassPrivate);
  OpenGLMarkerScoped _("Debug G Buffer");

  if (KInputManager::keyTriggered(Qt::Key_0))
  {
    p.m_display = static_cast<DisplayBuffer>(0);
  }
  if (KInputManager::keyTriggered(Qt::Key_1))
  {
    p.m_display = static_cast<DisplayBuffer>(1);
  }
  if (KInputManager::keyTriggered(Qt::Key_2))
  {
    p.m_display = static_cast<DisplayBuffer>(2);
  }
  if (KInputManager::keyTriggered(Qt::Key_3))
  {
    p.m_display = static_cast<DisplayBuffer>(3);
  }
  if (KInputManager::keyTriggered(Qt::Key_4))
  {
    p.m_display = static_cast<DisplayBuffer>(4);
  }
  if (KInputManager::keyTriggered(Qt::Key_5))
  {
    p.m_display = static_cast<DisplayBuffer>(5);
  }

  // Generate the GBuffer
  p.m_program[p.m_display]->bind();
  p.m_quad.draw();
  p.m_program[p.m_display]->release();
}

void DebugGBufferPass::teardown()
{
  P(DebugGBufferPassPrivate);
  delete m_private;
}
