#include "khalfedgemesh.h"
#include <QFile>
#include <KFileReader>
#include <KHalfEdgeObjParser>

#include <unordered_map>
#include <algorithm>
#include <KVertex>
#include <OpenGLBuffer>
#include <OpenGLVertexArrayObject>
#include "openglmesh.h"
#include <OpenGLFunctions>

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
  typedef KHalfEdgeMesh::EdgeIndex EdgeIndex;
  typedef KHalfEdgeMesh::HalfEdgeIndex HalfEdgeIndex;
  typedef KHalfEdgeMesh::FaceIndex FaceIndex;
  // typedefs (Elements)
  typedef KHalfEdgeMesh::Vertex Vertex;
  typedef KHalfEdgeMesh::HalfEdge HalfEdge;
  typedef KHalfEdgeMesh::Edge Edge;
  typedef KHalfEdgeMesh::Face Face;
  // typedefs (Containers)
  typedef KHalfEdgeMesh::VertexContainer VertexContainer;
  typedef KHalfEdgeMesh::HalfEdgeContainer HalfEdgeContainer;
  typedef KHalfEdgeMesh::EdgeContainer EdgeContainer;
  typedef KHalfEdgeMesh::FaceContainer FaceContainer;
  typedef std::unordered_map<Indices,EdgeIndex,IndicesHash> EdgeLookup;

  // Add Commands (Does not check if value already exists!)
  VertexIndex addVertex(const KVector3D &v);
  EdgeIndex addEdge(const index_array &from, const index_array &to);
  HalfEdgeIndex addHalfEdge(const index_array &from, const index_array &to);
  FaceIndex addFace(index_array &a, index_array &b, index_array &c);

  // Query Commands (start from 1)
  Vertex *vertex(VertexIndex const &idx);
  Edge *edge(EdgeIndex const &idx);
  HalfEdge *twin(HalfEdge *idx);
  HalfEdge *halfEdge(HalfEdgeIndex const &idx);
  Face *face(FaceIndex const &idx);
  Vertex const *vertex(VertexIndex const &idx) const;
  Edge const *edge(EdgeIndex const &idx) const;
  HalfEdgeIndex twin(HalfEdgeIndex const &idx) const;
  HalfEdge const *twin(HalfEdge const *idx) const;
  HalfEdge const *halfEdge(HalfEdgeIndex const &idx) const;
  Face const *face(FaceIndex const &idx) const;
  VertexIndex index(Vertex const *v) const;
  EdgeIndex index(Edge const *e) const;
  HalfEdgeIndex index(HalfEdge const *he) const;
  FaceIndex index(Face const *f) const;
  VertexContainer const &vertices() const;
  EdgeContainer const &edges() const;
  FaceContainer const &faces() const;

  // Get Commands (start from 1, queries first)
  HalfEdgeIndex findHalfEdge(const index_array &from, const index_array &to);
  HalfEdgeIndex getHalfEdge(const index_array &from, const index_array &to);

  // Helpers
  void normalizeIndex(KAbstractMesh::index_type &v, size_t const &sizePlusOne);
  KVector3D calculateFaceNormal(const Face *face);
  KVector3D calculateVertexNormal(const Vertex *vertex);

private:
  VertexContainer m_vertices;
  EdgeContainer m_edges;
  FaceContainer m_faces;
  EdgeLookup m_edgeLookup;
};

KHalfEdgeMeshPrivate::VertexIndex KHalfEdgeMeshPrivate::addVertex(const KVector3D &v)
{
  m_vertices.emplace_back(v, 0);
  return m_vertices.size();
}

