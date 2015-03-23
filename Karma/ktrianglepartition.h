#ifndef KTRIANGLEPARTITION_H
#define KTRIANGLEPARTITION_H KTrianglePartition

#include <KVector3D>
#include <KPointCloud>
#include <KTriangleIndexCloud>

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
};

inline KTrianglePartition::KTrianglePartition(const KPointCloud &cloud) :
  m_pointCloud(cloud)
{
  // Intentionally Empty
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
    return (KVector3D::dotProduct(m_pointCloud[tri.indices[0] - 1], m_axis) < m_midDot)
        && (KVector3D::dotProduct(m_pointCloud[tri.indices[1] - 1], m_axis) < m_midDot)
        && (KVector3D::dotProduct(m_pointCloud[tri.indices[2] - 1], m_axis) < m_midDot);
  }

protected:
  float m_midDot;
  KVector3D m_axis;
};

#endif // KTRIANGLEPARTITION_H

