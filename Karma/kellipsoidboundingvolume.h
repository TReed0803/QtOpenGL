#ifndef KELLIPSOIDBOUNDINGVOLUME_H
#define KELLIPSOIDBOUNDINGVOLUME_H KEllipsoidBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KEllipsoidBoundingVolumePrivate;
class KEllipsoidBoundingVolume : public KAbstractBoundingVolume
{
public:
  enum Method
  {
    PcaMethod
  };

  KEllipsoidBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KEllipsoidBoundingVolume();
  void draw(KTransform3D &t, KColor const &color) const;
private:
  KEllipsoidBoundingVolumePrivate *m_private;
};

#endif // KELLIPSOIDBOUNDINGVOLUME_H
