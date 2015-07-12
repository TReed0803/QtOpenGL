#include "kadaptiveoctree.h"

#include <KMacros>
#include <KMatrix4x4>
#include <KHalfEdgeMesh>
#include <KTransform3D>
#include <KColor>
#include <KAabbBoundingVolume>
#include <KTrianglePartition>
#include <KTrianglePointIterator>
#include <OpenGLDebugDraw>

/*******************************************************************************
 * KAdaptiveOctreeNode
 ******************************************************************************/
class KAdaptiveOctreeNode
{
public:
  typedef KTriangleIndexCloud::ConstIterator ConstIterator;

  KAdaptiveOctreeNode(size_t depth, KAabbBoundingVolume const &aabb, KPointCloud &cloud);
  bool isLeaf() const;
  void debugDraw(KTransform3D &trans, size_t min, size_t max) const;

  size_t m_depth;
  KColor m_color;
  KPointCloud &m_pointCloud;
  KAabbBoundingVolume m_aabb;
  KAdaptiveOctreeNode *m_children[8];
  KTriangleIndexCloud m_objects;
};

KAdaptiveOctreeNode::KAdaptiveOctreeNode(size_t depth, KAabbBoundingVolume const &aabb, KPointCloud &cloud) :
  m_depth(depth), m_color(float(std::rand()) / RAND_MAX, float(std::rand()) / RAND_MAX, float(std::rand()) / RAND_MAX), m_pointCloud(cloud), m_aabb(aabb)
{
  for (int i = 0; i < 8; ++i)
  {
    m_children[i] = 0;
  }
}

bool KAdaptiveOctreeNode::isLeaf() const
{
  for (int i = 0; i < 8; ++i)
  {
    if(m_children[i] != 0)
      return false;
  }
  return true;
}

void KAdaptiveOctreeNode::debugDraw(KTransform3D &trans, size_t min, size_t max) const
{
  if (m_depth <= max)
  {
    if (m_depth >= min)
    {
      m_aabb.draw(trans, m_color);
    }
    for (int i = 0; i < 8; ++i)
    {
      if (m_children[i])
      {
        m_children[i]->debugDraw(trans, min, max);
      }
    }
  }
}

/*******************************************************************************
 * KAdaptiveOctreePrivate
 ******************************************************************************/
class KAdaptiveOctreePrivate
{
public:
  typedef KTriangleIndexCloud::Iterator TriangleIterator;
  typedef KAdaptiveOctree::TerminationPred TerminationPred;

  KAdaptiveOctreePrivate(KGeometryCloud &parent);
  void buildBottomUp(TerminationPred pred);
  void buildTopDown(TerminationPred pred);
  KAdaptiveOctreeNode* recursiveTopDown(size_t depth, KAabbBoundingVolume aabb, TriangleIterator begin, TriangleIterator end, TerminationPred pred);

  size_t m_maxDepth;
  KGeometryCloud m_parent;
  KPointCloud m_pointCloud;
  KAdaptiveOctreeNode *m_root;
};

KAdaptiveOctreePrivate::KAdaptiveOctreePrivate(KGeometryCloud &parent) :
  m_parent(parent), m_root(0)
{
  // Intentionally Empty
}

void KAdaptiveOctreePrivate::buildBottomUp(TerminationPred pred)
{
  (void)pred;
  qFatal("Unsupported Build Method!");
}

void KAdaptiveOctreePrivate::buildTopDown(TerminationPred pred)
{
  m_maxDepth = 0;
  KTriangleIndexCloud & triangleCloud = m_parent.triangleIndexCloud();
  KPointCloud & pointCloud = m_parent.pointCloud();
  KAabbBoundingVolume boundingVolume(KTrianglePointIterator(triangleCloud.begin(), pointCloud), KTrianglePointIterator(triangleCloud.end(), pointCloud));
  boundingVolume.makeCube();
  m_pointCloud = m_parent.pointCloud();
  m_root = recursiveTopDown(0, boundingVolume, triangleCloud.begin(), triangleCloud.end(), pred);
}

KAdaptiveOctreeNode* KAdaptiveOctreePrivate::recursiveTopDown(size_t depth, KAabbBoundingVolume aabb, TriangleIterator begin, TriangleIterator end, TerminationPred pred)
{
  KPointCloud const & pointCloud = m_parent.pointCloud();
  size_t numTriangles = std::distance(begin, end);
  if (m_maxDepth < depth) m_maxDepth = depth;

  // Check if the predicate was met (terminating condition)
  KAdaptiveOctreeNode *node = new KAdaptiveOctreeNode(depth, aabb, m_pointCloud);
  if (pred(numTriangles, m_maxDepth))
  {
    node->m_objects.copy(begin, end);
    return node;
  }

  // Get the aabb of all children
  aabb.scale(0.5f);
  float extent = aabb.extents().max.x() - aabb.center().x(); // Note: Any extent will do since it's a cube
  KAabbBoundingVolume aabbList[] =
  {
    aabb.copyOffset( extent,  extent,  extent),
    aabb.copyOffset(-extent,  extent,  extent),
    aabb.copyOffset( extent, -extent,  extent),
    aabb.copyOffset(-extent, -extent,  extent),
    aabb.copyOffset( extent, -extent, -extent),
    aabb.copyOffset(-extent, -extent, -extent),
    aabb.copyOffset( extent,  extent, -extent),
    aabb.copyOffset(-extent,  extent, -extent)
  };

  // Create all OctNodes
  TriangleIterator prevIter = begin;
  TriangleIterator currIter = end;
  for (int i = 0; i < 8; ++i)
  {
    currIter = std::partition(prevIter, end, KTrianglePartitionInsideAabb(pointCloud, aabbList[i]));
    node->m_children[i] = recursiveTopDown(depth + 1, aabbList[i], prevIter, currIter, pred);
    prevIter = currIter;
  }

  // Grab remaining indices
  node->m_objects.copy(begin, end);
  return node;
}

/*******************************************************************************
 * KAdaptiveOctree
 ******************************************************************************/
KAdaptiveOctree::KAdaptiveOctree() :
  m_private(new KAdaptiveOctreePrivate(*this))
{
  // Intentionally Empty
}

KAdaptiveOctree::~KAdaptiveOctree()
{
  // Intentionally Empty
}

void KAdaptiveOctree::clear()
{
  KGeometryCloud::clear();
  m_private = new KAdaptiveOctreePrivate(*this);
}

size_t KAdaptiveOctree::depth() const
{
  P(const KAdaptiveOctreePrivate);
  return p.m_maxDepth;
}

void KAdaptiveOctree::build(BuildMethod method, TerminationPred pred)
{
  P(KAdaptiveOctreePrivate);

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

void KAdaptiveOctree::debugDraw(size_t min, size_t max)
{
  KTransform3D trans;
  debugDraw(trans, min, max);
}

void KAdaptiveOctree::debugDraw(KTransform3D &trans, size_t min, size_t max)
{
  P(KAdaptiveOctreePrivate);
  if (p.m_root)
  {
    p.m_root->debugDraw(trans, min, max);
  }
}
