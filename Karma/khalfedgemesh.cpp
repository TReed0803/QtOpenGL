#include "khalfedgemesh.h"
#include "kbufferedfilereader.h"
#include "khalfedgeobjparser.h"
#include "kvertex.h"

#include <algorithm>
#include <functional>
#include <unordered_map>

#include <QFile>

#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLMesh>
#include <OpenGLVertexArrayObject>

/*******************************************************************************
 * Indices (Fast Lookup)
 ******************************************************************************/
struct Indices
{
public:

  // Typedefs
  typedef KHalfEdgeMesh::index_type index_type;
  typedef KHalfEdgeMesh::index_pair index_pair;

  // Member Information
  union
  {
    struct
    {
      index_type high;
      index_type low;
    };
    index_pair all;
  };

  // Constructor / Operators
  inline Indices(index_type from, index_type to);
  inline bool operator<(const Indices &rhs) const;
  inline bool operator==(const Indices &rhs) const;
};

inline Indices::Indices(index_type from, index_type to)
{
  if (from > to)
  {
    high = from;
    low = to;
  }
  else
  {
    high = to;
    low = from;
  }
}

inline bool Indices::operator<(const Indices &rhs) const
{
  return all < rhs.all;
}
inline bool Indices::operator==(const Indices &rhs) const
{
  return all == rhs.all;
}

struct IndicesHash : public std::unary_function<Indices::index_pair const&, Indices const&>
{
  inline Indices::index_pair const& operator()(Indices const& rhs) const
  {
    return rhs.all;
  }
};

/*******************************************************************************
 * HalfEdgeMeshPrivate
 ******************************************************************************/
class KHalfEdgeMeshPrivate
{
public:

  // typedefs (Fundamental)
  typedef KHalfEdgeMesh::index_type index_type;
  typedef KHalfEdgeMesh::index_pair index_pair;
  typedef KHalfEdgeMesh::index_array index_array;
  // typedefs (Indices)
  typedef KHalfEdgeMesh::VertexIndex VertexIndex;
  typedef KHalfEdgeMesh::HalfEdgeIndex HalfEdgeIndex;
  typedef KHalfEdgeMesh::FaceIndex FaceIndex;
  // typedefs (Elements)
  typedef KHalfEdgeMesh::Vertex Vertex;
  typedef KHalfEdgeMesh::HalfEdge HalfEdge;
  typedef KHalfEdgeMesh::Face Face;
  // typedefs (Containers)
  typedef KHalfEdgeMesh::VertexContainer VertexContainer;
  typedef KHalfEdgeMesh::HalfEdgeContainer HalfEdgeContainer;
  typedef KHalfEdgeMesh::FaceContainer FaceContainer;
  typedef std::unordered_map<Indices,HalfEdgeIndex,IndicesHash> HalfEdgeLookup;

  // Add Commands (Does not check if value already exists!)
  inline VertexIndex addVertex(const KVector3D &v);
  HalfEdgeIndex addEdge(const index_array &from, const index_array &to);
  HalfEdgeIndex addHalfEdge(const index_array &from, const index_array &to);
  FaceIndex addFace(index_array &a, index_array &b, index_array &c);

  // Query Commands (index => elements)
  inline Vertex *vertex(VertexIndex const &idx);
  inline HalfEdge *halfEdge(HalfEdgeIndex const &idx);
  inline Face *face(FaceIndex const &idx);
  inline Vertex const *vertex(VertexIndex const &idx) const;
  inline HalfEdge const *halfEdge(HalfEdgeIndex const &idx) const;
  inline Face const *face(FaceIndex const &idx) const;

  // Query Commands (elements => index)
  inline VertexIndex index(Vertex const *v) const;
  inline HalfEdgeIndex index(HalfEdge const *he) const;
  inline FaceIndex index(Face const *f) const;

