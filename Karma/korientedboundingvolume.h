#ifndef KORIENTEDBOUNDINGVOLUME_H
#define KORIENTEDBOUNDINGVOLUME_H KOrientedBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KOrientedBoundingVolumePrivate;
class KOrientedBoundingVolume : public KAbstractBoundingVolume
{
public:

  // Construction Methods
  enum Method
  {
    PcaMethod
  };

  // Constructors / Destructor
  KOrientedBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KOrientedBoundingVolume();

  // Virtual Implementaiton
  void draw(KTransform3D &t, KColor const &color);

private:
  KOrientedBoundingVolumePrivate *m_private;
};

#endif // KORIENTEDBOUNDINGVOLUME_H
