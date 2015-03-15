#ifndef KSHAREDPOINTER_H
#define KSHAREDPOINTER_H KSharedPointer

#include <cstdint>
#include <KTypeTraits>

template <typename T>
class KSharedPointer
{
public:
  typedef uint32_t SizeType;
  typedef T  ValueType;
  typedef T* PointerType;
  typedef T& ReferenceType;
  typedef const T  ConstValueType;
  typedef const T* ConstPointerType;
  typedef const T& ConstReferenceType;

  struct ReferenceContainer
  {
    ReferenceContainer(PointerType ptr);
    ~ReferenceContainer();
    SizeType m_references;
    PointerType m_internal;
  };

  KSharedPointer();
  KSharedPointer(const KSharedPointer &rhs);
  explicit KSharedPointer(PointerType data);
  ~KSharedPointer();
  PointerType operator->();
  ReferenceType operator*();
  ConstPointerType operator->() const;
  ConstReferenceType operator*() const;
  void operator=(const KSharedPointer &rhs);
  void operator=(PointerType rhs);

private:
  ReferenceContainer *m_data;
};

template <typename T>
KSharedPointer<T>::ReferenceContainer::ReferenceContainer(PointerType ptr) :
  m_references(1), m_internal(ptr)
{
  // Intentionally Empty
}

template <typename T>
KSharedPointer<T>::ReferenceContainer::~ReferenceContainer()
{
  Karma::safeDelete(m_internal);
}

template <typename T>
KSharedPointer<T>::KSharedPointer() :
  m_data(0)
{
  // Intentionally Empty
}

template <typename T>
KSharedPointer<T>::KSharedPointer(const KSharedPointer &ptr) :
  m_data(ptr.m_data)
{
  if (m_data)
  {
    ++m_data->m_references;
  }
}

template <typename T>
KSharedPointer<T>::KSharedPointer(PointerType data) :
  m_data(new ReferenceContainer(data))
{
  // Intentionally Empty
}

template <typename T>
KSharedPointer<T>::~KSharedPointer()
{
  if (m_data)
  {
    --m_data->m_references;
    if (m_data->m_references == 0)
    {
      delete m_data;
    }
  }
}

template <typename T>
auto KSharedPointer<T>::operator->() -> PointerType
{
  return m_data->m_internal;
}

template <typename T>
auto KSharedPointer<T>::operator*() -> ReferenceType
{
  return *m_data->m_internal;
}

template <typename T>
auto KSharedPointer<T>::operator->() const -> ConstPointerType
{
  return m_data->m_internal;
}

template <typename T>
auto KSharedPointer<T>::operator*() const -> ConstReferenceType
{
  return *m_data->m_internal;
}

template <typename T>
void KSharedPointer<T>::operator=(const KSharedPointer &rhs)
{
  if (m_data) --m_data->m_references;
  m_data = rhs.m_data;
  if (m_data) ++m_data->m_references;
}

template <typename T>
void KSharedPointer<T>::operator=(PointerType rhs)
{
  if (m_data) --m_data->m_references;
  m_data = new ReferenceContainer(rhs);
}

#endif // KSHAREDPOINTER_H
