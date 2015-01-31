#include "halfedgemesh.h"
#include "abstractobjparser.h"
#include <QFile>
#include "filereader.h"
#include "halfedgeobjparser.h"
#include <vector>
#include <algorithm>
#include "../vertex.h"
#include <OpenGLBuffer>
#include <OpenGLVertexArrayObject>
#include "openglmesh.h"
#include <OpenGLFunctions>

struct Indices : std::pair<uint64_t,uint64_t>
{
  Indices(uint64_t from, uint64_t to)
  {
    if (from > to)
    {
      first = from;
      second = to;
    }
    else
    {
      first = to;
      second = from;
    }
  }
  inline bool operator<(const Indices &rhs) const
  {
    if (first < rhs.first)
      return true;
    if (first == rhs.first && second < rhs.second)
      return true;
    return false;
  }
  inline bool operator==(const Indices &rhs) const
  {
    return first == rhs.first && second == rhs.second;
  }
};

class HalfEdgeMeshPrivate
{
public:
  typedef std::vector<HalfEdgeMesh::Vertex> VertexList;
  typedef std::vector<HalfEdgeMesh::Face> FaceList;
  typedef std::vector<HalfEdgeMesh::HalfEdge> HalfEdgeList;
  typedef std::map<Indices,uint64_t> HalfEdgeLookup;
  typedef OpenGLMesh* (HalfEdgeMeshPrivate::*BufferCtor)();

  VertexList m_vertexes;
  FaceList m_faces;
  HalfEdgeList m_edges;
  HalfEdgeLookup m_lookup;

  uint64_t findEdge(uint64_t from, uint64_t to);
  uint64_t createEdge(uint64_t from, uint64_t to);
  uint64_t createFace(uint64_t first);

  // Access
  HalfEdgeMesh::Face *getFace(uint64_t idx);
  HalfEdgeMesh::HalfEdge *getEdge(uint64_t idx);
  HalfEdgeMesh::Vertex *getVertex(uint64_t idx);
};

uint64_t HalfEdgeMeshPrivate::findEdge(uint64_t from, uint64_t to)
{
  HalfEdgeLookup::const_iterator it = m_lookup.find(Indices(from, to));
  if (it == m_lookup.end()) return 0;

  if (from > to)
    return it->second;
  else
    return it->second + 1;
}

uint64_t HalfEdgeMeshPrivate::createEdge(uint64_t from, uint64_t to)
{
  Indices idx(from,to);
  HalfEdgeMesh::HalfEdge edge;
  edge.i_face = 0;
  edge.i_next = 0;

  edge.i_to = idx.second;
  m_edges.push_back(edge);
  edge.i_to = idx.first;
  m_edges.push_back(edge);

  // Note: We want idx starting from 1.
  //       So we only sutract 1 from the total edges.
  uint64_t offset = m_edges.size() - 1;
  m_lookup[idx] = offset;
  if (from > to)
    return offset;
  else
    return offset + 1;
}

uint64_t HalfEdgeMeshPrivate::createFace(uint64_t first)
{
  HalfEdgeMesh::Face face;
  face.i_first = first;
  m_faces.push_back(face);
  return m_faces.size();
}

HalfEdgeMesh::Face *HalfEdgeMeshPrivate::getFace(uint64_t idx)
{
  return &m_faces[idx - 1];
}

HalfEdgeMesh::HalfEdge *HalfEdgeMeshPrivate::getEdge(uint64_t idx)
{
  return &m_edges[idx - 1];
}

HalfEdgeMesh::Vertex *HalfEdgeMeshPrivate::getVertex(uint64_t idx)
{
  return &m_vertexes[idx - 1];
}

/*******************************************************************************
 * Half Edge Mesh Public
 ******************************************************************************/
#define P(c) c &p = *m_private

HalfEdgeMesh::HalfEdgeMesh(QObject *parent, QFile *file) :
 AbstractMesh(parent), m_private(new HalfEdgeMeshPrivate)
{
  (void)file;
}

HalfEdgeMesh::HalfEdgeMesh(QObject *parent, const QString &fileName) :
  AbstractMesh(parent), m_private(new HalfEdgeMeshPrivate)
{
  QFile file(fileName);
  FileReader reader(&file);
  HalfEdgeObjParser parser(this, &reader);
  parser.parse();
}

HalfEdgeMesh::~HalfEdgeMesh()
{
  delete m_private;
}

uint64_t HalfEdgeMesh::addVertex(const QVector3D &v)
{
  P(HalfEdgeMeshPrivate);
  Vertex vert;
  vert.position = v;
  vert.i_from = 0;
  p.m_vertexes.push_back(vert);
  return p.m_vertexes.size();
}

uint64_t HalfEdgeMesh::addVertex(const AbstractObjParser::Vec4 &v)
{
  return addVertex(QVector3D(v.x, v.y, v.z));
}

void HalfEdgeMesh::addFace(const AbstractObjParser::Face &f)
{
  if (f.size() > 3)
    triangulateFace(f);
  else
    addFace(f[0], f[1], f[2]);
}

void HalfEdgeMesh::addFace(const AbstractObjParser::FaceIndex &v1, const AbstractObjParser::FaceIndex &v2, const AbstractObjParser::FaceIndex &v3)
{
  P(HalfEdgeMeshPrivate);
  HalfEdge *edge;
  uint64_t edgeA = resolveEdge(v1, v2);
  uint64_t edgeB = resolveEdge(v2, v3);
  uint64_t edgeC = resolveEdge(v3, v1);
  uint64_t faceIdx = p.createFace(edgeA);
  edge = getEdge(edgeA);
  edge->i_face = faceIdx;
  edge->i_next = edgeB;
  edge = getEdge(edgeB);
  edge->i_face = faceIdx;
  edge->i_next = edgeC;
  edge = getEdge(edgeC);
  edge->i_face = faceIdx;
  edge->i_next = edgeA;
}

