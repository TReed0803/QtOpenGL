#include "openglrectanglelightgroup.h"

#include <KMacros>
#include <vector>
#include <OpenGLMesh>
#include <OpenGLRectangleLight>
#include <OpenGLAreaLightData>
#include <OpenGLShaderProgram>
#include <OpenGLUniformBufferObject>
#include <OpenGLBindings>
#include <OpenGLViewport>
#include <OpenGLRenderBlock>
#include <KMath>
#include <KInputManager>
#include <OpenGLDebugDraw>

static struct shader_data_t
{
  int samplePosition[4];
  float worldPosition[4];
  float viewVector[4];
  float reflectionVector[4];
  float normalVector[4];
  float mrpPoint[4];
  float mrpUnclamped[4];
  float ellipseMajor[4];
  float ellipseMinor[4];
  float ellipseNormal[4];
  float bottomLeft[4];
  float topRight[4];
  float bottomRight[4];
  float topLeft[4];
} shader_data;

class OpenGLRectangleLightGroupPrivate
{
public:
  GLuint ssbo;
  int m_uniformOffset;
  OpenGLMesh m_mesh;
  OpenGLShaderProgram *m_program;
  OpenGLUniformBufferObject m_uniforms;
  std::vector<OpenGLRectangleLight*> m_lights;
};

OpenGLRectangleLightGroup::OpenGLRectangleLightGroup()
{
  // Intentionally Empty
}

OpenGLRectangleLightGroup::~OpenGLRectangleLightGroup()
{
  // Intentionally Empty
}

bool OpenGLRectangleLightGroup::create()
{
  m_private = new OpenGLRectangleLightGroupPrivate;
  P(OpenGLRectangleLightGroupPrivate);
  p.m_mesh.create(":/resources/objects/quad.obj");
  p.m_program = new OpenGLShaderProgram;
  p.m_program->create();
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/rectangleLight.vert");
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/rectangleLight.frag");
  p.m_program->link();

  // SSBO
  GL::glGenBuffers(1, &p.ssbo);
  GL::glBindBuffer(GL_SHADER_STORAGE_BUFFER, p.ssbo);
  GL::glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shader_data), &shader_data, GL_DYNAMIC_COPY);
  GL::glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  return p.m_uniforms.create();
}

void OpenGLRectangleLightGroup::commit(const OpenGLViewport &view)
{
  P(OpenGLRectangleLightGroupPrivate);
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

  OpenGLRectangleLight *src;
  OpenGLAreaLightData *dest;
  OpenGLRenderBlock const &stats = view.current();
  for (SizeType i = 0; i < p.m_lights.size(); ++i)
  {
    src = p.m_lights[i];
    dest = reinterpret_cast<OpenGLAreaLightData*>(data);
    dest->f_intensity = src->intensity();
    dest->m_modelToPersp = stats.worldToPersp() * Karma::ToGlm(src->toMatrix());
    dest->f_radius = src->radius();
    dest->v_color = Karma::ToGlm(src->color());
    dest->v_viewPosition = glm::vec3(stats.worldToView() * Karma::ToGlm(src->translation(), 1.0f));
    dest->v_data0.x = src->halfWidth();
    dest->v_data0.y = src->halfHeight();
    dest->v_data0.z = src->width();
    dest->v_data0.w = src->height();
    dest->v_data1   = stats.worldToView() * Karma::ToGlm(src->forward(), 0.0);
    dest->v_data2   = stats.worldToView() * Karma::ToGlm(src->right(), 0.0);
    dest->v_data3   = stats.worldToView() * Karma::ToGlm(src->up(), 0.0);
    data += p.m_uniformOffset;
  }

  p.m_uniforms.unmap();
  p.m_uniforms.release();

  KPoint point = KInputManager::mousePosition();
  shader_data.samplePosition[0] = point.x();
  shader_data.samplePosition[1] = 600 - point.y();
  GL::glBindBuffer(GL_SHADER_STORAGE_BUFFER, p.ssbo);
  GL::glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shader_data), &shader_data, GL_DYNAMIC_COPY);
}

