#include "kbsptree.h"

#include <KMacros>
#include <KMatrix4x4>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <KColor>
#include <KAabbBoundingVolume>
#include <KTrianglePartition>
#include <KTrianglePointIterator>
#include <OpenGLDebugDraw>
#include <KPlane>

/*******************************************************************************
 * KAdaptiveOctreeNode
 ******************************************************************************/
class KBspTreeNode
{
public:
  typedef KTriangleIndexCloud::ConstIterator ConstIterator;

  KBspTreeNode(size_t depth, KPointCloud &cloud);
  bool isLeaf() const;
  void debugDraw(KTransform3D &trans, size_t min, size_t max) const;

  size_t m_depth;
  KColor m_color;
  KPlane m_plane;
  KBspTreeNode *m_left;
  KBspTreeNode *m_right;
  KTriangleIndexCloud m_objects;
  KPointCloud &m_pointCloud;
};

KBspTreeNode::KBspTreeNode(size_t depth, KPointCloud &cloud) :
  m_depth(depth), m_color(float(std::rand()) / RAND_MAX, float(std::rand()) / RAND_MAX, float(std::rand()) / RAND_MAX), m_left(0), m_right(0), m_pointCloud(cloud)
{
  // Intentionally Empty
}

bool KBspTreeNode::isLeaf() const
{
  return (m_left == 0 && m_right == 0);
}

void KBspTreeNode::debugDraw(KTransform3D &trans, size_t min, size_t max) const
{
  if (m_depth <= max)
  {
    if (m_depth >= min)
    {
      for (KTriangleIndexCloud::ElementType const &elm : m_objects)
      {
        OpenGLDebugDraw::World::drawTriangle(
          m_pointCloud[elm.indices[0] - 1],
          m_pointCloud[elm.indices[1] - 1],
          m_pointCloud[elm.indices[2] - 1],
          m_color
        );
      }
    }
    if (m_left)
    {
      m_left->debugDraw(trans, min, max);
    }
    if (m_right)
    {
      m_right->debugDraw(trans, min, max);
    }
  }
}

/*******************************************************************************
 * KBspTreePrivate
 ******************************************************************************/
class KBspTreePrivate
{
public:
  typedef KTriangleIndexCloud::Iterator TriangleIterator;
  typedef KBspTree::TerminationPred TerminationPred;

  KBspTreePrivate(KGeometryCloud &parent);
  void buildBottomUp(TerminationPred pred);
  void buildTopDown(TerminationPred pred);
  KBspTreeNode* recursiveTopDown(size_t depth, TriangleIterator begin, TriangleIterator end, TerminationPred pred);
  KPlane pickSplittingPlane(TriangleIterator begin, TriangleIterator end, float skipWeight = 0.0f);

  KBspTreeNode *m_root;
  size_t m_maxDepth;
  KGeometryCloud m_parent;
  KPointCloud m_pointCloud;
};

KBspTreePrivate::KBspTreePrivate(KGeometryCloud &parent) :
  m_root(0), m_parent(parent)
{
  // Intentionally Empty
}

void KBspTreePrivate::buildBottomUp(TerminationPred pred)
{
  (void)pred;
  qFatal("Unsupported Build Method!");
}

void KBspTreePrivate::buildTopDown(TerminationPred pred)
{
  m_maxDepth = 0;
  KTriangleIndexCloud & triangleCloud = m_parent.triangleIndexCloud();
  m_pointCloud = m_parent.pointCloud();
  m_root = recursiveTopDown(0, triangleCloud.begin(), triangleCloud.end(), pred);
}

KBspTreeNode* KBspTreePrivate::recursiveTopDown(size_t depth, TriangleIterator begin, TriangleIterator end, TerminationPred pred)
{
  KPointCloud const & pointCloud = m_parent.pointCloud();
  size_t numTriangles = std::distance(begin, end);
  if (m_maxDepth < depth) m_maxDepth = depth;

  // Check if the predicate was met (terminating condition)
  KBspTreeNode *node = new KBspTreeNode(depth, m_pointCloud);
  if (pred(numTriangles, m_maxDepth))
  {
    node->m_objects.copy(begin, end);
    return node;
  }

  // Calculate Plane
  float skip = 0.0f;
  size_t testTriangles = numTriangles;
  while (testTriangles > 1000)
  {
    skip += 0.1f;
    testTriangles /= 10;
  }
  KPlane plane = pickSplittingPlane(begin, end, skip);
  node->m_plane = plane;

  // Create all nodes
  TriangleIterator middle = std::partition(begin, end, KTrianglePartitionPlane(pointCloud, plane));
  node->m_left = recursiveTopDown(depth + 1, begin, middle, pred);
  node->m_right = recursiveTopDown(depth + 1, middle, end, pred);

  // Grab remaining indices
  node->m_objects.copy(begin, end);
  return node;
}

