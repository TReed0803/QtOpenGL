#include "compositionpass.h"

#include <KMacros>
#include <KInputManager>
#include <KScene>
#include <OpenGLMesh>
#include <OpenGLShaderProgram>
#include <OpenGLRenderBlock>
#include <OpenGLRenderer>
#include <OpenGLViewport>

enum PresentType
{
  PresentComposition,
  PresentDepth,
  PresentLinearDepth,
  PresentPosition,
  PresentViewNormal,
  PresentDiffuse,
  PresentSpecular,
  PresentVelocity,
  PresentLightAccumulation,
  MaxPresentations
};

class CompositionPassPrivate
{
public:
  CompositionPassPrivate();
  OpenGLMesh m_quadGL;
  PresentType m_present;
  OpenGLShaderProgram *m_presentationProgram[MaxPresentations];
};

CompositionPassPrivate::CompositionPassPrivate() :
  m_present(PresentComposition)
{
  // Intentionally Empty
}

void CompositionPass::initialize()
{
  m_private = new CompositionPassPrivate;
  P(CompositionPassPrivate);

  char const* fragFiles[] = {
    ":/resources/shaders/gbuffer/backbuffer.frag",
    ":/resources/shaders/gbuffer/depth.frag",
    ":/resources/shaders/gbuffer/linearDepth.frag",
    ":/resources/shaders/gbuffer/position.frag",
    ":/resources/shaders/gbuffer/normal.frag",
    ":/resources/shaders/gbuffer/diffuse.frag",
    ":/resources/shaders/gbuffer/specular.frag",
    ":/resources/shaders/gbuffer/velocity.frag",
    ":/resources/shaders/gbuffer/lightbuffer.frag"
  };

  if (sizeof(fragFiles) / sizeof(char const*) != MaxPresentations)
  {
    qFatal("Fatal: Must be able to present screen data for every presentation type!");
  }

  for (int i = 0; i < MaxPresentations; ++i)
  {
    p.m_presentationProgram[i] = new OpenGLShaderProgram();
    p.m_presentationProgram[i]->addIncludePath(":/resources/shaders");
    p.m_presentationProgram[i]->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer/main.vert");
    p.m_presentationProgram[i]->addShaderFromSourceFile(QOpenGLShader::Fragment, fragFiles[i]);
    p.m_presentationProgram[i]->link();
  }

  p.m_quadGL.create(":/resources/objects/quad.obj");
}

void CompositionPass::resize(int width, int height)
{
  // Unused (Present to backbuffer)
  (void)width;
  (void)height;
}

void CompositionPass::stage()
{
  // Unused
}

void CompositionPass::commit(const OpenGLViewport &view)
{
  P(CompositionPassPrivate);
  (void)view;

  // Change Buffer (Note: Shouldn't happen in a render pass)
  if (KInputManager::keyTriggered(Qt::Key_ParenRight))
  {
    p.m_present = PresentComposition;
  }
  else if (KInputManager::keyTriggered(Qt::Key_Exclam))
  {
    p.m_present = PresentDepth;
  }
  else if (KInputManager::keyTriggered(Qt::Key_At))
  {
    p.m_present = PresentLinearDepth;
  }
  else if (KInputManager::keyTriggered(Qt::Key_NumberSign))
  {
    p.m_present = PresentPosition;
  }
  else if (KInputManager::keyTriggered(Qt::Key_Dollar))
  {
    p.m_present = PresentViewNormal;
  }
  else if (KInputManager::keyTriggered(Qt::Key_Percent))
  {
    p.m_present = PresentDiffuse;
  }
  else if (KInputManager::keyTriggered(Qt::Key_AsciiCircum))
  {
    p.m_present = PresentSpecular;
  }
  else if (KInputManager::keyTriggered(Qt::Key_Ampersand))
  {
    p.m_present = PresentVelocity;
  }
  else if (KInputManager::keyTriggered(Qt::Key_Asterisk))
  {
    p.m_present = PresentLightAccumulation;
  }
}

void CompositionPass::render(const KScene &scene)
{
  P(CompositionPassPrivate);
  (void)scene;
  p.m_presentationProgram[p.m_present]->bind();
  p.m_quadGL.draw();
  p.m_presentationProgram[p.m_present]->release();
}

void CompositionPass::teardown()
{
  P(CompositionPassPrivate);
  for (int i = 0; i < MaxPresentations; ++i)
  {
    delete p.m_presentationProgram[i];
  }
  delete m_private;
}
