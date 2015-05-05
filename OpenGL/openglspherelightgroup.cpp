#include "openglspherelightgroup.h"

#include <KMacros>
#include <vector>
#include <OpenGLMesh>
#include <OpenGLSphereLight>
#include <OpenGLAreaLightData>
#include <OpenGLShaderProgram>
#include <OpenGLUniformBufferObject>
#include <OpenGLBindings>
#include <OpenGLViewport>
#include <OpenGLRenderBlock>
#include <KMath>

class OpenGLSphereLightGroupPrivate
{
public:
  int m_uniformOffset;
  OpenGLMesh m_mesh;
  OpenGLShaderProgram *m_program;
  OpenGLUniformBufferObject m_uniforms;
  std::vector<OpenGLSphereLight*> m_lights;
};

OpenGLSphereLightGroup::OpenGLSphereLightGroup()
{
  // Intentionally Empty
}

OpenGLSphereLightGroup::~OpenGLSphereLightGroup()
{
  // Intentionally Empty
}

bool OpenGLSphereLightGroup::create()
{
  m_private = new OpenGLSphereLightGroupPrivate;
  P(OpenGLSphereLightGroupPrivate);
  p.m_mesh.create(":/resources/objects/quad.obj");
  p.m_program = new OpenGLShaderProgram;
  p.m_program->create();
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/sphereLight.vert");
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/sphereLight.frag");
  p.m_program->link();

  return p.m_uniforms.create();
}

void OpenGLSphereLightGroup::commit(const OpenGLViewport &view)
{
  P(OpenGLSphereLightGroupPrivate);
  if (p.m_lights.empty()) return;

  OpenGLUniformBufferObject::RangeAccessFlags flags =
      OpenGLUniformBufferObject::RangeInvalidate
    | OpenGLUniformBufferObject::RangeUnsynchronized
    | OpenGLUniformBufferObject::RangeWrite;

  p.m_uniforms.bind();
  p.m_uniformOffset = p.m_uniforms.reserve(sizeof(OpenGLAreaLightData), p.m_lights.size());
  char *data = static_cast<char*>(p.m_uniforms.mapRange(0, p.m_uniformOffset * p.m_lights.size(), flags));

  if (data == NULL)
  {
    qFatal("Failed to map the buffer range!");
  }

  OpenGLSphereLight *src;
  OpenGLAreaLightData *dest;
  OpenGLRenderBlock const &stats = view.current();
  for (int i = 0; i < p.m_lights.size(); ++i)
  {
    src = p.m_lights[i];
    dest = reinterpret_cast<OpenGLAreaLightData*>(data);
    dest->f_intensity = src->intensity();
    dest->m_modelToPersp = stats.worldToPersp() * Karma::ToGlm(src->toMatrix());
    dest->f_radius = src->radius();
    dest->v_color = Karma::ToGlm(src->color());
    dest->v_viewPosition = glm::vec3(stats.worldToView() * Karma::ToGlm(src->translation(), 1.0f));
    dest->v_data0.x = src->radius();
    data += p.m_uniformOffset;
  }

  p.m_uniforms.unmap();
  p.m_uniforms.release();
}

void OpenGLSphereLightGroup::draw()
{
  P(OpenGLSphereLightGroupPrivate);
  if (p.m_lights.empty()) return;
  p.m_mesh.bind();
  p.m_program->bind();
  GL::glDisable(GL_DEPTH_TEST);
  GL::glEnable(GL_BLEND);
  GL::glBlendFunc(GL_ONE, GL_ONE);
  for (unsigned i = 0; i < p.m_lights.size(); ++i)
  {
    if (!p.m_lights[i]->active()) continue;
    p.m_uniforms.bindRange(OpenGLUniformBufferObject::UniformBuffer, K_LIGHT_BINDING, static_cast<int>(p.m_uniformOffset * i), static_cast<int>(sizeof(OpenGLAreaLightData)));
    p.m_mesh.draw();
  }
  GL::glDisable(GL_BLEND);
  GL::glEnable(GL_DEPTH_TEST);
}

OpenGLSphereLight *OpenGLSphereLightGroup::createLight()
{
  P(OpenGLSphereLightGroupPrivate);
  OpenGLSphereLight *light = new OpenGLSphereLight;
  p.m_lights.push_back(light);
  return light;
}

OpenGLSphereLight *OpenGLSphereLightGroup::operator[](int idx)
{
  P(OpenGLSphereLightGroupPrivate);
  return p.m_lights[idx];
}

int OpenGLSphereLightGroup::size() const
{
  P(const OpenGLSphereLightGroupPrivate);
  return p.m_lights.size();
}

auto OpenGLSphereLightGroup::begin() -> LightContainer::iterator
{
  P(OpenGLSphereLightGroupPrivate);
  return p.m_lights.begin();
}

auto OpenGLSphereLightGroup::end() -> LightContainer::iterator
{
  P(OpenGLSphereLightGroupPrivate);
  return p.m_lights.end();
}
