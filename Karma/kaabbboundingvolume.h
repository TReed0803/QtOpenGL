#ifndef KAABBBOUNDINGVOLUME_H
#define KAABBBOUNDINGVOLUME_H KAabbBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KAabbBoundingVolumePrivate;
class KAabbBoundingVolume : public KAbstractBoundingVolume
{
public:

  enum Method
  {
    MinMaxMethod
  };

  KAabbBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KAabbBoundingVolume();
  void draw(KTransform3D &t, const KColor &color);
private:
  KAabbBoundingVolumePrivate *m_private;
};

#endif // KAABBBOUNDINGVOLUME_H
