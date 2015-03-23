#ifndef KADAPTIVEOCTREE_H
#define KADAPTIVEOCTREE_H KAdaptiveOctree

class KColor;
class KHalfEdgeMesh;
class KTransform3D;
#include <cstddef>

class KAdaptiveOctreePrivate;
class KAdaptiveOctree
{
public:
  KAdaptiveOctree();
  ~KAdaptiveOctree();

  typedef bool (*TerminationPred)(size_t numTriangles, size_t depth);

  void addGeometry(KHalfEdgeMesh const &mesh, KTransform3D &transform);
  bool isDirty() const;
  void build(TerminationPred pred);
  void clear();
  void draw(KTransform3D &trans, KColor const &color);
  void draw(KTransform3D &trans, KColor const &color, size_t min);
  void draw(KTransform3D &trans, KColor const &color, size_t min, size_t max);
  size_t depth();
private:
  KAdaptiveOctreePrivate *m_private;
};

#endif // KADAPTIVEOCTREE_H
