#ifndef KORIENTEDBOUNDINGVOLUME_H
#define KORIENTEDBOUNDINGVOLUME_H KOrientedBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KOrientedBoundingVolume : public KAbstractBoundingVolume
{
public:
  enum Method
  {
    PcaMethod
  };

  KOrientedBoundingVolume(KHalfEdgeMesh const &hem, Method method);
  ~KOrientedBoundingVolume();
  void draw(KTransform3D &t);
};

#endif // KORIENTEDBOUNDINGVOLUME_H
