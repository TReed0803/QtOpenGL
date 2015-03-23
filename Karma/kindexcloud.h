#ifndef KINDEXCLOUD_H
#define KINDEXCLOUD_H KIndexCloud

#include <cstddef>
#include <vector>

class KIndexCloud
{
public:
  typedef size_t ElementType;
  typedef std::vector<ElementType> ContainerType;

  void reserve(size_t count);
  void emplace_back(ElementType elm);
private:
  ContainerType m_container;
};

inline void KIndexCloud::reserve(size_t count)
{
  m_container.reserve(count);
}

inline void KIndexCloud::emplace_back(ElementType elm)
{
  m_container.emplace_back(elm);
}

#endif // KINDEXCLOUD_H