uint64_t HalfEdgeMesh::resolveEdge(const AbstractObjParser::FaceIndex &v1, const AbstractObjParser::FaceIndex &v2)
{
  P(HalfEdgeMeshPrivate);
  uint64_t edge = p.findEdge(v1.v, v2.v);
  if (edge == 0)
  {
    return p.createEdge(v1.v, v2.v);
  }
  return edge;
}

HalfEdgeMesh::Face *HalfEdgeMesh::getFace(uint64_t idx)
{
  P(HalfEdgeMeshPrivate);
  return p.getFace(idx);
}

HalfEdgeMesh::HalfEdge *HalfEdgeMesh::getEdge(uint64_t idx)
{
  P(HalfEdgeMeshPrivate);
  return p.getEdge(idx);
}

HalfEdgeMesh::Vertex *HalfEdgeMesh::getVertex(uint64_t idx)
{
  P(HalfEdgeMeshPrivate);
  return p.getVertex(idx);
}

OpenGLMesh* HalfEdgeMesh::createOpenGLMesh(OpenGLMesh::Options options)
{
  P(HalfEdgeMeshPrivate);

  // Currently we don't support options
  (void)options;

  // Create information
  OpenGLFunctions f(QOpenGLContext::currentContext());
  OpenGLMesh *mesh = new OpenGLMesh(parent());
  OpenGLVertexArrayObject *vao = mesh->vertexArrayObject();
  OpenGLBuffer buffer = mesh->createBuffer();
  const HalfEdgeMesh::HalfEdge *edge = Q_NULLPTR;

  // Load data into OpenGLBuffer
  vao->bind();
  {
    buffer.bind();

    // Send data to GPU
    buffer.allocate(sizeof(::Vertex) * p.m_faces.size() * 3);
    ::Vertex *data = static_cast<::Vertex*>(buffer.map(QOpenGLBuffer::WriteOnly));
    for (size_t i = 0; i < p.m_faces.size(); ++i)
    {
      edge = getEdge(p.m_faces[i].i_first);
      data[3*i] = ::Vertex(getVertex(edge->i_to)->position);
      edge = getEdge(edge->i_next);
      data[3*i+1] = ::Vertex(getVertex(edge->i_to)->position);
      edge = getEdge(edge->i_next);
      data[3*i+2] = ::Vertex(getVertex(edge->i_to)->position);
    }
    buffer.unmap();

    // Bind attributes
    f.glVertexAttribPointer(0, ::Vertex::PositionTupleSize, GL_FLOAT, GL_FALSE, ::Vertex::stride(), (void*)::Vertex::positionOffset());
    f.glVertexAttribPointer(1, ::Vertex::ColorTupleSize, GL_FLOAT, GL_FALSE, ::Vertex::stride(), (void*)::Vertex::colorOffset());
    f.glEnableVertexAttribArray(0);
    f.glEnableVertexAttribArray(1);
  }
  vao->release();

  // Initialize mesh
  mesh->setMode(OpenGLMesh::Contiguous | OpenGLMesh::Interleaved);
  mesh->setDrawArrays(GL_TRIANGLES, p.m_faces.size() * 3);
  return mesh;
}

uint64_t HalfEdgeMesh::countVertexes() const
{
  P(HalfEdgeMeshPrivate);
  return p.m_vertexes.size();
}

uint64_t HalfEdgeMesh::countFaces() const
{
  P(HalfEdgeMeshPrivate);
  return p.m_faces.size();
}

uint64_t HalfEdgeMesh::countHalfEdges() const
{
  P(HalfEdgeMeshPrivate);
  return p.m_edges.size();
}

uint64_t HalfEdgeMesh::countEdges() const
{
  P(HalfEdgeMeshPrivate);
  return p.m_edges.size() / 2;
}

std::vector<uint64_t> HalfEdgeMesh::findFaceless()
{
  P(HalfEdgeMeshPrivate);
  std::vector<uint64_t> he;
  for (uint64_t i = 0; i < p.m_edges.size(); ++i)
  {
    if (p.m_edges[i].i_face == 0) he.push_back(i + 1);
  }
  return he;
}

HalfEdgeMesh::HalfEdge *HalfEdgeMesh::getTwin(uint64_t edge)
{
  // Remainder Mod 2 := Get next
  if (edge % 2)
    return getEdge(edge + 1);
  else
    return getEdge(edge - 1);
}

void HalfEdgeMesh::triangulateFace(const AbstractObjParser::Face &f)
{
  P(HalfEdgeMeshPrivate);

  AbstractObjParser::FaceIndex newIndex;
  newIndex.v = p.m_vertexes.size() + 1;
  newIndex.t = 0;
  newIndex.n = 0;

  // While we accumulate for the average, add the faces
  size_t i;
  QVector3D avg(getVertex(f[0].v)->position);
  for (i = 1; i < f.size(); ++i)
  {
    avg += getVertex(f[i].v)->position;
  }
  avg /= f.size();
  newIndex.v = addVertex(avg);

  for (i = 1; i < f.size(); ++i)
  {
    avg += getVertex(f[i].v)->position;
    addFace(f[i-1], f[i], newIndex);
  }
  addFace(f[i-1], f[0], newIndex);
}
