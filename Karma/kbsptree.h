#ifndef KBSPTREE_H
#define KBSPTREE_H KBspTree

class KColor;
class KHalfEdgeMesh;
class KTransform3D;
#include <KGeometryCloud>
#include <KSharedPointer>

class KBspTreePrivate;
class KBspTree : public KGeometryCloud
{
public:
  KBspTree();
  ~KBspTree();

  void clear();
  size_t depth() const;
  void build(BuildMethod method, TerminationPred pred);
  void debugDraw(size_t min = 0, size_t max = std::numeric_limits<size_t>::max());
  void debugDraw(KTransform3D &trans, size_t min = 0, size_t max = std::numeric_limits<size_t>::max());

private:
  KSharedPointer<KBspTreePrivate> m_private;
};

#endif // KBSPTREE_H