KPlane KBspTreePrivate::pickSplittingPlane(TriangleIterator begin, TriangleIterator end, float skipWeight)
{
  const float K = 0.8f;

  // Initialize search statistics
  float bestScore = std::numeric_limits<float>::max();
  size_t numPolygons = std::distance(begin, end);
  size_t skipSize = numPolygons * skipWeight;
  skipSize = Karma::clamp(skipSize, static_cast<size_t>(1), numPolygons);

  // For analysis
  float score;
  KPlane plane, bestPlane;
  int numCoplanar, numInFront, numInBack, numStraddling;
  int bestCoplanar, bestInFront, bestInBack, bestStraddling;
  bestCoplanar = bestInFront = bestInBack = bestStraddling = 0;

  // Iterate over the range, skipping based on the weight provided.
  size_t numRemaining;
  TriangleIterator origBegin = begin;
  while (begin != end)
  {
    // Test if we have no more remaining
    numRemaining = std::distance(begin, end);
    if (numRemaining < skipSize) break;

    // Construct the sample plane
    KTriangleIndexCloud::ElementType const &sampleTriangle = *begin;
    plane = KPlane(
      m_pointCloud[sampleTriangle.indices[0] - 1],
      m_pointCloud[sampleTriangle.indices[1] - 1],
      m_pointCloud[sampleTriangle.indices[2] - 1]
    );

    // Count the polygons
    numCoplanar = numInFront = numInBack = numStraddling = 0;
    Karma::classifyRange(plane, origBegin, end, m_pointCloud, &numCoplanar, &numInFront, &numInBack, &numStraddling);

    // Score the polygons
    score = K * (numStraddling + numCoplanar) + (1.0f - K) * std::abs(numInFront - numInBack);
    if (score < bestScore)
    {
      bestCoplanar = numCoplanar;
      bestInFront = numInFront;
      bestInBack = numInBack;
      bestStraddling = numStraddling;
      bestScore = score;
      bestPlane = plane;
    }

    // Note: Skipping through a percentage of the mesh was introduced for efficiency reasons
    begin += skipSize;
  }

  // Edge case: No plane formed by the faces of the mesh will reduce sample size
  while (bestInFront == 0 || bestInBack == 0)
  {
    KTriangleIndexCloud::ElementType const &a = *(origBegin + (std::rand() % numPolygons));
    KTriangleIndexCloud::ElementType const &b = *(origBegin + (std::rand() % numPolygons));
    KTriangleIndexCloud::ElementType const &c = *(origBegin + (std::rand() % numPolygons));
    bestPlane = KPlane(
      m_pointCloud[a.indices[0] - 1],
      m_pointCloud[b.indices[1] - 1],
      m_pointCloud[c.indices[2] - 1]
    );
    Karma::classifyRange(bestPlane, origBegin, end, m_pointCloud, &bestCoplanar, &bestInFront, &bestInBack, &bestStraddling);
  }

  // Due to the edge case, the splitting of the mesh isn't always the same.
  return bestPlane;
}

/*******************************************************************************
 * KBspTree
 ******************************************************************************/
KBspTree::KBspTree() :
  m_private(new KBspTreePrivate(*this))
{
  // Intentionally Empty
}

KBspTree::~KBspTree()
{
  // Intentionally Empty
}

void KBspTree::clear()
{
  KGeometryCloud::clear();
  m_private = new KBspTreePrivate(*this);
}

size_t KBspTree::depth() const
{
  P(const KBspTreePrivate);
  return p.m_maxDepth;
}

void KBspTree::build(KGeometryCloud::BuildMethod method, KGeometryCloud::TerminationPred pred)
{
  P(KBspTreePrivate);

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
  KGeometryCloud::clear();

  // Next: Depth-first search to find all leaf nodes.
  //       Form an index buffer of all contiguous leaf nodes.
  //       As we iterate through, update the parent nodes so they
  //       understand the "drawable range" of the children.
}

void KBspTree::debugDraw(size_t min, size_t max)
{
  KTransform3D trans;
  debugDraw(trans, min, max);
}

void KBspTree::debugDraw(KTransform3D &trans, size_t min, size_t max)
{
  P(KBspTreePrivate);
  if (p.m_root)
  {
    p.m_root->debugDraw(trans, min, max);
  }
}
