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
  typedef typename LightContainer::iterator LightIterator;
  typedef typename LightContainer::const_iterator ConstLightIterator;
  typedef typename LightContainer::reverse_iterator LightReverseIterator;
  typedef typename LightContainer::const_reverse_iterator ConstLightReverseIterator;

  void prepMesh(OpenGLMesh &mesh);
  void update(const KMatrix4x4 &perspective, const KMatrix4x4 &view);
  virtual void initializeMesh(OpenGLMesh &mesh) = 0;
  virtual void translateData(const KMatrix4x4 &perspective, const KMatrix4x4 &view, DataPointer data) = 0;

  // Light Factory Methods
  bool create();
  LightPointer createLight();
  const LightContainer &lights() const;

  // Light Query Methods
  LightIterator begin();
  LightIterator end();
  ConstLightIterator begin() const;
  ConstLightIterator end() const;
  ConstLightIterator cbegin() const;
  ConstLightIterator cend() const;
  LightReverseIterator rbegin();
  LightReverseIterator rend();
  ConstLightReverseIterator rbegin() const;
  ConstLightReverseIterator rend() const;
  ConstLightReverseIterator crbegin() const;
  ConstLightReverseIterator crend() const;

protected:
  BufferType m_buffer;
  LightContainer m_lights;
};

template <typename T, typename D>
void OpenGLLightGroup<T, D>::prepMesh(OpenGLMesh &mesh)
{
  m_buffer.bind();
  initializeMesh(mesh);
}

template <typename T, typename D>
void OpenGLLightGroup<T, D>::update(const KMatrix4x4 &perspective, const KMatrix4x4 &view)
{
  if (m_lights.empty()) return;

  m_buffer.bind();
  m_buffer.reserve(m_lights.size());
  BufferType::RangeAccessFlags flags =
      BufferType::RangeInvalidate
    | BufferType::RangeUnsynchronized
    | BufferType::RangeWrite;
  DataPointer data = m_buffer.mapRange(0, m_lights.size(), flags);

  if (data == NULL)
  {
    qFatal("Failed to map the buffer range!");
  }

  translateData(perspective, view, data);

  m_buffer.unmap();
  m_buffer.release();
}

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

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::begin() -> LightIterator
{
  return m_lights.begin();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::end() -> LightIterator
{
  return m_lights.end();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::begin() const -> ConstLightIterator
{
  return m_lights.begin();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::end() const -> ConstLightIterator
{
  return m_lights.end();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::cbegin() const -> ConstLightIterator
{
  return m_lights.cbegin();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::cend() const -> ConstLightIterator
{
  return m_lights.cend();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::rbegin() -> LightReverseIterator
{
  return m_lights.rbegin();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::rend() -> LightReverseIterator
{
  return m_lights.rend();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::rbegin() const -> ConstLightReverseIterator
{
  return m_lights.rbegin();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::rend() const -> ConstLightReverseIterator
{
  return m_lights.rend();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::crbegin() const -> ConstLightReverseIterator
{
  return m_lights.crbegin();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::crend() const -> ConstLightReverseIterator
{
  return m_lights.crend();
}

#endif // OPENGLLIGHTGROUP_H
