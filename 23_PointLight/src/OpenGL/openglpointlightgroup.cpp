#include "openglpointlightgroup.h"

#include <OpenGLVertexArrayObject>
#include <OpenGLFunctions>
#include <OpenGLMesh>
#include <OpenGLBuffer>
#include <cstdint>
#include <vector>
#include <KColor>
#include <KMatrix4x4>
#include <KVector3D>

class OpenGLPointLightGroupPrivate
{
public:
  typedef std::vector<OpenGLPointLight*> LightList;

  OpenGLPointLightGroupPrivate();

  uint64_t m_bufferSize;
  OpenGLMesh *m_mesh;
  OpenGLBuffer m_buffer;
  LightList m_instances;
  OpenGLFunctions m_functions;
};

OpenGLPointLightGroupPrivate::OpenGLPointLightGroupPrivate() :
  m_bufferSize(0), m_mesh(Q_NULLPTR)
{
  m_functions.initializeOpenGLFunctions();
  m_buffer.setUsagePattern(OpenGLBuffer::DynamicDraw);
  m_buffer.create();
}

#define P(c) c &p = *m_private
OpenGLPointLightGroup::OpenGLPointLightGroup(QObject *parent) :
  QObject(parent), m_private(new OpenGLPointLightGroupPrivate)
{
  // Intentionally Empty
}
#include <KTransform3D>
#define M_2PI   6.2831853071795
#define M_PI    3.1415926535897
#define M_SQRT2 1.4142135623730

double sec(double x)
{
  return (2.0 * cos(x)) / (cos(2.0 * x) + 1.0);
}

float CalculateScalar(int segments, int rings)
{
  if (rings % 2)
    return sqrt(pow(sec(M_PI/segments),2.0f) * pow(sec(M_PI/(2*rings)), 2.0f));
  return M_SQRT2 * sqrt(1.0f / (1.0f + cos(M_2PI / segments) * cos(M_PI / rings)));
}

float TestUV(int segments, int rings)
{
  float longitude = M_2PI / float(segments);
  float latitude = M_PI / float(rings);
  float latitudeA, latitudeB;
  if (rings % 2)
  {
    latitudeA = latitude / 2.0f;
    latitudeB = -latitudeA;
  }
  else
  {
    latitudeA = latitude;
    latitudeB = 0.0f;
  }
  float sideLength = sqrt(2.0f - 2.0f * (cos(latitudeA) * cos(latitudeB) * cos(longitude) + sin(latitudeA) * sin(latitudeB)));
  float halfSideLength = sideLength / 2.0f;
  float linearDepth = sqrt(1.0f - pow(halfSideLength, 2.0));
  float similarScalar = 1.0f / linearDepth;
  float similarHalfLength = halfSideLength * similarScalar;
  float finalValue = sqrt(similarHalfLength * similarHalfLength + 1);
  float TSideLength = sqrt(3.0f-cos(M_2PI/segments)-2.0f*pow(cos(M_PI/segments),2.0f)*cos(M_PI/rings));
  return finalValue;
}

float TestUV1(int segments)
{
  float arcLength = M_2PI / float(segments);
  float sideLength = sqrt(2.0f - 2.0f * cos(arcLength));
  float halfSideLength = sideLength / 2.0f;
  float linearDepth = sqrt(1.0f - pow(halfSideLength, 2.0));
  float similarScalar = 1.0f / linearDepth;
  float similarHalfLength = halfSideLength * similarScalar;
  float finalValue = sqrt(similarHalfLength * similarHalfLength + 1);
  return finalValue;
}

