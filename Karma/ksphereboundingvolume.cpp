#include "ksphereboundingvolume.h"
#include <KMacros>

class KSphereBoundingVolumePrivate
{
public:
  void calculateCentroidMethod(const KHalfEdgeMesh &mesh);
  void calculateRittersMethod(const KHalfEdgeMesh &mesh);
  void calculateLarssonsMethod(const KHalfEdgeMesh &mesh);
  void calculatePcaMethod(const KHalfEdgeMesh &mesh);
};

void KSphereBoundingVolumePrivate::calculateCentroidMethod(const KHalfEdgeMesh &mesh)
{

}

void KSphereBoundingVolumePrivate::calculateRittersMethod(const KHalfEdgeMesh &mesh)
{

}

void KSphereBoundingVolumePrivate::calculateLarssonsMethod(const KHalfEdgeMesh &mesh)
{

}

void KSphereBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{

}

KSphereBoundingVolume::KSphereBoundingVolume(const KHalfEdgeMesh &mesh, Method method) :
  m_private(new KSphereBoundingVolumePrivate)
{
  P(KSphereBoundingVolumePrivate);
  switch (method)
  {
  case CentroidMethod:
    p.calculateCentroidMethod(mesh);
    break;
  case RittersMethod:
    p.calculateRittersMethod(mesh);
    break;
  case LarssonsMethod:
    p.calculateLarssonsMethod(mesh);
    break;
  case PcaMethod:
    p.calculatePcaMethod(mesh);
    break;
  }
}

KSphereBoundingVolume::~KSphereBoundingVolume()
{
  delete m_private;
}

void KSphereBoundingVolume::draw(KTransform3D &t, const KColor &color)
{

}
