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

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/
struct KTriangleElement
{
  typedef size_t index_type;
  KTriangleElement(index_type e0, index_type e1, index_type e2);
  index_type indices[3];
};

KTriangleElement::KTriangleElement(KTriangleElement::index_type e0, KTriangleElement::index_type e1, KTriangleElement::index_type e2)
{
  indices[0] = e0;
  indices[1] = e1;
  indices[2] = e2;
}

struct KPointElement
{
  typedef KVector3D point_type;
  KPointElement(point_type const &p);
  point_type point;
};

KPointElement::KPointElement(const KPointElement::point_type &p) :
  point(p)
{
  // Intentionally Empty
}

typedef std::vector<KPointElement> KPointCloud;
typedef std::vector<KTriangleElement> KTriangleCloud;
typedef std::vector<KTriangleElement::index_type> KTriangleIndexCloud;
typedef std::vector<KAabbBoundingVolume*> KAabbCloud;

struct KPointCloudAccessor : public std::unary_function<KVector3D, KPointElement>
{
  KVector3D const &operator()(KPointElement const &elm) const
  {
    return elm.point;
  }
};

struct KStaticGeometryInstance
{
  typedef KTriangleCloud::const_iterator const_iterator;
  KStaticGeometryInstance(const_iterator begin, const_iterator end);
  KTriangleIndexCloud m_indexCloud;
};

KStaticGeometryInstance::KStaticGeometryInstance(const_iterator begin, const_iterator end)
{
  KTriangleElement const*elm;
  size_t numTriangles = std::distance(begin, end);
  size_t numIndicies  = 3 * numTriangles;
  m_indexCloud.reserve(numIndicies);
  while (begin != end)
  {
    elm = &*begin;
    m_indexCloud.push_back(elm->indices[0] - 1);
    m_indexCloud.push_back(elm->indices[1] - 1);
    m_indexCloud.push_back(elm->indices[2] - 1);
    ++begin;
  }
}

struct KTriangleVertexIterator : public std::iterator<std::random_access_iterator_tag, KVector3D>
{
  typedef KTriangleCloud::const_iterator parent_iterator;
  KTriangleVertexIterator(parent_iterator it, KPointCloud const *points) :
    m_index(0), m_cloud(points), m_parent(it)
  {
    // Intentionally Empty
  }

  void operator++()
  {
    ++m_index;
    if (m_index >= 3)
    {
      m_index = 0;
      ++m_parent;
    }
  }
  KVector3D const& operator*() const
  {
    // Note: Recall that Half Edge indices are stored as they are referenced
    //       from the obj - starting from an index of 1.
    return (*m_cloud)[m_parent->indices[m_index] - 1].point;
  }
  char m_index;
  KPointCloud const *m_cloud;
  parent_iterator m_parent;
};

bool operator!=(KTriangleVertexIterator const &lhs, KTriangleVertexIterator const &rhs)
{
  return (lhs.m_parent != rhs.m_parent);
}

struct KPartitionTrianglesAlongAxis : std::unary_function<bool, KTriangleElement>
{
public:

  KPartitionTrianglesAlongAxis(KVector3D const &center, KVector3D const &axis, KPointCloud const &cloud) :
    m_axis(axis), m_pointCloud(&cloud), m_midDot(KVector3D::dotProduct(center, axis))
  {
    // Intentionally Empty
  }

  bool operator()(KTriangleElement const &tri) const
  {
    return (KVector3D::dotProduct((*m_pointCloud)[tri.indices[0] - 1].point, m_axis) < m_midDot)
        && (KVector3D::dotProduct((*m_pointCloud)[tri.indices[1] - 1].point, m_axis) < m_midDot)
        && (KVector3D::dotProduct((*m_pointCloud)[tri.indices[2] - 1].point, m_axis) < m_midDot);
  }

  float m_midDot;
  KVector3D m_axis;
  KPointCloud const *m_pointCloud;
};

struct KStaticGeometryNode
{
  typedef KTriangleCloud::const_iterator const_iterator;

  KStaticGeometryNode(size_t depth, const_iterator begin, const_iterator end, KPointCloud const &pointCloud);
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

KStaticGeometryNode::KStaticGeometryNode(size_t d, const_iterator begin, const_iterator end, KPointCloud const &pointCloud) :
  aabb(KTriangleVertexIterator(begin, &pointCloud), KTriangleVertexIterator(end, &pointCloud)),
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
class KStaticGeometryPrivate : public OpenGLFunctions
{
public:
  typedef KTriangleCloud::iterator TriangleIterator;
  typedef KStaticGeometry::TerminationPred TerminationPred;
  KStaticGeometryPrivate();
  void buildBottomUp(TerminationPred pred);
  void buildTopDown(TerminationPred pred);

