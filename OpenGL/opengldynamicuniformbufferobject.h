#ifndef OPENGLDYNAMICUNIFORMBUFFEROBJECT_H
#define OPENGLDYNAMICUNIFORMBUFFEROBJECT_H OpenGLDynamicUniformBufferObject

#include <OpenGLUniformBufferObject>

template <typename T>
class OpenGLDynamicUniformBufferObject : public OpenGLUniformBufferObject
{
public:
  typedef T ElementType;
  bool create();
  void reserve(int count);
  int skipSize() const;
private:
  int m_elementSize;
};

template <typename T>
inline bool OpenGLDynamicUniformBufferObject<T>::create()
{
  int elementSize = sizeof(ElementType);
  int alignment = OpenGLUniformBufferObject::alignmentOffset();
  m_elementSize = std::ceil(float(elementSize) / alignment) * alignment;
  return OpenGLUniformBufferObject::create();
}

template <typename T>
inline void OpenGLDynamicUniformBufferObject<T>::reserve(int count)
{
  int requested = m_elementSize * count;
  if (size() < requested)
  {
    OpenGLUniformBufferObject::allocate(requested);
  }
}

template <typename T>
inline int OpenGLDynamicUniformBufferObject<T>::skipSize() const
{
  return m_elementSize;
}

#endif // OPENGLDYNAMICUNIFORMBUFFEROBJECT_H

