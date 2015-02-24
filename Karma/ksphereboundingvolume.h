#ifndef KSPHEREBOUNDINGVOLUME_H
#define KSPHEREBOUNDINGVOLUME_H KSphereBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KSphereBoundingVolume : public KAbstractBoundingVolume
{
public:
  enum Method
  {
    CentroidMethod,
    RittersMethod,
    LarssonsMethod,
    PcaMethod
  };

  KSphereBoundingVolume(KHalfEdgeMesh const &hem, Method method);
  ~KSphereBoundingVolume();
  void draw(KTransform3D &t);
};

#endif // KSPHEREBOUNDINGVOLUME_H