  // Possible clouds
  KAabbCloud m_aabbCloud;
  KPointCloud m_pointCloud;
  KTriangleCloud m_triangleCloud;
  KStaticGeometryNode *m_root;
  size_t m_maxDepth;

private:
  KStaticGeometryNode *recursiveTopDown(size_t depth, TriangleIterator begin, TriangleIterator end, TerminationPred pred);
};

KStaticGeometryPrivate::KStaticGeometryPrivate()
{
  initializeOpenGLFunctions();
}

void KStaticGeometryPrivate::buildBottomUp(TerminationPred pred)
{
  typedef std::vector<KStaticGeometryNode*> KStaticGeometryNodeCloud;
  KStaticGeometryNodeCloud nodes;
  size_t numTriangles = m_triangleCloud.size();
  nodes.reserve(numTriangles);

  int leafCount;
  int depthEstimate;
  int leafSize = static_cast<int>(numTriangles);
  do
  {
    leafCount = static_cast<int>(numTriangles / leafSize);
    depthEstimate = std::ceil(std::log(leafSize) / Karma::Log2);
    leafSize /= 2;
  }
  while (pred(numTriangles / leafSize, depthEstimate));

  // Initial Construction
  size_t remaining;
  KStaticGeometryNode *currNode;
  KTriangleCloud::const_iterator it = m_triangleCloud.begin();
  while (it != m_triangleCloud.cend())
  {
    remaining = std::distance(it, m_triangleCloud.cend());
    if (remaining > leafCount)
      remaining = leafCount;
    currNode = new KStaticGeometryNode(0, it, it + remaining, m_pointCloud);
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
  size_t numPoints = std::distance(begin, end);

  if (numPoints == 0) return 0;
  if (m_maxDepth < depth) m_maxDepth = depth;

  KStaticGeometryNode *node = new KStaticGeometryNode(depth, begin, end, m_pointCloud);
  if (!pred(numPoints, depth))
  {
    KVector3D const &maxAxis = node->aabb.maxAxis();
    TriangleIterator secondHalf = std::partition(begin, end, KPartitionTrianglesAlongAxis(node->aabb.center(), maxAxis, m_pointCloud));
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
  m_root = recursiveTopDown(0, m_triangleCloud.begin(), m_triangleCloud.end(), pred);
}

/*******************************************************************************
 * KStaticGeometryPrivate
 ******************************************************************************/
KStaticGeometry::KStaticGeometry() :
  m_private(new KStaticGeometryPrivate)
{
  // Intentionall Empty
}

KStaticGeometry::~KStaticGeometry()
{
  delete m_private;
}

void KStaticGeometry::addGeometry(const KHalfEdgeMesh &mesh, KTransform3D &transform)
{
  P(KStaticGeometryPrivate);
  size_t currOffset = p.m_pointCloud.size();

  // Transform and store all vertices
  KMatrix4x4 const &modelToWorld = transform.toMatrix();
  for (KHalfEdgeMesh::Vertex const & v: mesh.vertices())
  {
    KVector3D temp = modelToWorld * v.position;
    p.m_pointCloud.emplace_back(temp);
  }

  // Aggregate all of the triangles formed of the vertices
  KHalfEdgeMesh::HalfEdge const *hEdge[3];
  for (KHalfEdgeMesh::Face const & f: mesh.faces())
  {
    hEdge[0] = mesh.halfEdge(f.first);
    hEdge[1] = mesh.halfEdge(hEdge[0]->next);
    hEdge[2] = mesh.halfEdge(hEdge[1]->next);
    p.m_triangleCloud.emplace_back(hEdge[0]->to + currOffset, hEdge[1]->to + currOffset, hEdge[2]->to + currOffset);
  }
}

void KStaticGeometry::build(BuildMethod method, TerminationPred pred)
{
  P(KStaticGeometryPrivate);

  // If there is no new geometry to build, do nothing.
  if (!isDirty()) return;

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

bool KStaticGeometry::isDirty() const
{
  P(KStaticGeometryPrivate);
  return (!p.m_pointCloud.empty() || !p.m_triangleCloud.empty());
}

void KStaticGeometry::clear()
{
  P(KStaticGeometryPrivate);
  p.m_pointCloud.clear();
  p.m_triangleCloud.clear();
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