void OpenGLPointLightGroup::update(const KMatrix4x4 &perspective, const KMatrix4x4 &worldToCamera)
{
  P(OpenGLPointLightGroupPrivate);
  uint64_t required = 0;
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:translation
  required += p.m_instances.size() * sizeof(GLfloat) * 4;  // vec3:attenuation
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:ambient
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:diffuse
  required += p.m_instances.size() * sizeof(GLfloat) * 3;  // vec3:specular
  required += p.m_instances.size() * sizeof(GLfloat) * 16; // vec3:modelToCamera

  std::vector<float> instanceInfo;
  instanceInfo.reserve(required);
  for (OpenGLPointLight *instance : p.m_instances)
  {
    KTransform3D t;
    float test = TestUV(8, 8);
    t.setScale(instance->radius() * test);
    t.setTranslation(instance->translation());
    KVector3D const &translation = worldToCamera * instance->translation();
    KVector3D const &attenuation = instance->attenuation();
    KColor const &ambient  = instance->ambient();
    KColor const &diffuse  = instance->diffuse();
    KColor const &specular  = instance->specular();
    KMatrix4x4 modelToCamera = worldToCamera * t.toMatrix();
    KMatrix4x4 modelToView = perspective * modelToCamera;
    instanceInfo.push_back( translation.x() );
    instanceInfo.push_back( translation.y() );
    instanceInfo.push_back( translation.z() );
    instanceInfo.push_back( attenuation.x() );
    instanceInfo.push_back( attenuation.y() );
    instanceInfo.push_back( attenuation.z() );
    instanceInfo.push_back( instance->radius() );
    instanceInfo.push_back( ambient.redF() );
    instanceInfo.push_back( ambient.greenF() );
    instanceInfo.push_back( ambient.blueF() );
    instanceInfo.push_back( diffuse.redF() );
    instanceInfo.push_back( diffuse.greenF() );
    instanceInfo.push_back( diffuse.blueF() );
    instanceInfo.push_back( specular.redF() );
    instanceInfo.push_back( specular.greenF() );
    instanceInfo.push_back( specular.blueF() );
    for (int i = 0; i < 16; ++i)
      instanceInfo.push_back( modelToView.constData()[i] );
  }

  // Prepare OpenGL Buffer data
  if (p.m_bufferSize < required)
  {
    p.m_buffer.bind();
    p.m_buffer.allocate(instanceInfo.data(), required);
    p.m_buffer.release();
    p.m_bufferSize = required;
  }
  else
  {
    p.m_buffer.bind();
    p.m_buffer.write(0, instanceInfo.data(), required);
    p.m_buffer.release();
  }
}

void OpenGLPointLightGroup::draw()
{
  P(OpenGLPointLightGroupPrivate);
  p.m_mesh->vertexArrayObject()->bind();
  p.m_functions.glDrawElementsInstanced(p.m_mesh->mode(), p.m_mesh->count(), GL_UNSIGNED_INT, (void*)0, p.m_instances.size());
  p.m_mesh->vertexArrayObject()->release();
}

void OpenGLPointLightGroup::setMesh(OpenGLMesh *mesh)
{
  P(OpenGLPointLightGroupPrivate);
  p.m_mesh = mesh;
  OpenGLVertexArrayObject *obj = p.m_mesh->vertexArrayObject();
  obj->bind();
  p.m_buffer.bind();
  p.m_functions.glEnableVertexAttribArray(1);
  p.m_functions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, OpenGLPointLight::Stride(), (void*)(OpenGLPointLight::PositionOffset()));
  p.m_functions.glVertexAttribDivisor(1, 1);
  p.m_functions.glEnableVertexAttribArray(2);
  p.m_functions.glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, OpenGLPointLight::Stride(), (void*)(OpenGLPointLight::AttenuationOffset()));
  p.m_functions.glVertexAttribDivisor(2, 1);
  p.m_functions.glEnableVertexAttribArray(3);
  p.m_functions.glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, OpenGLPointLight::Stride(), (void*)(OpenGLPointLight::AmbientOffset()));
  p.m_functions.glVertexAttribDivisor(3, 1);
  p.m_functions.glEnableVertexAttribArray(4);
  p.m_functions.glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, OpenGLPointLight::Stride(), (void*)(OpenGLPointLight::DiffuseOffset()));
  p.m_functions.glVertexAttribDivisor(4, 1);
  p.m_functions.glEnableVertexAttribArray(5);
  p.m_functions.glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, OpenGLPointLight::Stride(), (void*)(OpenGLPointLight::SpecularOffset()));
  p.m_functions.glVertexAttribDivisor(5, 1);
  for (int i = 0; i < 4; ++i)
  {
    p.m_functions.glEnableVertexAttribArray(6 + i);
    p.m_functions.glVertexAttribPointer(6 + i, 4, GL_FLOAT, GL_FALSE, OpenGLPointLight::Stride(), (void*)(OpenGLPointLight::TransformOffset() + sizeof(GLfloat) * i * 4));
    p.m_functions.glVertexAttribDivisor(6 + i, 1);
  }
  obj->release();
}

OpenGLPointLight *OpenGLPointLightGroup::createLight()
{
  P(OpenGLPointLightGroupPrivate);
  OpenGLPointLight *instance = new OpenGLPointLight;
  p.m_instances.push_back(instance);
  return instance;
}

OpenGLPointLightGroup::LightList &OpenGLPointLightGroup::lights()
{
  P(OpenGLPointLightGroupPrivate);
  return p.m_instances;
}

const OpenGLPointLightGroup::LightList &OpenGLPointLightGroup::lights() const
{
  P(OpenGLPointLightGroupPrivate);
  return p.m_instances;
}