void OpenGLRectangleLightGroup::draw()
{
  P(OpenGLRectangleLightGroupPrivate);
  if (p.m_lights.empty()) return;
  p.m_mesh.bind();
  p.m_program->bind();

  GLuint block_index = 0;
  block_index = GL::glGetProgramResourceIndex(p.m_program->programId(), GL_SHADER_STORAGE_BLOCK, "BufferObject");
  GL::glShaderStorageBlockBinding(p.m_program->programId(), block_index, 10);
  GL::glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, p.ssbo);

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

  /*
  GL::glBindBuffer(GL_SHADER_STORAGE_BUFFER, p.ssbo);
  GLvoid* pointer = GL::glMapRange(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
  memcpy(&shader_data, pointer, sizeof(shader_data));
  GL::glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  KVector3D wPos = KVector3D(shader_data.worldPosition[0], shader_data.worldPosition[1], shader_data.worldPosition[2]);
  KVector3D vVector = KVector3D(shader_data.viewVector[0], shader_data.viewVector[1], shader_data.viewVector[2]);
  KVector3D rVector = KVector3D(shader_data.reflectionVector[0], shader_data.reflectionVector[1], shader_data.reflectionVector[2]);
  KVector3D nVector = KVector3D(shader_data.normalVector[0], shader_data.normalVector[1], shader_data.normalVector[2]);
  KVector3D sPos = KVector3D(shader_data.mrpPoint[0], shader_data.mrpPoint[1], shader_data.mrpPoint[2]);
  KVector3D eMajor = KVector3D(shader_data.ellipseMajor[0], shader_data.ellipseMajor[1], shader_data.ellipseMajor[2]);
  KVector3D eMinor = KVector3D(shader_data.ellipseMinor[0], shader_data.ellipseMinor[1], shader_data.ellipseMinor[2]);
  KVector3D eNormal = KVector3D(shader_data.ellipseNormal[0], shader_data.ellipseNormal[1], shader_data.ellipseNormal[2]);
  KVector3D bottomLeft = KVector3D(shader_data.bottomLeft[0], shader_data.bottomLeft[1], shader_data.bottomLeft[2]);
  KVector3D topRight = KVector3D(shader_data.topRight[0], shader_data.topRight[1], shader_data.topRight[2]);
  KVector3D bottomRight = KVector3D(shader_data.bottomRight[0], shader_data.bottomRight[1], shader_data.bottomRight[2]);
  KVector3D topLeft = KVector3D(shader_data.topLeft[0], shader_data.topLeft[1], shader_data.topLeft[2]);

  KVector3D mrpUnclamped = KVector3D(shader_data.mrpUnclamped[0], shader_data.mrpUnclamped[1], shader_data.mrpUnclamped[2]);
  OpenGLDebugDraw::World::drawSphere(wPos, 0.1f, Qt::green);
  OpenGLDebugDraw::World::drawSphere(sPos, 0.1f, Qt::red);
  OpenGLDebugDraw::World::drawSphere(mrpUnclamped, 0.1f, Qt::yellow);
  OpenGLDebugDraw::World::drawLine(wPos, wPos + vVector, Qt::green);
  OpenGLDebugDraw::World::drawLine(wPos, wPos + nVector, Qt::yellow);
  OpenGLDebugDraw::World::drawLine(wPos, wPos + rVector, Qt::red);
  OpenGLDebugDraw::World::drawOval(mrpUnclamped, eNormal, eMajor, eMinor, Qt::green);
  OpenGLDebugDraw::World::drawQuad(bottomLeft, bottomRight, topLeft, topRight, Qt::green);
  */
}

OpenGLRectangleLight *OpenGLRectangleLightGroup::createLight()
{
  P(OpenGLRectangleLightGroupPrivate);
  OpenGLRectangleLight *light = new OpenGLRectangleLight;
  p.m_lights.push_back(light);
  return light;
}

OpenGLRectangleLightGroup::SizeType OpenGLRectangleLightGroup::size() const
{
  P(const OpenGLRectangleLightGroupPrivate);
  return p.m_lights.size();
}

OpenGLRectangleLight *OpenGLRectangleLightGroup::operator[](int idx)
{
  P(OpenGLRectangleLightGroupPrivate);
  return p.m_lights[idx];
}

auto OpenGLRectangleLightGroup::begin() -> LightContainer::iterator
{
  P(OpenGLRectangleLightGroupPrivate);
  return p.m_lights.begin();
}

auto OpenGLRectangleLightGroup::end() -> LightContainer::iterator
{
  P(OpenGLRectangleLightGroupPrivate);
  return p.m_lights.end();
}
