#include "kstaticgeometry.h"

#include <vector>
#include <KMath>
#include <KMacros>
#include <KVector3D>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <KAabbBoundingVolume>
#include <KPointCloud>
#include <KTriangleIndexCloud>
#include <KIndexCloud>
#include <KTrianglePointIterator>
#include <KTrianglePartition>

// Todo: Move to external file
typedef std::vector<KAabbBoundingVolume*> KAabbCloud;

/*******************************************************************************
 * KStaticGeometryInstance
 ******************************************************************************/
struct KStaticGeometryInstance
{
  KStaticGeometryInstance(KTriangleIndexIterator begin, KTriangleIndexIterator end);
  KIndexCloud m_indexCloud;
};

KStaticGeometryInstance::KStaticGeometryInstance(KTriangleIndexIterator begin, KTriangleIndexIterator end)
{
  size_t numIndices = std::distance(begin, end);
  m_indexCloud.reserve(numIndices);
  while (begin != end)
  {
    m_indexCloud.emplace_back((*begin) - 1);
    ++begin;
  }
}

/*******************************************************************************
 * KStaticGeometryNode
 ******************************************************************************/
class KStaticGeometryNode
{
public:
  typedef KTriangleIndexCloud::ConstIterator ConstIterator;

  KStaticGeometryNode(size_t depth, ConstIterator begin, ConstIterator end, KPointCloud const &pointCloud);
  KStaticGeometryNode(size_t depth, KStaticGeometryNode *left, KStaticGeometryNode *right);
  bool isLeaf() const;
  void drawAabb(KTransform3D &trans, KColor const &color, size_t min, size_t max) const;
  void correctDepth(size_t depth);
  size_t getMaxDepth();

  KAabbBoundingVolume aabb;
  KStaticGeometryNode *left;
  KStaticGeometryNode *right;

  // For drawing
  size_t from, to;
  size_t depth;
  KStaticGeometryInstance *instance;
};

KStaticGeometryNode::KStaticGeometryNode(size_t d, ConstIterator begin, ConstIterator end, KPointCloud const &pointCloud) :
  aabb(KTrianglePointIterator(begin, pointCloud), KTrianglePointIterator(end, pointCloud)),
  left(0), right(0), instance(0), depth(d)
{
  // Intentionally Empty
}

KStaticGeometryNode::KStaticGeometryNode(size_t d, KStaticGeometryNode *left, KStaticGeometryNode *right) :
  aabb(left->aabb, right->aabb),
  left(left), right(right), instance(0), depth(d)
{
  left->depth = depth + 1;
  right->depth = depth + 1;
}

bool KStaticGeometryNode::isLeaf() const
{
  return (left == 0);
}

void KStaticGeometryNode::drawAabb(KTransform3D &trans, const KColor &color, size_t min, size_t max) const
{
  if (depth <= max)
  {
    if (depth >= min)
    {
      aabb.draw(trans, Karma::colorShift(color, 0.1f * depth));
    }
    if (!isLeaf())
    {
      if (left)  left->drawAabb(trans, color, min, max);
      if (right) right->drawAabb(trans, color, min, max);
    }
  }
}

void KStaticGeometryNode::correctDepth(size_t d)
{
  depth = d;
  if (left) left->correctDepth(d + 1);
  if (right) right->correctDepth(d + 1);
}

size_t KStaticGeometryNode::getMaxDepth()
{
  return std::max(depth, std::max(left ? left->getMaxDepth() : 0, right ? right->getMaxDepth() : 0));
}

/*******************************************************************************
 * KStaticGeometryPrivate
 ******************************************************************************/
class KStaticGeometryPrivate
{
public:
  typedef KTriangleIndexCloud::Iterator TriangleIterator;
  typedef KStaticGeometry::TerminationPred TerminationPred;
  KStaticGeometryPrivate(KGeometryCloud &parent);
  void buildBottomUp(TerminationPred pred);
  void buildTopDown(TerminationPred pred);

  KAabbCloud m_aabbCloud;
  KStaticGeometryNode *m_root;
  size_t m_maxDepth;
  KGeometryCloud &m_parent;

private:
  KStaticGeometryNode *recursiveTopDown(size_t depth, TriangleIterator begin, TriangleIterator end, TerminationPred pred);
};

// Helper functor
struct KAabbMinCentroidDistance : public std::binary_function<bool, KStaticGeometryNode*, KStaticGeometryNode*>
{
  KAabbMinCentroidDistance(KStaticGeometryNode const *geom) :
    m_geom(geom)
  {
    // Intentionally Empty
  }

  bool operator()(const KStaticGeometryNode *lhs, const KStaticGeometryNode *rhs) const
  {
    return (lhs->aabb.center() - m_geom->aabb.center()).lengthSquared() < (rhs->aabb.center() - m_geom->aabb.center()).lengthSquared();
  }

  KStaticGeometryNode const *m_geom;
};

KStaticGeometryPrivate::KStaticGeometryPrivate(KGeometryCloud &parent) :
  m_parent(parent)
{
  // Intentionally Empty
}

