#ifndef KABSTRACTBOUNDINGVOLUME_H
#define KABSTRACTBOUNDINGVOLUME_H KAbstractBoundingVolume

class KColor;
class KTransform3D;

class KAbstractBoundingVolume
{
public:
  KAbstractBoundingVolume();
  ~KAbstractBoundingVolume();
  virtual void draw(KTransform3D &t, KColor const &color) = 0;
};

#endif // KABSTRACTBOUNDINGVOLUME_H
