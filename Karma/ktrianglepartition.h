#ifndef KTRIANGLEPARTITION_H
#define KTRIANGLEPARTITION_H KTrianglePartition

#include <KVector3D>
#include <KPointCloud>
#include <KTriangleIndexCloud>
#include <KAabbBoundingVolume>
#include <KPlane>

/*******************************************************************************
 * KTrianglePartition
 ******************************************************************************/
class KTrianglePartition : public std::unary_function<bool, KTriangleIndexCloud::ElementType>
{
public:
  typedef KTriangleIndexCloud::ElementType ElementType;
  KTrianglePartition(KPointCloud const &cloud);
protected:
  KPointCloud const &m_pointCloud;
  KVector3D const &point(int idx) const;
};

inline KTrianglePartition::KTrianglePartition(const KPointCloud &cloud) :
  m_pointCloud(cloud)
{
  // Intentionally Empty
}

inline const KVector3D &KTrianglePartition::point(int idx) const
{
  return m_pointCloud[idx - 1];
}

/*******************************************************************************
 * KTrianglePartitionAlongAxis
 ******************************************************************************/
class KTrianglePartitionAlongAxis : KTrianglePartition
{
public:

  KTrianglePartitionAlongAxis(KPointCloud const &cloud, KVector3D const &center, KVector3D const &axis) :
    KTrianglePartition(cloud), m_axis(axis), m_midDot(KVector3D::dotProduct(center, axis))
  {
    // Intentionally Empty
  }

  bool operator()(ElementType const &tri) const
  {
    return (KVector3D::dotProduct(point(tri.indices[0]), m_axis) < m_midDot)
        && (KVector3D::dotProduct(point(tri.indices[1]), m_axis) < m_midDot)
        && (KVector3D::dotProduct(point(tri.indices[2]), m_axis) < m_midDot);
  }

protected:
  float m_midDot;
  KVector3D m_axis;
};

/*******************************************************************************
 * KTrianglePartitionInsideAabb
 ******************************************************************************/
class KTrianglePartitionInsideAabb : KTrianglePartition
{
public:

  KTrianglePartitionInsideAabb(KPointCloud const &cloud, KAabbBoundingVolume const &aabb) :
    KTrianglePartition(cloud), m_aabb(aabb)
  {
    // Intentionally Empty
  }

  bool operator()(ElementType const &tri) const
  {
    return m_aabb.contains(point(tri.indices[0]))
        && m_aabb.contains(point(tri.indices[1]))
        && m_aabb.contains(point(tri.indices[2]));
  }

protected:
  KAabbBoundingVolume m_aabb;
};

/*******************************************************************************
 * KTrianglePartitionPlane
 ******************************************************************************/
class KTrianglePartitionPlane : KTrianglePartition
{
public:

  KTrianglePartitionPlane(KPointCloud const &cloud, KPlane const &plane) :
    KTrianglePartition(cloud), m_plane(plane)
  {
    // Intentionally Empty
  }

  bool operator()(ElementType const &tri) const
  {
    return m_plane.pointInFront(point(tri.indices[0]))
        && m_plane.pointInFront(point(tri.indices[1]))
        && m_plane.pointInFront(point(tri.indices[2]));
  }

protected:
  KPlane m_plane;
};

#endif // KTRIANGLEPARTITION_H