KHalfEdgeMeshPrivate::EdgeIndex KHalfEdgeMeshPrivate::addEdge(const index_array &from, const index_array &to)
{
  Indices idx(from[0], to[0]);
  m_edges.emplace_back(idx.low, idx.high);

  // Note: We want idx starting from 1.
  //       So we only sutract 1 from the total edges.
  EdgeIndex offset = m_edges.size();
  m_edgeLookup.emplace(idx, offset);

  return offset;
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::addHalfEdge(const index_array &from, const index_array &to)
{
  EdgeIndex edgeIndex = addEdge(from, to);

  // Convert to HalfEdgeIndex
  if (from[0] > to[0])
    return 2 * edgeIndex - 1;
  else
    return 2 * edgeIndex;
}

inline void KHalfEdgeMeshPrivate::normalizeIndex(KAbstractMesh::index_type &v, size_t const &sizePlusOne)
{
  if (v < sizePlusOne) return;
  v %= sizePlusOne;
  ++v;
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

  // Set Vertex half edges
  if (vertex(v1[0])->to == 0) vertex(v1[0])->to = edgeA;
  if (vertex(v2[0])->to == 0) vertex(v2[0])->to = edgeB;
  if (vertex(v3[0])->to == 0) vertex(v3[0])->to = edgeC;

  // Create Face
  m_faces.emplace_back(edgeA);
  FaceIndex faceIdx = m_faces.size();

  // Initialize half edges
  HalfEdge *edge;
  edge = halfEdge(edgeA);
  edge->face = faceIdx;
  edge->next = edgeB;
  edge = halfEdge(edgeB);
  edge->face = faceIdx;
  edge->next = edgeC;
  edge = halfEdge(edgeC);
  edge->face = faceIdx;
  edge->next = edgeA;

  return faceIdx;
}

KHalfEdgeMeshPrivate::Vertex *KHalfEdgeMeshPrivate::vertex(const VertexIndex &idx)
{
  return &m_vertices[idx - 1];
}

KHalfEdgeMeshPrivate::Edge *KHalfEdgeMeshPrivate::edge(const EdgeIndex &idx)
{
  return &m_edges[idx - 1];
}

KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::twin(HalfEdge *edge)
{
  return halfEdge(twin(index(edge)));
}

KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::halfEdge(const HalfEdgeIndex &idx)
{
  return &reinterpret_cast<HalfEdge*>(m_edges.data())[idx - 1];
}

KHalfEdgeMeshPrivate::Face *KHalfEdgeMeshPrivate::face(const FaceIndex &idx)
{
  return &m_faces[idx - 1];
}

KHalfEdgeMeshPrivate::Vertex const *KHalfEdgeMeshPrivate::vertex(const VertexIndex &idx) const
{
  return &m_vertices[idx - 1];
}

KHalfEdgeMeshPrivate::Edge const *KHalfEdgeMeshPrivate::edge(const EdgeIndex &idx) const
{
  return &m_edges[idx - 1];
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::twin(const HalfEdgeIndex &idx) const
{
  if (idx % 2)
    return idx + 1;
  else
    return idx - 1;
}

KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::twin(HalfEdge const *edge) const
{
  return halfEdge(twin(index(edge)));
}

KHalfEdgeMeshPrivate::HalfEdge const *KHalfEdgeMeshPrivate::halfEdge(const HalfEdgeIndex &idx) const
{
  return &reinterpret_cast<HalfEdge const*>(m_edges.data())[idx - 1];
}

KHalfEdgeMeshPrivate::Face const *KHalfEdgeMeshPrivate::face(const FaceIndex &idx) const
{
  return &m_faces[idx - 1];
}

KHalfEdgeMeshPrivate::VertexIndex KHalfEdgeMeshPrivate::index(Vertex const *v) const
{
  return (v - m_vertices.data()) + 1;
}

KHalfEdgeMeshPrivate::EdgeIndex KHalfEdgeMeshPrivate::index(Edge const *e) const
{
  return (e - m_edges.data()) + 1;
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::index(HalfEdge const *he) const
{
  return (he - reinterpret_cast<HalfEdge const*>(m_edges.data())) + 1;
}

KHalfEdgeMeshPrivate::FaceIndex KHalfEdgeMeshPrivate::index(Face const *f) const
{
  return (f - m_faces.data()) + 1;
}

KHalfEdgeMeshPrivate::VertexContainer const &KHalfEdgeMeshPrivate::vertices() const
{
  return m_vertices;
}

KHalfEdgeMeshPrivate::EdgeContainer const &KHalfEdgeMeshPrivate::edges() const
{
  return m_edges;
}

KHalfEdgeMeshPrivate::FaceContainer const &KHalfEdgeMeshPrivate::faces() const
{
  return m_faces;
}

KHalfEdgeMeshPrivate::HalfEdgeIndex KHalfEdgeMeshPrivate::findHalfEdge(const index_array &from, const index_array &to)
{
  EdgeLookup::const_iterator it = m_edgeLookup.find(Indices(from[0], to[0]));
  if (it == m_edgeLookup.end()) return 0;

  if (from[0] > to[0])
    return 2 * it->second - 1;
  else
    return 2 * it->second;
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
  c.normalize();

  return c;
}

KVector3D KHalfEdgeMeshPrivate::calculateVertexNormal(const Vertex *vertex)
{
  HalfEdgeIndex currIndex(0), twinIndex(0);
  KVector3D normal;
  const HalfEdge *startEdge = halfEdge(vertex->to);
  const HalfEdge *edge = startEdge;
  do
  {
    if (edge->face != 0)
      normal += calculateFaceNormal(face(edge->face));
    currIndex = index(edge);
    twinIndex = twin(currIndex);
    edge = halfEdge(twinIndex);
    edge = halfEdge(edge->next);
  }
  while (edge != startEdge);
  normal.normalize();

  return normal;
}

/*******************************************************************************
 * Half Edge Mesh Public
 ******************************************************************************/
#define P(c) c &p = *m_private

KHalfEdgeMesh::KHalfEdgeMesh(QObject *parent, const QString &fileName) :
  KAbstractMesh(parent), m_private(new KHalfEdgeMeshPrivate)
{
  KFileReader reader(fileName);
  KHalfEdgeObjParser parser(this, &reader);
  parser.parse();
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

const KHalfEdgeMesh::HalfEdgeIndex KHalfEdgeMesh::twin(KHalfEdgeMesh::HalfEdgeIndex idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.twin(idx);
}

const KHalfEdgeMesh::Edge *KHalfEdgeMesh::edge(KHalfEdgeMesh::EdgeIndex idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.edge(idx);
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
  return reinterpret_cast<HalfEdgeContainer const&>(p.edges());
}

KHalfEdgeMesh::EdgeContainer const &KHalfEdgeMesh::edges() const
{
  P(KHalfEdgeMeshPrivate);
  return p.edges();
}

KHalfEdgeMesh::FaceContainer const &KHalfEdgeMesh::faces() const
{
  P(KHalfEdgeMeshPrivate);
  return p.faces();
}

// Creates the OpenGL Mesh
OpenGLMesh* KHalfEdgeMesh::createOpenGLMesh(OpenGLMesh::Options options)
{
  P(KHalfEdgeMeshPrivate);
  FaceContainer const &faces = p.faces();
  size_t faceCount = faces.size();

  // Currently we don't support options
  (void)options;

  // Create information
  OpenGLFunctions f(QOpenGLContext::currentContext());
  OpenGLMesh *mesh = new OpenGLMesh(parent());
  OpenGLVertexArrayObject *vao = mesh->vertexArrayObject();
  OpenGLBuffer buffer = mesh->createBuffer();
  const HalfEdge *edge = Q_NULLPTR;

  // Load data into OpenGLBuffer
  vao->bind();
  {
    buffer.bind();

    // Send data to GPU
    QVector3D n;
    buffer.allocate(sizeof(KVertex) * faceCount * 3);
    KVertex *data = static_cast<KVertex*>(buffer.map(QOpenGLBuffer::WriteOnly));
    for (size_t i = 0; i < faceCount; ++i)
    {
      edge = p.halfEdge(faces[i].first);
      n = p.calculateFaceNormal(&faces[i]);
      data[3*i] = KVertex(vertex(edge->to)->position, n);
      edge = p.halfEdge(edge->next);
      data[3*i+1] = KVertex(vertex(edge->to)->position, n);
      edge = p.halfEdge(edge->next);
      data[3*i+2] = KVertex(vertex(edge->to)->position, n);
    }
    buffer.unmap();

    // Bind attributes
    f.glVertexAttribPointer(0, KVertex::PositionTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::positionOffset());
    f.glVertexAttribPointer(1, KVertex::ColorTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::colorOffset());
    f.glEnableVertexAttribArray(0);
    f.glEnableVertexAttribArray(1);
  }
  vao->release();

  // Initialize mesh
  mesh->setMode(OpenGLMesh::Contiguous | OpenGLMesh::Interleaved);
  mesh->setDrawArrays(GL_TRIANGLES, faceCount * 3);
  return mesh;
}