void KStaticGeometryPrivate::buildBottomUp(TerminationPred pred)
{
  KPointCloud & pointCloud = m_parent.pointCloud();
  KTriangleIndexCloud & triangleCloud = m_parent.triangleIndexCloud();
  typedef std::vector<KStaticGeometryNode*> KStaticGeometryNodeCloud;
  KStaticGeometryNodeCloud nodes;
  size_t numTriangles = triangleCloud.size();
  nodes.reserve(numTriangles);

  int leafCount;
  int depthEstimate;
  int leafSize = static_cast<int>(numTriangles);
  do
  {
    leafCount = static_cast<int>(numTriangles / leafSize);
    depthEstimate = std::ceil(std::log(leafSize) / Karma::Log2);
    leafSize /= 2;
    if (leafSize == 1) break;
  }
  while (pred(numTriangles / leafSize, depthEstimate));

  // Initial Construction
  size_t remaining;
  KStaticGeometryNode *currNode;
  TriangleIterator it = triangleCloud.begin();
  while (it != triangleCloud.cend())
  {
    remaining = std::distance(it, triangleCloud.end());
    if (remaining > leafCount)
      remaining = leafCount;
    currNode = new KStaticGeometryNode(0, it, it + remaining, pointCloud);
    nodes.push_back(currNode);
    std::advance(it, remaining);
  }

  std::vector<KStaticGeometryNode*> working;
  while (nodes.size() != 1)
  {
    working.reserve(nodes.size() / 2 + 1);
    KStaticGeometryNodeCloud::iterator it = nodes.begin();
    while (it != nodes.end())
    {
      KAabbMinCentroidDistance distanceCheck(*it);
      KStaticGeometryNodeCloud::iterator min = std::min_element(it + 1, nodes.end(), distanceCheck);
      if (min != nodes.end())
      {
        std::iter_swap(it + 1, min);
        working.push_back(new KStaticGeometryNode(0, *it, *(it + 1)));
        std::advance(it, 2);
      }
      else
      {
        working.push_back(*it);
        std::advance(it, 1);
      }
    }

    nodes.swap(working);
    working.clear();
  }

  m_root = nodes[0];
  m_root->correctDepth(0);
  m_maxDepth = m_root->getMaxDepth();
}

KStaticGeometryNode *KStaticGeometryPrivate::recursiveTopDown(size_t depth, TriangleIterator begin, TriangleIterator end, TerminationPred pred)
{
  KPointCloud const & pointCloud = m_parent.pointCloud();
  size_t numPoints = std::distance(begin, end);

  if (numPoints == 0) return 0;
  if (m_maxDepth < depth) m_maxDepth = depth;

  KStaticGeometryNode *node = new KStaticGeometryNode(depth, begin, end, pointCloud);
  if (!pred(numPoints, depth))
  {
    KVector3D const &maxAxis = node->aabb.maxAxis();
    TriangleIterator secondHalf = std::partition(begin, end, KTrianglePartitionAlongAxis(pointCloud, node->aabb.center(), maxAxis));
    if (secondHalf == begin || secondHalf == end)
    {
      node->instance = new KStaticGeometryInstance(begin, end);
    }
    else
    {
      node->left  = recursiveTopDown(depth + 1,      begin, secondHalf, pred);
      node->right = recursiveTopDown(depth + 1, secondHalf,        end, pred);
    }
  }
  else
  {
    node->instance = new KStaticGeometryInstance(begin, end);
  }

  return node;
}

void KStaticGeometryPrivate::buildTopDown(TerminationPred pred)
{
  // Top-Down looks at the entire triangle cloud.
  m_maxDepth = 0;
  KTriangleIndexCloud & triangleCloud = m_parent.triangleIndexCloud();
  m_root = recursiveTopDown(0, triangleCloud.begin(), triangleCloud.end(), pred);
}

/*******************************************************************************
 * KStaticGeometry
 ******************************************************************************/
KStaticGeometry::KStaticGeometry() :
  m_private(new KStaticGeometryPrivate(*this))
{
  // Intentionall Empty
}

KStaticGeometry::~KStaticGeometry()
{
  delete m_private;
}

void KStaticGeometry::build(BuildMethod method, TerminationPred pred)
{
  P(KStaticGeometryPrivate);

  // If there is no new geometry to build, do nothing.
  if (!dirty()) return;

  // Build based on selected method
  switch (method)
  {
  case BottomUpMethod:
    p.buildBottomUp(pred);
    break;
  case TopDownMethod:
    p.buildTopDown(pred);
    break;
  }

  // We no longer need this data
  clear();

  // Next: Depth-first search to find all leaf nodes.
  //       Form an index buffer of all contiguous leaf nodes.
  //       As we iterate through, update the parent nodes so they
  //       understand the "drawable range" of the children.

}

size_t KStaticGeometry::depth() const
{
  P(KStaticGeometryPrivate);
  return p.m_maxDepth;
}

void KStaticGeometry::drawAabbs(KTransform3D &trans, const KColor &color)
{
  drawAabbs(trans, color, 0);
}

void KStaticGeometry::drawAabbs(KTransform3D &trans, const KColor &color, size_t min)
{
  P(KStaticGeometryPrivate);
  p.m_root->drawAabb(trans, color, min, std::numeric_limits<size_t>::max());
}

void KStaticGeometry::drawAabbs(KTransform3D &trans, const KColor &color, size_t min, size_t max)
{
  P(KStaticGeometryPrivate);
  p.m_root->drawAabb(trans, color, min, max);
}
