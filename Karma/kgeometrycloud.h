#ifndef KGEOMETRYCLOUD_H
#define KGEOMETRYCLOUD_H KGeometryCloud

class KHalfEdgeMesh;
class KTransform3D;
class KPointCloud;
class KTriangleIndexCloud;
#include <KSharedPointer>

class KGeometryCloudPrivate;
class KGeometryCloud
{
public:
  KGeometryCloud();
  ~KGeometryCloud();

  void addGeometry(KHalfEdgeMesh const &mesh);
  void addGeometry(KHalfEdgeMesh const &mesh, KTransform3D const &trans);

  void clear();
  bool dirty() const;

  KPointCloud const &pointCloud() const;
  KTriangleIndexCloud const &triangleIndexCloud() const;
  KPointCloud &pointCloud();
  KTriangleIndexCloud &triangleIndexCloud();
private:
  KSharedPointer<KGeometryCloudPrivate> m_private;
};

#endif // KGEOMETRYCLOUD_H
