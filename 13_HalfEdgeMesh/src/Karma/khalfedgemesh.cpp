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

struct Indices
{
  union
  {
    struct
    {
      uint32_t high;
      uint32_t low;
    };
    uint64_t all;
  };
  Indices(uint32_t from, uint32_t to)
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
  inline bool operator<(const Indices &rhs) const
  {
    return all < rhs.all;
  }
  inline bool operator==(const Indices &rhs) const
  {
    return all == rhs.all;
  }
};

struct IndicesHash : public std::unary_function<uint64_t, Indices const&>
{
  inline uint64_t operator()(Indices const& rhs) const
  {
    return rhs.all;
  }
};

class KHalfEdgeMeshPrivate
{
public:
  typedef KHalfEdgeMesh::Vertex Vertex;
  typedef KHalfEdgeMesh::Face Face;
  typedef KHalfEdgeMesh::HalfEdge HalfEdge;
  typedef KHalfEdgeMesh::Edge Edge;
  typedef KHalfEdgeMesh::VertexList VertexList;
  typedef KHalfEdgeMesh::EdgeList EdgeList;
  typedef KHalfEdgeMesh::FaceList FaceList;
  typedef KHalfEdgeMesh::VertexIndex VertexIndex;
  typedef KHalfEdgeMesh::EdgeIndex EdgeIndex;
  typedef KHalfEdgeMesh::HalfEdgeIndex HalfEdgeIndex;
  typedef KHalfEdgeMesh::FaceIndex FaceIndex;
  typedef KHalfEdgeMesh::index_array index_array;
  typedef std::unordered_map<Indices,EdgeIndex,IndicesHash> EdgeLookup;

  // Add Commands (Does not check if value already exists!)
  VertexIndex addVertex(const KVector3D &v);
  EdgeIndex addEdge(const index_array &from, const index_array &to);
  HalfEdgeIndex addHalfEdge(const index_array &from, const index_array &to);
  FaceIndex addFace(index_array &a, index_array &b, index_array &c);

  // Query Commands (start from 1)
  Vertex *vertex(uint64_t idx);
  Edge *edge(uint64_t idx);
  HalfEdge *halfEdge(uint64_t idx);
  Face *face(uint64_t idx);
  VertexList &vertices();
  EdgeList &edges();
  FaceList &faces();

  // Get Commands (start from 1, queries first)
  HalfEdgeIndex findHalfEdge(const index_array &from, const index_array &to);
  HalfEdgeIndex getHalfEdge(const index_array &from, const index_array &to);
private:
  VertexList m_vertices;
  EdgeList m_edges;
  FaceList m_faces;
  EdgeLookup m_edgeLookup;
};

KHalfEdgeMeshPrivate::VertexIndex KHalfEdgeMeshPrivate::addVertex(const KVector3D &v)
{
  Vertex vertex;
  vertex.position = v;
  vertex.to = 0;
  m_vertices.push_back(vertex);
  return m_vertices.size();
}

KHalfEdgeMeshPrivate::EdgeIndex KHalfEdgeMeshPrivate::addEdge(const index_array &from, const index_array &to)
{
  Edge edge;
  Indices idx(from[0], to[0]);
  edge.a.face = edge.b.face = 0;
  edge.a.next = edge.b.next = 0;
  edge.a.to = idx.low;
  edge.b.to = idx.high;
  m_edges.push_back(edge);

  // Note: We want idx starting from 1.
  //       So we only sutract 1 from the total edges.
  EdgeIndex offset = m_edges.size();
  m_edgeLookup.insert(EdgeLookup::value_type(idx, offset));

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

KHalfEdgeMeshPrivate::FaceIndex KHalfEdgeMeshPrivate::addFace(index_array &v1, index_array &v2, index_array &v3)
{
  // Normalize Indices
  size_t size = m_vertices.size();
  v1[0] = (v1[0] - 1) % size + 1;
  v2[0] = (v2[0] - 1) % size + 1;
  v3[0] = (v3[0] - 1) % size + 1;

  // Create Face
  Face face;
  FaceIndex faceIdx = m_faces.size() + 1;

  // Create edges
  HalfEdge *edge;
  HalfEdgeIndex edgeA = getHalfEdge(v1, v2);
  HalfEdgeIndex edgeB = getHalfEdge(v2, v3);
  HalfEdgeIndex edgeC = getHalfEdge(v3, v1);

  // Set first HalfEdge
  face.first = edgeA;
  m_faces.push_back(face);

  // Initialize half edges
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

KHalfEdgeMeshPrivate::Vertex *KHalfEdgeMeshPrivate::vertex(uint64_t idx)
{
  return &m_vertices[idx - 1];
}

KHalfEdgeMeshPrivate::Edge *KHalfEdgeMeshPrivate::edge(uint64_t idx)
{
  return &m_edges[idx - 1];
}

KHalfEdgeMeshPrivate::HalfEdge *KHalfEdgeMeshPrivate::halfEdge(uint64_t idx)
{
  return &reinterpret_cast<HalfEdge*>(m_edges.data())[idx - 1];
}

KHalfEdgeMeshPrivate::Face *KHalfEdgeMeshPrivate::face(uint64_t idx)
{
  return &m_faces[idx - 1];
}

KHalfEdgeMeshPrivate::VertexList &KHalfEdgeMeshPrivate::vertices()
{
  return m_vertices;
}

KHalfEdgeMeshPrivate::EdgeList &KHalfEdgeMeshPrivate::edges()
{
  return m_edges;
}

KHalfEdgeMeshPrivate::FaceList &KHalfEdgeMeshPrivate::faces()
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

/*******************************************************************************
 * Half Edge Mesh Public
 ******************************************************************************/
#define P(c) c &p = *m_private

KHalfEdgeMesh::KHalfEdgeMesh(QObject *parent, const QString &fileName) :
  KAbstractMesh(parent), m_private(new KHalfEdgeMeshPrivate)
{
  QFile file(fileName);
  KFileReader reader(&file);
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

KHalfEdgeMesh::Face const *KHalfEdgeMesh::face(FaceIndex idx) const
{
  P(KHalfEdgeMeshPrivate);
  return p.face(idx);
}

KHalfEdgeMesh::VertexList const &KHalfEdgeMesh::vertices() const
{
  P(KHalfEdgeMeshPrivate);
  return p.vertices();
}

KHalfEdgeMesh::EdgeList const &KHalfEdgeMesh::edges() const
{
  P(KHalfEdgeMeshPrivate);
  return p.edges();
}

KHalfEdgeMesh::FaceList const &KHalfEdgeMesh::faces() const
{
  P(KHalfEdgeMeshPrivate);
  return p.faces();
}

// Creates the OpenGL Mesh
OpenGLMesh* KHalfEdgeMesh::createOpenGLMesh(OpenGLMesh::Options options)
{
  P(KHalfEdgeMeshPrivate);
  FaceList const &faces = p.faces();
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
      n = calculateNormal(edge);
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

KVector3D KHalfEdgeMesh::calculateNormal(const HalfEdge *edge)
{
  P(KHalfEdgeMeshPrivate);

  KVector3D pos1 = vertex(edge->to)->position;
  edge = p.halfEdge(edge->next);
  KVector3D pos2 = vertex(edge->to)->position;
  edge = p.halfEdge(edge->next);
  KVector3D pos3 = vertex(edge->to)->position;

  KVector3D a = pos2 - pos1;
  KVector3D b = pos3 - pos1;
  KVector3D c = KVector3D::crossProduct(a,b);
  c.normalize();

  return c;
}
