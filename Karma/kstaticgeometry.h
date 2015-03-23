#ifndef KSTATICGEOMETRY_H
#define KSTATICGEOMETRY_H KStaticGeometry

class KColor;
class KHalfEdgeMesh;
class KTransform3D;
#include <cstddef>
#include <KGeometryCloud>
#include <KSharedPointer>

class KStaticGeometryPrivate;
class KStaticGeometry : public KGeometryCloud
{
public:
  KStaticGeometry();
  ~KStaticGeometry();

  enum BuildMethod
  {
    TopDownMethod,
    BottomUpMethod
  };

  typedef bool (*TerminationPred)(size_t numTriangles, size_t depth);

  void build(BuildMethod method, TerminationPred pred);
  void draw();
  void clear();
  size_t depth() const;
  void drawAabbs(KTransform3D &trans, KColor const &color);
  void drawAabbs(KTransform3D &trans, KColor const &color, size_t min);
  void drawAabbs(KTransform3D &trans, KColor const &color, size_t min, size_t max);

private:
  KSharedPointer<KStaticGeometryPrivate> m_private;
};

#endif // KSTATICGEOMETRY_H
