#ifndef KPOINTCLOUD_H
#define KPOINTCLOUD_H KPointCloud

#include <vector>
#include <KVector3D>
#include <KStandardAccessor>

class KPointCloud
{
public:
  typedef KVector3D ElementType;
  typedef std::vector<ElementType> ContainerType;
  typedef KStandardAccessor<ElementType> AccessorType;
  typedef ContainerType::const_iterator ConstIterator;

  void emplace_back(ElementType const &elm);
  ElementType &operator[](size_t elm);
  ElementType const &operator[](size_t elm) const;
  size_t size() const;
  bool empty() const;
  void clear();
  void reserve(size_t count);

  // Iterators
  ConstIterator cend() const;
private:
  ContainerType m_container;
};

inline void KPointCloud::emplace_back(ElementType const &elm)
{
  m_container.emplace_back(elm);
}

inline auto KPointCloud::operator[](size_t elm) -> ElementType&
{
  return m_container[elm];
}

inline auto KPointCloud::operator[](size_t elm) const -> ElementType const&
{
  return m_container[elm];
}

inline size_t KPointCloud::size() const
{
  return m_container.size();
}

inline bool KPointCloud::empty() const
{
  return m_container.empty();
}

inline void KPointCloud::clear()
{
  m_container.clear();
}

inline void KPointCloud::reserve(size_t count)
{
  m_container.reserve(count);
}

inline auto KPointCloud::cend() const -> ConstIterator
{
  return m_container.cend();
}

#endif // KPOINTCLOUD_H
