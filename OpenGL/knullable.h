#ifndef KNULLABLE_H
#define KNULLABLE_H KNullable

#include <cstddef>

template <typename T>
class KNullable
{
public:
  typedef T ValueType;
  typedef const T ConstValueType;
  typedef T* PointerType;
  typedef const T* ConstPointerType;

  KNullable();
  KNullable(ConstValueType &value);

  // Casting Operations
  explicit operator bool() const;
  operator ValueType&();
  operator ConstValueType&() const;
  PointerType operator->();
  ConstPointerType operator->() const;
  ValueType &operator*();
  ConstValueType &operator*() const;

  // Assignment operators
  void operator=(ConstValueType &value);
  void operator=(const std::nullptr_t &nil);

private:
  bool m_valid;
  ValueType m_value;
};

template <typename T>
KNullable<T>::KNullable() :
  m_valid(false)
{
  // Intentionally Empty
}

template <typename T>
KNullable<T>::KNullable(ConstValueType &value)
  : m_valid(true), m_value(value)
{
  // Intentionally Empty
}

template <typename T>
KNullable<T>::operator bool() const
{
  return m_valid;
}

template <typename T>
KNullable<T>::operator ValueType&()
{
  return m_value;
}

template <typename T>
KNullable<T>::operator ConstValueType&() const
{
  return m_value;
}

template <typename T>
auto KNullable<T>::operator->() -> PointerType
{
  return &m_value;
}

template <typename T>
auto KNullable<T>::operator->() const -> ConstPointerType
{
  return &m_value;
}

template <typename T>
auto KNullable<T>::operator*() -> ValueType&
{
  return m_value;
}

template <typename T>
auto KNullable<T>::operator*() const -> ConstValueType&
{
  return m_value;
}

template <typename T>
void KNullable<T>::operator=(ConstValueType &value)
{
  m_valid = true;
  m_value = value;
}

template <typename T>
void KNullable<T>::operator=(const std::nullptr_t &nil)
{
  (void)nil;
  m_valid = false;
}

#endif // KNULLABLE_H

