#ifndef KTRIANGLEINDEXITERATOR_H
#define KTRIANGLEINDEXITERATOR_H KTriangleIndexIterator

#include <iterator>
#include <KTriangleIndexCloud>
#include <KIntermediateIterator>

class KTriangleIndexIterator : public KIntermediateIterator<KTriangleIndexCloud::ElementType::ConstIterator, KTriangleIndexCloud::Iterator, KTriangleIndexCloud::ConstIterator>
{
public:
  KTriangleIndexIterator(FromIterator it);
  KTriangleIndexIterator(FromIteratorConst it);
  void operator++();
  void validate();
};

inline KTriangleIndexIterator::KTriangleIndexIterator(FromIterator it) :
  KIntermediateIterator(it)
{
  // Intentionally Empty
}

inline KTriangleIndexIterator::KTriangleIndexIterator(FromIteratorConst it) :
  KIntermediateIterator(it)
{
  // Intentionally Empty
}

inline void KTriangleIndexIterator::operator++()
{
  ++m_to;
  if (m_to == m_from->indices.cend())
  {
    ++m_from;
    invalidate();
  }
}

inline void KTriangleIndexIterator::validate()
{
  m_to = m_from->indices.cbegin();
}

namespace std
{
  inline KTriangleIndexIterator::difference_type
  distance(KTriangleIndexIterator const & begin, KTriangleIndexIterator const &end)
  {
    return 3 * std::distance(begin.fromIterator(), end.fromIterator());
  }
}

#endif // KTRIANGLEINDEXITERATOR_H
