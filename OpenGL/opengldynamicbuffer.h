#ifndef OPENGLDYNAMICBUFFER_H
#define OPENGLDYNAMICBUFFER_H OpenGLDynamicBuffer

#include <OpenGLBuffer>

template <typename T>
class OpenGLDynamicBuffer : public OpenGLBuffer
{
public:
  typedef T ElementType;
  typedef T* ElementPointer;
  typedef T& ElementReference;
  typedef size_t SizeType;

  OpenGLDynamicBuffer(OpenGLBuffer::Type type = OpenGLBuffer::VertexBuffer);
  void reserve(size_t elements);
  ElementPointer mapRange(size_t offset, size_t count, RangeAccessFlags access);
  void bindRangeElement(Type type, unsigned index, unsigned element);
  SizeType count() const;
};

template <typename T>
OpenGLDynamicBuffer<T>::OpenGLDynamicBuffer(OpenGLBuffer::Type type) :
  OpenGLBuffer(type)
{
  // Intentionally Empty
}

template <typename T>
void OpenGLDynamicBuffer<T>::reserve(size_t elements)
{
  size_t sizeInBytes = sizeof(ElementType) * elements;
  if (!isCreated() || size() < static_cast<int>(sizeInBytes))
  {
    allocate(sizeInBytes);
  }
}

template <typename T>
auto OpenGLDynamicBuffer<T>::mapRange(size_t offset, size_t count, RangeAccessFlags access) -> ElementPointer
{
  return static_cast<ElementPointer>(OpenGLBuffer::mapRange(offset, sizeof(ElementType) * count, access));
}

template <typename T>
void OpenGLDynamicBuffer<T>::bindRangeElement(Type type, unsigned index, unsigned element)
{
  OpenGLBuffer::bindRange(type, index, sizeof(ElementType) * element, sizeof(ElementType));
}

template <typename T>
auto OpenGLDynamicBuffer<T>::count() const -> SizeType
{
  return size() / sizeof(ElementType);
}

#endif // OPENGLDYNAMICBUFFER_H

