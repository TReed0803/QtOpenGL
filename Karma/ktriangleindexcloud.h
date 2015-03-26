#ifndef KTRIANGLEINDEXCLOUD_H
#define KTRIANGLEINDEXCLOUD_H KTriangleIndexCloud

#include <cstddef>
#include <vector>
#include <array>

class KTriangleIndexCloud
{
public:

  struct ElementType
  {
    typedef size_t IndexType;
    typedef std::array<IndexType, 3> IndicesContainer;
    typedef IndicesContainer::iterator Iterator;
    typedef IndicesContainer::const_iterator ConstIterator;
    ElementType();
    ElementType(IndexType e0, IndexType e1, IndexType e2);
    ElementType offset(IndexType offset) const;
    IndicesContainer indices;
  };
  typedef std::vector<ElementType> ContainerType;
  typedef ContainerType::iterator Iterator;
  typedef ContainerType::const_iterator ConstIterator;

  void reserve(size_t count);
  void emplace_back(ElementType const &elm);
  size_t size() const;
  void clear();
  bool empty() const;
  template <typename It>
  void copy(It begin, It end);

  // Iterators
  ConstIterator cbegin() const;
  ConstIterator cend() const;
  ConstIterator begin() const;
  ConstIterator end() const;
  Iterator begin();
  Iterator end();
private:
  ContainerType m_container;
};

inline KTriangleIndexCloud::ElementType::ElementType()
{
  indices[0] = indices[1] = indices[2] = 0;
}

inline KTriangleIndexCloud::ElementType::ElementType(IndexType e0, IndexType e1, IndexType e2)
{
  indices[0] = e0;
  indices[1] = e1;
  indices[2] = e2;
}

inline auto KTriangleIndexCloud::ElementType::offset(IndexType offset) const -> ElementType
{
  return ElementType
  (
    indices[0] + offset,
    indices[1] + offset,
    indices[2] + offset
  );
}

inline void KTriangleIndexCloud::reserve(size_t count)
{
  m_container.reserve(count);
}

inline void KTriangleIndexCloud::emplace_back(ElementType const &elm)
{
  m_container.emplace_back(elm);
}

inline size_t KTriangleIndexCloud::size() const
{
  return m_container.size();
}

inline void KTriangleIndexCloud::clear()
{
  m_container.clear();
}

inline bool KTriangleIndexCloud::empty() const
{
  return m_container.empty();
}

inline auto KTriangleIndexCloud::cbegin() const -> ConstIterator
{
  return m_container.cbegin();
}

inline auto KTriangleIndexCloud::cend() const -> ConstIterator
{
  return m_container.cend();
}

inline auto KTriangleIndexCloud::begin() const -> ConstIterator
{
  return m_container.begin();
}

inline auto KTriangleIndexCloud::end() const -> ConstIterator
{
  return m_container.end();
}

inline auto KTriangleIndexCloud::begin() -> Iterator
{
  return m_container.begin();
}

inline auto KTriangleIndexCloud::end() -> Iterator
{
  return m_container.end();
}

template <typename It>
void KTriangleIndexCloud::copy(It begin, It end)
{
  while (begin != end)
  {
    m_container.push_back(*begin);
    ++begin;
  }
}

#endif // KTRIANGLEINDEXCLOUD_H