  // Traversal operations
  inline HalfEdge *twin(HalfEdge *he);
  inline HalfEdge *twin(HalfEdgeIndex const &idx);
  inline HalfEdge const *twin(HalfEdge const *he) const;
  inline HalfEdge const *twin(HalfEdgeIndex const &idx) const;
  inline HalfEdgeIndex twinIndex(HalfEdgeIndex const &idx) const;

  inline KVector3D edgeVector(HalfEdge const *he) const;
  inline KVector3D edgeVector(HalfEdgeIndex const &idx) const;

  inline HalfEdge *cwSwivel(HalfEdge *he);
  inline HalfEdge *cwSwivel(HalfEdgeIndex const &idx);
  inline HalfEdge const *cwSwivel(HalfEdge const*he) const;
  inline HalfEdge const *cwSwivel(HalfEdgeIndex const &idx) const;
  inline HalfEdgeIndex cwSwivelIndex(HalfEdgeIndex const &idx) const;

  inline HalfEdge *cwBounds(HalfEdge *he);
  inline HalfEdge *cwBounds(HalfEdgeIndex const &idx);
  inline HalfEdge const *cwBounds(HalfEdge const*he) const;
  inline HalfEdge const *cwBounds(HalfEdgeIndex const &idx) const;
  inline HalfEdgeIndex cwBoundsIndex(HalfEdgeIndex idx) const;

  // Container access
  inline VertexContainer const &vertices() const;
  inline HalfEdgeContainer const &halfEdges() const;
  inline FaceContainer const &faces() const;

  // Helpers
  HalfEdgeIndex findHalfEdge(const index_array &from, const index_array &to);
  HalfEdgeIndex getHalfEdge(const index_array &from, const index_array &to);
  void normalizeIndex(index_type &v, size_t const &sizePlusOne);
  void initializeInnerHalfEdge(HalfEdgeIndex const &he, FaceIndex const &f, HalfEdgeIndex const &next);
  KVector3D calculateFaceNormal(const Face *face);
  KVector3D calculateVertexNormal(const Vertex *vertex, std::vector<KVector3D> &accumulator);
  void connectBoundaries();
  void connectEdges(HalfEdge *edge);

private:
  VertexContainer m_vertices;
  HalfEdgeContainer m_halfEdges;
  FaceContainer m_faces;
  HalfEdgeLookup m_halfEdgeLookup;
};

/*******************************************************************************
 * HalfEdgeMeshPrivate :: Add Commands
 ******************************************************************************/
