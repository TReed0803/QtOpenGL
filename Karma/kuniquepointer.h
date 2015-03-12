#ifndef KUNIQUEPOINTER_H
#define KUNIQUEPOINTER_H KUniquePointer

template <typename T>
class KUniquePointer
{
public:
  typedef T ValueType;
  typedef T* ValuePointer;
  typedef T& ValueReference;
  typedef const T ConstValueType;
  typedef const T* ConstValuePointer;
  typedef const T& ConstValueReference;
  KUniquePointer();
  KUniquePointer(ValuePointer value);
  ~KUniquePointer();
  ValueReference operator*();
  ConstValueReference operator*() const;
  ValueReference operator->();
  ConstValueReference operator->() const;
  void operator=(ValuePointer value);
private:
  ValuePointer m_data;
};

template <typename T>
KUniquePointer<T>::KUniquePointer() :
  m_data(0)
{
  // Intentionally Empty
}

template <typename T>
KUniquePointer<T>::KUniquePointer(ValuePointer value) :
  m_data(value)
{
  // Intentionally Empty
}

template <typename T>
KUniquePointer<T>::~KUniquePointer()
{
  delete m_data;
}

template <typename T>
auto KUniquePointer<T>::operator*() -> ValueReference
{
  return *m_data;
}

template <typename T>
auto KUniquePointer<T>::operator*() const -> ConstValueReference
{
  return *m_data;
}

template <typename T>
auto KUniquePointer<T>::operator->() -> ValueReference
{
  return *m_data;
}

template <typename T>
auto KUniquePointer<T>::operator->() const -> ConstValueReference
{
  return *m_data;
}

template <typename T>
void KUniquePointer<T>::operator=(ValuePointer value)
{
  delete m_data;
  m_data = value;
}

#endif // KUNIQUEPOINTER_H

