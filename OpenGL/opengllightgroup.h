#ifndef OPENGLLIGHTGROUP_H
#define OPENGLLIGHTGROUP_H OpenGLLightGroup

#include <vector>
#include <OpenGLMesh>
#include <OpenGLDynamicBuffer>
#include <OpenGLAbstractLightGroup>
#include <OpenGLLight>
#include <OpenGLUniformBufferObject>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>

class OpenGLRenderBlock;

template <typename T, typename D>
class OpenGLLightGroup : public OpenGLAbstractLightGroup
{
public:

  // Light Types
  typedef OpenGLLightGroup<T, D> LightGroup;
  typedef T LightType;
  typedef T* LightPointer;
  typedef T& LightReference;
  typedef const T ConstLightType;
  typedef const T* ConstLightPointer;
  typedef const T& ConstLightRefernece;
  typedef D DataType;
  typedef D* DataPointer;
  typedef D& DataReference;
  typedef OpenGLDynamicBuffer<DataType> BufferType;
  typedef std::vector<LightPointer> LightContainer;
  typedef typename LightContainer::iterator LightIterator;
  typedef typename LightContainer::const_iterator ConstLightIterator;
  typedef typename LightContainer::reverse_iterator LightReverseIterator;
  typedef typename LightContainer::const_reverse_iterator ConstLightReverseIterator;
  typedef typename LightContainer::size_type SizeType;

  void prepMesh(OpenGLMesh &mesh);
  void commit(const OpenGLViewport &view);
  void draw();
  virtual void initializeMesh(OpenGLMesh &mesh) = 0;
  virtual void translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end) = 0;
  virtual void translateUniforms(const OpenGLRenderBlock &stats, Byte *data, SizeType step, ConstLightIterator begin, ConstLightIterator end) = 0;

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
  SizeType size() const;
  bool empty() const;

protected:
  BufferType m_buffer;
  OpenGLUniformBufferObject m_uniforms;
  unsigned m_uniformOffset;
  unsigned m_numShadowLights;
  unsigned m_numRegularLights;
  LightContainer m_lights;
};

template <typename T, typename D>
void OpenGLLightGroup<T, D>::prepMesh(OpenGLMesh &mesh)
{
  m_buffer.bind();
  initializeMesh(mesh);
}

template <typename T, typename D>
void OpenGLLightGroup<T, D>::commit(const OpenGLViewport &view)
{
  if (m_lights.empty()) return;

  // Seperate shadow-casters from regular lights
  LightIterator regularLights = std::partition(m_lights.begin(), m_lights.end(), OpenGLLight::ShadowCastingPred<true>());
  m_numShadowLights  = std::distance(m_lights.begin(), regularLights);
  m_numRegularLights = int(m_lights.size() - m_numShadowLights);

  BufferType::RangeAccessFlags flags =
      BufferType::RangeInvalidate
    | BufferType::RangeUnsynchronized
    | BufferType::RangeWrite;

  // Upload regular light information
  if (m_numRegularLights > 0)
  {
    m_buffer.bind();
    m_buffer.reserve(m_numRegularLights);
    DataPointer data = m_buffer.mapRange(0, m_numRegularLights, flags);

    if (data == NULL)
    {
      qFatal("Failed to map the buffer range!");
    }

    translateBuffer(view.current(), data, regularLights, m_lights.end());

    m_buffer.unmap();
    m_buffer.release();
  }

  // Upload uniform light information
  // Note: because UBOs have complicated alignments, we cannot cast to DataPointer.
  //       The UBO must increment preciecely by GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT.
  if (m_numShadowLights > 0)
  {
    m_uniforms.bind();
    m_uniformOffset = m_uniforms.reserve(sizeof(DataType), m_numShadowLights);
    Byte *data = static_cast<Byte*>(m_uniforms.mapRange(0, m_uniformOffset * m_numShadowLights, flags));

    if (data == NULL)
    {
      qFatal("Failed to map the buffer range!");
    }

    translateUniforms(view.current(), data, m_uniformOffset, m_lights.begin(), regularLights);

    m_uniforms.unmap();
    m_uniforms.release();
  }
}

template <typename T, typename D>
void OpenGLLightGroup<T, D>::draw()
{
  if (m_lights.empty()) return;

  m_mesh.bind();

  // Batch render regular lights
  m_regularLight->bind();
  m_mesh.drawInstanced(0, m_numRegularLights);

  // Render each shadow light
  if (m_shadowCastingLight)
  {
    m_shadowCastingLight->bind();
    for (size_t i = 0; i < m_numShadowLights; ++i)
    {
      m_uniforms.bindRange(BufferType::UniformBuffer, 3, static_cast<int>(m_uniformOffset * i), static_cast<int>(sizeof(DataType)));
      m_mesh.draw();
    }
  }

  m_mesh.release();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::create() -> bool
{
  m_uniforms.setUsagePattern(BufferType::DynamicDraw);
  m_buffer.setUsagePattern(BufferType::DynamicDraw);
  return m_buffer.create() && m_uniforms.create();
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

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::size() const -> SizeType
{
  return m_lights.size();
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::empty() const -> bool
{
  return m_lights.empty();
}

#endif // OPENGLLIGHTGROUP_H
