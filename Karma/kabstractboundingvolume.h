#ifndef KABSTRACTBOUNDINGVOLUME_H
#define KABSTRACTBOUNDINGVOLUME_H KAbstractBoundingVolume

class KTransform3D;

class KAbstractBoundingVolume
{
public:
  KAbstractBoundingVolume();
  ~KAbstractBoundingVolume();
  virtual void draw(KTransform3D &t) = 0;
};

#endif // KABSTRACTBOUNDINGVOLUME_H
