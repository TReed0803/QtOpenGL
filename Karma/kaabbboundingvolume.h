#ifndef KAABBBOUNDINGVOLUME_H
#define KAABBBOUNDINGVOLUME_H KAabbBoundingVolume

#include <KAbstractBoundingVolume>
class KHalfEdgeMesh;

class KAabbBoundingVolume : public KAbstractBoundingVolume
{
public:

  enum Method
  {
    MinMaxMethod
  };

  KAabbBoundingVolume(KHalfEdgeMesh const &hem, Method method);
  ~KAabbBoundingVolume();
  void draw(KTransform3D &t);
};

#endif // KAABBBOUNDINGVOLUME_H
