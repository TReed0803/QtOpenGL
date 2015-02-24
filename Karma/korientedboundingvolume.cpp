#include "korientedboundingvolume.h"
#include <KMacros>

class KOrientedBoundingVolumePrivate
{
public:
  void calculatePcaMethod(KHalfEdgeMesh const &mesh);
};

void KOrientedBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{

}

KOrientedBoundingVolume::KOrientedBoundingVolume(const KHalfEdgeMesh &mesh, Method method) :
  m_private(new KOrientedBoundingVolumePrivate)
{
  P(KOrientedBoundingVolumePrivate);
  switch (method)
  {
  case PcaMethod:
    p.calculatePcaMethod(mesh);
    break;
  }
}

KOrientedBoundingVolume::~KOrientedBoundingVolume()
{
  delete m_private;
}

void KOrientedBoundingVolume::draw(KTransform3D &t, const KColor &color)
{

}



