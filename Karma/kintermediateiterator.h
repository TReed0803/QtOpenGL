#ifndef KINTERMEDIATEITERATOR_H
#define KINTERMEDIATEITERATOR_H KIntermediateIterator

#include <iterator>

template <typename To, typename From>
class KIntermediateIterator :
    public std::iterator<
      typename To::iterator_category,
      typename To::value_type,
      typename To::difference_type,
      typename To::pointer,
      typename To::reference>
{
public:
  typedef From FromIterator;
  typedef To ToIterator;

  explicit KIntermediateIterator(FromIterator from);
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

template <typename To, typename From>
KIntermediateIterator<To, From>::KIntermediateIterator(FromIterator from) :
  m_from(from), m_valid(false)
{
  // Intentionally Empty
}

template <typename To, typename From>
inline bool KIntermediateIterator<To, From>::operator!=(KIntermediateIterator const &rhs) const
{
  return (m_from != rhs.m_from);
}

template <typename To, typename From>
auto KIntermediateIterator<To, From>::operator*() -> reference
{
  makeValid();
  return *m_to;
}

template <typename To, typename From>
auto KIntermediateIterator<To, From>::toIterator() const -> ToIterator const &
{
  makeValid();
  return m_to;
}

template <typename To, typename From>
auto KIntermediateIterator<To, From>::fromIterator() const -> FromIterator const &
{
  return m_from;
}

template <typename To, typename From>
inline void KIntermediateIterator<To, From>::invalidate()
{
  m_valid = false;
}

template <typename To, typename From>
inline void KIntermediateIterator<To, From>::makeValid()
{
  if (!m_valid)
  {
    m_valid = true;
    validate();
  }
}

#endif // KINTERMEDIATEITERATOR_H
