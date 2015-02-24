#ifndef KORIENTEDBOUNDINGVOLUME_H
#define KORIENTEDBOUNDINGVOLUME_H KOrientedBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KOrientedBoundingVolumePrivate;
class KOrientedBoundingVolume : public KAbstractBoundingVolume
{
public:
  enum Method
  {
    PcaMethod
  };

  KOrientedBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KOrientedBoundingVolume();
  void draw(KTransform3D &t, KColor const &color);
private:
  KOrientedBoundingVolumePrivate *m_private;
};

#endif // KORIENTEDBOUNDINGVOLUME_H
