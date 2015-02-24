#ifndef KELLIPSOIDBOUNDINGVOLUME_H
#define KELLIPSOIDBOUNDINGVOLUME_H KEllipsoidBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KEllipsoidBoundingVolume : public KAbstractBoundingVolume
{
public:
  enum Method
  {
    PcaMethod
  };

  KEllipsoidBoundingVolume(KHalfEdgeMesh const &hem, Method method);
  ~KEllipsoidBoundingVolume();
  void draw(KTransform3D &t);
};

#endif // KELLIPSOIDBOUNDINGVOLUME_H
