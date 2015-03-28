#include "kfrustum.h"

#include <KVector4D>
#include <KMatrix4x4>

KFrustum::KFrustum()
{
  // Intentionally Empty
}

KFrustum::KFrustum(const KMatrix4x4 &viewProj)
{
  setFrustum(viewProj);
}

void KFrustum::setFrustum(const KMatrix4x4 &viewProj)
{
  // Left clipping plane
  m_planes[0].set(
    viewProj(3,0) + viewProj(0,0),
    viewProj(3,1) + viewProj(0,1),
    viewProj(3,2) + viewProj(0,2),
    viewProj(3,3) + viewProj(0,3)
  );

  // Right clipping plane
  m_planes[1].set(
    viewProj(3,0) - viewProj(0,0),
    viewProj(3,1) - viewProj(0,1),
    viewProj(3,2) - viewProj(0,2),
    viewProj(3,3) - viewProj(0,3)
  );

  // Bottom clipping plane
  m_planes[2].set(
    viewProj(3,0) + viewProj(1,0),
    viewProj(3,1) + viewProj(1,1),
    viewProj(3,2) + viewProj(1,2),
    viewProj(3,3) + viewProj(1,3)
  );

  // Top clipping plane
  m_planes[3].set(
    viewProj(3,0) - viewProj(1,0),
    viewProj(3,1) - viewProj(1,1),
    viewProj(3,2) - viewProj(1,2),
    viewProj(3,3) - viewProj(1,3)
  );

  // Near clipping plane
  m_planes[4].set(
    viewProj(3,0) + viewProj(2,0),
    viewProj(3,1) + viewProj(2,1),
    viewProj(3,2) + viewProj(2,2),
    viewProj(3,3) + viewProj(2,3)
  );

  // Far clipping plane
  m_planes[5].set(
    viewProj(3,0) - viewProj(2,0),
    viewProj(3,1) - viewProj(2,1),
    viewProj(3,2) - viewProj(2,2),
    viewProj(3,3) - viewProj(2,3)
  );
}

bool KFrustum::intersects(const KAabbBoundingVolume &aabb) const
{
  for (int i = 0; i < 6; ++i)
  {
    if (m_planes[i].pointInBack(aabb.point(0)) &&
        m_planes[i].pointInBack(aabb.point(1)) &&
        m_planes[i].pointInBack(aabb.point(2)) &&
        m_planes[i].pointInBack(aabb.point(3)) &&
        m_planes[i].pointInBack(aabb.point(4)) &&
        m_planes[i].pointInBack(aabb.point(5)) &&
        m_planes[i].pointInBack(aabb.point(6)) &&
        m_planes[i].pointInBack(aabb.point(7))
        )
    {
      return false;
    }
  }
  return true;
}
