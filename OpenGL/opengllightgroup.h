#ifndef OPENGLLIGHTGROUP_H
#define OPENGLLIGHTGROUP_H OpenGLLightGroup

#include <vector>
#include <OpenGLMesh>
#include <OpenGLDynamicBuffer>
#include <OpenGLAbstractLightGroup>

template <typename T, typename D>
class OpenGLLightGroup : public OpenGLAbstractLightGroup
{
public:

  // Light Types
  typedef T LightType;
  typedef T* LightPointer;
  typedef T& LightReference;
  typedef D DataType;
  typedef D* DataPointer;
  typedef D& DataReference;
  typedef OpenGLDynamicBuffer<DataType> BufferType;
  typedef std::vector<LightPointer> LightContainer;

  bool create();
  LightPointer createLight();
  const LightContainer &lights() const;

protected:
  OpenGLMesh m_mesh;
  BufferType m_buffer;
  LightContainer m_lights;
};

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::create() -> bool
{
  m_buffer.setUsagePattern(BufferType::DynamicDraw);
  return m_buffer.create();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::createLight() -> LightPointer
{
  LightPointer light = new LightType();
  m_lights.emplace_back(light);
  return light;
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::lights() const -> const LightContainer&
{
  return m_lights;
}

#endif // OPENGLLIGHTGROUP_H
