#ifndef KFRUSTUM_H
#define KFRUSTUM_H KFrustum

class KMatrix4x4;
#include <KPlane>
#include <KAabbBoundingVolume>

class KFrustum
{
public:
  KFrustum();
  KFrustum(KMatrix4x4 const &viewProj);
  void setFrustum(KMatrix4x4 const &viewProj);

  bool intersects(KAabbBoundingVolume const &aabb) const;

private:
  KPlane m_planes[6];
};

#endif // KFRUSTUM_H
