#ifndef KSPHEREBOUNDINGVOLUME_H
#define KSPHEREBOUNDINGVOLUME_H KSphereBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KSphereBoundingVolumePrivate;
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

  KSphereBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KSphereBoundingVolume();
  void draw(KTransform3D &t, KColor const &color);
private:
  KSphereBoundingVolumePrivate *m_private;
};

#endif // KSPHEREBOUNDINGVOLUME_H
