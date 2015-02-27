#ifndef KSTATICGEOMETRY_H
#define KSTATICGEOMETRY_H KStaticGeometry

class KColor;
class KHalfEdgeMesh;
class KTransform3D;

class KStaticGeometryPrivate;
class KStaticGeometry
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

  void addGeometry(KHalfEdgeMesh const &mesh, KTransform3D &transform);
  bool isDirty() const;
  void build(BuildMethod method, TerminationPred pred = 0);
  void clear();
  void draw();
  size_t depth() const;
  void drawAabbs(KTransform3D &trans, KColor const &color);
  void drawAabbs(KTransform3D &trans, KColor const &color, size_t min);
  void drawAabbs(KTransform3D &trans, KColor const &color, size_t min, size_t max);

private:
  KStaticGeometryPrivate *m_private;
};

#endif // KSTATICGEOMETRY_H
