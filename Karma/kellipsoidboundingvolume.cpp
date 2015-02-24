#include "kellipsoidboundingvolume.h"
#include <KMacros>

class KEllipsoidBoundingVolumePrivate
{
public:
  void calculatePcaMethod(KHalfEdgeMesh const &mesh);
};

void KEllipsoidBoundingVolumePrivate::calculatePcaMethod(const KHalfEdgeMesh &mesh)
{

}

KEllipsoidBoundingVolume::KEllipsoidBoundingVolume(KHalfEdgeMesh const &mesh, Method method) :
  m_private(new KEllipsoidBoundingVolumePrivate)
{
  P(KEllipsoidBoundingVolumePrivate);
  switch (method)
  {
  case PcaMethod:
    p.calculatePcaMethod(mesh);
    break;
  }
}

KEllipsoidBoundingVolume::~KEllipsoidBoundingVolume()
{
  delete m_private;
}

void KEllipsoidBoundingVolume::draw(KTransform3D &t, const KColor &color)
{

}