inline KHalfEdgeMeshPrivate::VertexIndex KHalfEdgeMeshPrivate::addVertex(const KVector3D &v)
{
  m_vertices.emplace_back(v, 0);
  return VertexIndex(static_cast<index_type>(m_vertices.size()));
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::addEdge(const index_array &from, const index_array &to)
{
  Indices idx(from[0], to[0]);

  // First Half Edge
  m_halfEdges.emplace_back(idx.low);
  HalfEdgeIndex offset = HalfEdgeIndex(static_cast<index_type>(m_halfEdges.size()));
  m_halfEdgeLookup.emplace(idx,offset);

  // Seconds Half Edge
  m_halfEdges.emplace_back(idx.high);
  return offset;
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::addHalfEdge(const index_array &from, const index_array &to)
{
  HalfEdgeIndex edgeIndex = addEdge(from, to);

  // Convert to HalfEdgeIndex
  if (from[0] > to[0])
    return edgeIndex;
  else
    return edgeIndex + 1;
}

KHalfEdgeMeshPrivate::FaceIndex KHalfEdgeMeshPrivate::addFace(index_array &v1, index_array &v2, index_array &v3)
{
  // Normalize Indices
  size_t size = m_vertices.size() + 1;

  // Normalize Indices
  normalizeIndex(v1[0], size);
  normalizeIndex(v2[0], size);
  normalizeIndex(v3[0], size);

  // Create edges
  HalfEdgeIndex edgeA = getHalfEdge(v1, v2);
  HalfEdgeIndex edgeB = getHalfEdge(v2, v3);
  HalfEdgeIndex edgeC = getHalfEdge(v3, v1);

  // Create Face
  m_faces.emplace_back(edgeA);
  FaceIndex faceIdx = FaceIndex(static_cast<index_type>(m_faces.size()));

  // Initialize Inner Half Edges
  initializeInnerHalfEdge(edgeA, faceIdx, edgeB);
  initializeInnerHalfEdge(edgeB, faceIdx, edgeC);
  initializeInnerHalfEdge(edgeC, faceIdx, edgeA);

  // Set Vertex half edges
  if (vertex(v1[0])->to == 0) vertex(v1[0])->to = edgeA;
  if (vertex(v2[0])->to == 0) vertex(v2[0])->to = edgeB;
  if (vertex(v3[0])->to == 0) vertex(v3[0])->to = edgeC;

  return faceIdx;
}

/*******************************************************************************
 * HalfEdgeMeshPrivate :: Query Commands (index => element)
 ******************************************************************************/
inline KHalfEdgeMeshPrivate::Vertex *KHalfEdgeMeshPrivate::vertex(const VertexIndex &idx)
{
  return &m_vertices[idx - 1];
}

inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::halfEdge(const HalfEdgeIndex &idx)
{
  return &m_halfEdges[idx - 1];
}

inline KHalfEdgeMeshPrivate::Face *KHalfEdgeMeshPrivate::face(const FaceIndex &idx)
{
  return &m_faces[idx - 1];
}

inline KHalfEdgeMeshPrivate::Vertex const *KHalfEdgeMeshPrivate::vertex(const VertexIndex &idx) const
{
  return &m_vertices[idx - 1];
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::halfEdge(const HalfEdgeIndex &idx) const
{
  return &m_halfEdges.data()[idx - 1];
}

inline KHalfEdgeMeshPrivate::Face const *KHalfEdgeMeshPrivate::face(const FaceIndex &idx) const
{
  return &m_faces[idx - 1];
}

/*******************************************************************************
 * HalfEdgeMeshPrivate :: Query Commands (element => index)
 ******************************************************************************/
inline KHalfEdgeMeshPrivate::VertexIndex KHalfEdgeMeshPrivate::index(Vertex const *v) const
{
  return (v - m_vertices.data()) + 1;
}

inline KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::index(HalfEdge const *he) const
{
  return (he - m_halfEdges.data()) + 1;
}

inline KHalfEdgeMeshPrivate::FaceIndex KHalfEdgeMeshPrivate::index(Face const *f) const
{
  return (f - m_faces.data()) + 1;
}

/*******************************************************************************
 * HalfEdgeMeshPrivate :: Query Commands (containers)
 ******************************************************************************/
inline KHalfEdgeMeshPrivate::VertexContainer const &KHalfEdgeMeshPrivate::vertices() const
{
  return m_vertices;
}

inline const KHalfEdgeMeshPrivate::HalfEdgeContainer &KHalfEdgeMeshPrivate::halfEdges() const
{
  return m_halfEdges;
}

inline KHalfEdgeMeshPrivate::FaceContainer const &KHalfEdgeMeshPrivate::faces() const
{
  return m_faces;
}

/*******************************************************************************
 * HalfEdgeMeshPrivate :: Traversal Commands
 ******************************************************************************/
inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::twin(HalfEdge *he)
{
  return halfEdge(twinIndex(index(he)));
}

inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::twin(HalfEdgeIndex const &idx)
{
  return halfEdge(twinIndex(idx));
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::twin(HalfEdge const *edge) const
{
  return halfEdge(twinIndex(index(edge)));
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::twin(HalfEdgeIndex const &idx) const
{
  return halfEdge(twinIndex(idx));
}

inline KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::twinIndex(const HalfEdgeIndex &idx) const
{
  if (idx % 2)
    return idx + 1;
  else
    return idx - 1;
}

/////

inline KVector3D KHalfEdgeMeshPrivate::edgeVector(HalfEdge const *he) const
{
  return vertex(he->to)->position - vertex(twin(he)->to)->position;
}

inline KVector3D KHalfEdgeMeshPrivate::edgeVector(HalfEdgeIndex const &idx) const
{
  return edgeVector(halfEdge(idx));
}

/////

inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::cwSwivel(HalfEdge *he)
{
  return halfEdge(cwSwivelIndex(index(he)));
}

inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::cwSwivel(HalfEdgeIndex const &idx)
{
  return halfEdge(cwSwivelIndex(idx));
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::cwSwivel(HalfEdge const *he) const
{
  return halfEdge(cwSwivelIndex(index(he)));
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::cwSwivel(HalfEdgeIndex const &idx) const
{
  return halfEdge(cwSwivelIndex(idx));
}

inline KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::cwSwivelIndex(HalfEdgeIndex const &idx) const
{
  return twin(idx)->next;
}

/////

inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::cwBounds(HalfEdge *he)
{
  return halfEdge(cwBoundsIndex(index(he)));
}

inline KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::cwBounds(HalfEdgeIndex const &idx)
{
  return halfEdge(cwBoundsIndex(idx));
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::cwBounds(HalfEdge const *he) const
{
  return halfEdge(cwBoundsIndex(index(he)));
}

inline KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::cwBounds(HalfEdgeIndex const &idx) const
{
  return halfEdge(cwBoundsIndex(idx));
}

inline KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::cwBoundsIndex(HalfEdgeIndex idx) const
{
  while (twin(idx)->face != 0)
  {
    idx = cwSwivelIndex(idx);
  }
  return idx;
}

/*******************************************************************************
 * HalfEdgeMeshPrivate :: Helper Commands
 ******************************************************************************/
KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::findHalfEdge(const index_array &from, const index_array &to)
{
  HalfEdgeLookup::const_iterator it = m_halfEdgeLookup.find(Indices(from[0], to[0]));
  if (it == m_halfEdgeLookup.end()) return 0;

  if (from[0] > to[0])
    return it->second;
  else
    return it->second + 1;
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::getHalfEdge(const index_array &from, const index_array &to)
{
  HalfEdgeIndex idx = findHalfEdge(from, to);
  if (idx == 0)
  {
    return addHalfEdge(from, to);
  }
  return idx;
}

inline void KHalfEdgeMeshPrivate::normalizeIndex(KAbstractMesh::index_type &v, size_t const &sizePlusOne)
{
  if (v < sizePlusOne) return;
  v %= sizePlusOne;
  ++v;
}

inline void KHalfEdgeMeshPrivate::initializeInnerHalfEdge(const KHalfEdgeMeshPrivate::HalfEdgeIndex &he, const KHalfEdgeMeshPrivate::FaceIndex &f, const KHalfEdgeMeshPrivate::HalfEdgeIndex &next)
{
  HalfEdge *edge = halfEdge(he);
  edge->face = f;
  edge->next = next;
}

KVector3D KHalfEdgeMeshPrivate::calculateFaceNormal(const Face *face)
{
  const HalfEdge *edge = halfEdge(face->first);

  KVector3D pos1 = vertex(edge->to)->position;
  edge = halfEdge(edge->next);
  KVector3D pos2 = vertex(edge->to)->position;
  edge = halfEdge(edge->next);
  KVector3D pos3 = vertex(edge->to)->position;

  KVector3D a = pos2 - pos1;
  KVector3D b = pos3 - pos1;
  KVector3D c = KVector3D::crossProduct(a,b);

  // Note: QVector3D cannot handle small values.
  float length = c.length();
  if (length != 0.0f) c /= length;

  return c;
}

struct DotTest : public std::binary_function<bool, KVector3D const&, KVector3D const&>
{
  DotTest(KVector3D const &x) : _x(x) {}
  inline bool operator()(KVector3D const&a) const
  {
    return (KVector3D::dotProduct(a,_x) >= 1.0f);
  }
private:
  KVector3D _x;
};

KVector3D KHalfEdgeMeshPrivate::calculateVertexNormal(const Vertex *vertex, std::vector<KVector3D> &accumulator)
{
  // If the vertex isn't a part of any face, abandon it.
  if (!vertex->to) return KVector3D();

  KVector3D cumulative, normal;
  const HalfEdge *startEdge = halfEdge(vertex->to);
  const HalfEdge *edge = startEdge;
  do
  {
    if (edge->face != 0)
    {
      normal = calculateFaceNormal(face(edge->face));
      if (std::none_of(accumulator.begin(), accumulator.end(), DotTest(normal)))
      {
        accumulator.push_back(normal);
        cumulative += normal;
      }
    }
    edge = cwSwivel(edge);
  }
  while (edge != startEdge);
  accumulator.clear();

  // Note: QVector3D cannot handle small values.
  float length = cumulative.length();
  if (length != 0.0f) cumulative /= length;

  return cumulative;
}

void KHalfEdgeMeshPrivate::connectBoundaries()
{
  for (HalfEdge &edge : m_halfEdges)
  {
    if (edge.face == 0 && edge.next == 0)
    {
      connectEdges(&edge);
    }
  }
}

void KHalfEdgeMeshPrivate::connectEdges(KHalfEdgeMeshPrivate::HalfEdge *edge)
{
  HalfEdge *prev = twin(cwBounds(edge));
  do
  {
    prev->next = index(edge);
    edge = prev;
    prev = twin(cwBounds(edge));
  } while (prev->next == 0);
}

/*******************************************************************************
 * Half Edge Mesh Public
 ******************************************************************************/


KHalfEdgeMesh::KHalfEdgeMesh(QObject *parent) :
  KAbstractMesh(parent), m_private(new KHalfEdgeMeshPrivate)
{
  // Intentionally Empty
}

KHalfEdgeMesh::KHalfEdgeMesh(QObject *parent, const QString &fileName) :
  KAbstractMesh(parent), m_private(new KHalfEdgeMeshPrivate)
{
  P(KHalfEdgeMeshPrivate);
  KBufferedFileReader reader(fileName, 2048);
  if (!reader.valid())
  {
    qFatal("Failed to open file: `%s`", qPrintable(fileName));
  }
  KHalfEdgeObjParser parser(this, &reader);
  parser.initialize();
  parser.parse();
  p.connectBoundaries();
}

KHalfEdgeMesh::~KHalfEdgeMesh()
{
  delete m_private;
}

KHalfEdgeMesh::VertexIndex KHalfEdgeMesh::addVertex(const KVector3D &v)
{
  P(KHalfEdgeMeshPrivate);
  return p.addVertex(v);
}

KHalfEdgeMesh::FaceIndex KHalfEdgeMesh::addFace(index_array &a, index_array &b, index_array &c)
{
  P(KHalfEdgeMeshPrivate);
  return p.addFace(a, b, c);
}

// Query Commands (start from 1)
KHalfEdgeMesh::Vertex const *KHalfEdgeMesh::vertex(VertexIndex idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.vertex(idx);
}

const KHalfEdgeMesh::HalfEdge *KHalfEdgeMesh::halfEdge(KHalfEdgeMesh::HalfEdgeIndex idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.halfEdge(idx);
}

KHalfEdgeMesh::Face const *KHalfEdgeMesh::face(FaceIndex idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.face(idx);
}

KHalfEdgeMesh::VertexContainer const &KHalfEdgeMesh::vertices() const
{
  P(KHalfEdgeMeshPrivate);
  return p.vertices();
}

const KHalfEdgeMesh::HalfEdgeContainer &KHalfEdgeMesh::halfEdges() const
{
  P(KHalfEdgeMeshPrivate);
  return p.halfEdges();
}


KHalfEdgeMesh::FaceContainer const &KHalfEdgeMesh::faces() const
{
  P(KHalfEdgeMeshPrivate);
  return p.faces();
}

KHalfEdgeMesh::VertexIndex KHalfEdgeMesh::index(Vertex const *v) const
{
  P(KHalfEdgeMeshPrivate);
  return p.index(v);
}

KHalfEdgeMesh::HalfEdgeIndex KHalfEdgeMesh::index(const KHalfEdgeMesh::HalfEdge *he) const
{
  P(KHalfEdgeMeshPrivate);
  return p.index(he);
}

KHalfEdgeMesh::FaceIndex KHalfEdgeMesh::index(Face const *f) const
{
  P(KHalfEdgeMeshPrivate);
  return p.index(f);
}

const KHalfEdgeMesh::HalfEdge *KHalfEdgeMesh::twin(const KHalfEdgeMesh::HalfEdge *he) const
{
  P(KHalfEdgeMeshPrivate);
  return p.twin(he);
}

KHalfEdgeMesh::HalfEdge const *KHalfEdgeMesh::twin(HalfEdgeIndex const &idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.twin(idx);
}

KHalfEdgeMesh::HalfEdgeIndex KHalfEdgeMesh::twinIndex(const KHalfEdgeMesh::HalfEdgeIndex &idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.twinIndex(idx);
}

// Creates the OpenGL Mesh
OpenGLMesh* KHalfEdgeMesh::createOpenGLMesh(OpenGLMesh::Options options)
{
  P(KHalfEdgeMeshPrivate);

  // Currently we don't support options
  (void)options;

  // Create information
  OpenGLFunctions f;
  f.initializeOpenGLFunctions();
  OpenGLMesh *mesh = new OpenGLMesh(parent());
  OpenGLVertexArrayObject *vao = mesh->vertexArrayObject();
  OpenGLBuffer vertexBuffer = mesh->createBuffer(OpenGLBuffer::VertexBuffer);
  OpenGLBuffer indexBuffer = mesh->createBuffer(OpenGLBuffer::IndexBuffer);

  // Load data into OpenGLBuffer
  vao->bind();
  {
    KHalfEdgeMesh::HalfEdge *halfEdge;

    // Send data to GPU
    QVector3D n;
    int i = 0;
    std::vector<KVector3D> accumulator;
    vertexBuffer.bind();
    vertexBuffer.allocate(static_cast<int>(sizeof(KVertex) * p.vertices().size()));
    KVertex *data = static_cast<KVertex*>(vertexBuffer.map(QOpenGLBuffer::WriteOnly));
    for (KHalfEdgeMesh::Vertex const&v : p.vertices())
    {
      data[i++] = KVertex(v.position, p.calculateVertexNormal(&v, accumulator));
    }
    vertexBuffer.unmap();

    i = 0;
    indexBuffer.bind();
    indexBuffer.allocate(static_cast<int>(sizeof(uint32_t) * p.faces().size() * 3));
    uint32_t *index = static_cast<uint32_t*>(indexBuffer.map(QOpenGLBuffer::WriteOnly));
    for (KHalfEdgeMesh::Face const&f : p.faces())
    {
      halfEdge = p.halfEdge(f.first);
      index[i++] = halfEdge->to - 1;
      halfEdge = p.halfEdge(halfEdge->next);
      index[i++] = halfEdge->to - 1;
      halfEdge = p.halfEdge(halfEdge->next);
      index[i++] = halfEdge->to - 1;
    }
    indexBuffer.unmap();

    // Bind attributes
    f.glVertexAttribPointer(0, KVertex::PositionTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::positionOffset());
    f.glVertexAttribPointer(1, KVertex::ColorTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::colorOffset());
    f.glEnableVertexAttribArray(0);
    f.glEnableVertexAttribArray(1);
  }
  vao->release();

  // Initialize mesh
  mesh->setMode(OpenGLMesh::Contiguous | OpenGLMesh::Interleaved | OpenGLMesh::Indexed);
  mesh->setDrawArrays(GL_TRIANGLES, static_cast<int>(p.faces().size() * 3));
  return mesh;
}
