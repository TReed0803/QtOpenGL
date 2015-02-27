#ifndef KSPHEREBOUNDINGVOLUME_H
#define KSPHEREBOUNDINGVOLUME_H KSphereBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KSphereBoundingVolumePrivate;
class KSphereBoundingVolume : public KAbstractBoundingVolume
{
public:

  // Construction Methods
  enum Method
  {
    CentroidMethod,
    RittersMethod,
    LarssonsMethod,
    PcaMethod
  };

  // Constuctors / Destructor
  KSphereBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KSphereBoundingVolume();

  // Virtual Implementation
  void draw(KTransform3D &t, KColor const &color) const;

private:
  KSphereBoundingVolumePrivate *m_private;
};

#endif // KSPHEREBOUNDINGVOLUME_H
