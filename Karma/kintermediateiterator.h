#ifndef KINTERMEDIATEITERATOR_H
#define KINTERMEDIATEITERATOR_H KIntermediateIterator

#include <iterator>
#include <type_traits>

template <typename To, typename From, typename FromConst>
class KIntermediateIterator
{
public:
  typedef size_t difference_type;
  typedef From FromIterator;
  typedef FromConst FromIteratorConst;
  typedef To ToIterator;

  // IMPORTANT:
  // std::array::iterator was originally implemented as just a pointer.
  // This means some compilers may/may-not have the To::reference
  // typedef (which is what we want).
  // To account for this, we will abuse declype by dereferencing
  // a NULL value to "produce" the reference type.
  // C++17>= code should probably opt for To::reference typedef.
  typedef decltype(**((typename To*)NULL))& reference;

  explicit KIntermediateIterator(FromIterator from);
  explicit KIntermediateIterator(FromIteratorConst from);
  virtual void validate() = 0;
  bool operator!=(KIntermediateIterator const &rhs) const;
  reference operator*();
  ToIterator const &toIterator() const;
  FromIterator const &fromIterator() const;
protected:
  FromIterator m_from;
  ToIterator m_to;
  void invalidate();
private:
  bool m_valid;
  void makeValid();
};

template <typename To, typename From, typename FromConst>
KIntermediateIterator<To, From, FromConst>::KIntermediateIterator(FromIterator from) :
  m_from(from), m_valid(false)
{
  // Intentionally Empty
}

template <typename To, typename From, typename FromConst>
KIntermediateIterator<To, From, FromConst>::KIntermediateIterator(FromIteratorConst from) :
  m_from(*reinterpret_cast<FromIterator*>(&from)), m_valid(false)
{
  // Intentionally Empty
}

template <typename To, typename From, typename FromConst>
inline bool KIntermediateIterator<To, From, FromConst>::operator!=(KIntermediateIterator const &rhs) const
{
  return (m_from != rhs.m_from);
}

template <typename To, typename From, typename FromConst>
auto KIntermediateIterator<To, From, FromConst>::operator*() -> reference
{
  makeValid();
  return *m_to;
}

template <typename To, typename From, typename FromConst>
auto KIntermediateIterator<To, From, FromConst>::toIterator() const -> ToIterator const &
{
  makeValid();
  return m_to;
}

template <typename To, typename From, typename FromConst>
auto KIntermediateIterator<To, From, FromConst>::fromIterator() const -> FromIterator const &
{
  return m_from;
}

template <typename To, typename From, typename FromConst>
inline void KIntermediateIterator<To, From, FromConst>::invalidate()
{
  m_valid = false;
}

template <typename To, typename From, typename FromConst>
inline void KIntermediateIterator<To, From, FromConst>::makeValid()
{
  if (!m_valid)
  {
    m_valid = true;
    validate();
  }
}

#endif // KINTERMEDIATEITERATOR_H
