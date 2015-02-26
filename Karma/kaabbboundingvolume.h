#ifndef KAABBBOUNDINGVOLUME_H
#define KAABBBOUNDINGVOLUME_H KAabbBoundingVolume

#include <KAbstractBoundingVolume>
class KColor;
class KHalfEdgeMesh;
class KTransform3D;

class KAabbBoundingVolumePrivate;
class KAabbBoundingVolume : public KAbstractBoundingVolume
{
public:

  // Construction Methods
  enum Method
  {
    MinMaxMethod
  };

  // Constructors & Destructor
  KAabbBoundingVolume(KHalfEdgeMesh const &mesh, Method method);
  ~KAabbBoundingVolume();

  // Virtual Implementation
  void draw(KTransform3D &t, const KColor &color);

private:
  KAabbBoundingVolumePrivate *m_private;
};

#endif // KAABBBOUNDINGVOLUME_H
