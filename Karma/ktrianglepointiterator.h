#ifndef KTRIANGLEPOINTITERATOR_H
#define KTRIANGLEPOINTITERATOR_H KTrianglePointIterator

#include <KVector3D>
#include <KPointCloud>
#include <KTriangleIndexIterator>
#include <KTranslationIterator>

class KTrianglePointIterator : public KTranslationIterator<KTriangleIndexIterator, KVector3D>
{
public:
  KTrianglePointIterator(IteratorType it, KPointCloud const &cloud);
  void operator++();
  bool operator!=(KTrianglePointIterator const &rhs) const;
  ValueType const &operator*();
private:
  KPointCloud const &m_cloud;
  IteratorType m_iterator;
};

inline KTrianglePointIterator::KTrianglePointIterator(KTriangleIndexIterator it, KPointCloud const &cloud) :
  m_cloud(cloud), m_iterator(it)
{
  // Intentionally Empty
}

inline void KTrianglePointIterator::operator++()
{
  ++m_iterator;
}

inline bool KTrianglePointIterator::operator!=(KTrianglePointIterator const &rhs) const
{
  return (m_iterator != rhs.m_iterator);
}

inline auto KTrianglePointIterator::operator*() -> ValueType const &
{
  // Reminder: FaceIndices start from 1
  return m_cloud[(*m_iterator) - 1];
}

#endif // KTRIANGLEPOINTITERATOR_H
