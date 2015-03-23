#ifndef OPENGLLIGHTGROUP_H
#define OPENGLLIGHTGROUP_H OpenGLLightGroup

#include <vector>
#include <KRectF>
#include <OpenGLMesh>
#include <OpenGLDynamicBuffer>
#include <OpenGLAbstractLightGroup>
#include <OpenGLLight>
#include <OpenGLUniformBufferObject>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>
#include <OpenGLLightData>
#include <OpenGLScene>
#include <OpenGLFramebufferObject>
#include <OpenGLDebugDraw>
#include <KPoint>

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
  void drawShadowed(OpenGLScene &scene);
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
  std::vector<OpenGLViewport> m_viewports;
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

  if (m_viewports.size() < m_numShadowLights)
  {
    m_viewports.reserve(m_numShadowLights);
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

  m_mesh.release();
}
#include <KInputManager>
#include <OpenGLBlurData>
template <typename T, typename D>
void OpenGLLightGroup<T, D>::drawShadowed(OpenGLScene &scene)
{
  if (m_lights.empty()) return;

  // Activate the shadow texture
  GL::glActiveTexture(GL_TEXTURE0);
  m_shadowTexture.bind();

  // Render each shadow light
  for (size_t i = 0; i < m_numShadowLights; ++i)
  {
    m_uniforms.bindRange(BufferType::UniformBuffer, 3, static_cast<int>(m_uniformOffset * i), static_cast<int>(sizeof(DataType)));

    // Draw from Light's Perspective
    OpenGLFramebufferObject::push();
    GL::pushViewport();
      GL::glViewport(0, 0, 800, 600);
      m_shadowMappingFbo.bind();
      m_shadowMappingLight->bind();
      GL::glClearColor(std::numeric_limits<float>::infinity(), 1.0, 1.0f, 1.0f);
      GL::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      GL::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      scene.renderGeometry();
      m_shadowMappingLight->release();
    GL::popViewport();
    OpenGLFramebufferObject::pop();

    // Next: Blur the shadow map
    int W = 800;
    int H = 600;
    GLint loc = m_blurProgram->uniformLocation("Direction");
    m_blurProgram->bind();
    GL::glBindImageTexture(0, m_shadowTexture.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
    GL::glBindImageTexture(1, m_blurTexture.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    GL::glUniform2i(loc, 1, 0);
    GL::glDispatchCompute(std::ceil(float(W) / 128), H, 1);
    GL::glBindImageTexture(0, m_blurTexture.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
    GL::glBindImageTexture(1, m_shadowTexture.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    GL::glUniform2i(loc, 0, 1);
    GL::glDispatchCompute(std::ceil(float(H) / 128), W, 1);
    m_blurProgram->release();

    // Draw from Camera's Perspective
    m_mesh.bind();
      m_shadowCastingLight->bind();
      GL::glDisable(GL_DEPTH_TEST);
      GL::glEnable(GL_BLEND);
      GL::glBlendFunc(GL_ONE, GL_ONE);
      m_mesh.draw();
      GL::glDisable(GL_BLEND);
      GL::glEnable(GL_DEPTH_TEST);
      m_shadowCastingLight->release();
    m_mesh.release();
  }
}

template <typename T, typename D>
auto OpenGLLightGroup<T, D>::create() -> bool
{
  m_uniforms.setUsagePattern(BufferType::DynamicDraw);
  m_buffer.setUsagePattern(BufferType::DynamicDraw);
  return m_buffer.create() && m_uniforms.create() && OpenGLAbstractLightGroup::create();
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
