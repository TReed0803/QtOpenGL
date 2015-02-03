#ifndef KHALFEDGEMESH_H
#define KHALFEDGEMESH_H

#include <list>
#include <KAbstractMesh>
#include <KVector3D>

class QString;

class KHalfEdgeMeshPrivate;
class KHalfEdgeMesh : public KAbstractMesh
{
public:
  typedef uint64_t VertexIndex;
  typedef uint64_t EdgeIndex;
  typedef uint64_t HalfEdgeIndex;
  typedef uint64_t FaceIndex;

  struct Vertex
  {
    KVector3D position;
    HalfEdgeIndex to;
  };

  struct HalfEdge
  {
    VertexIndex to;
    FaceIndex face;
    HalfEdgeIndex next;
  };

  struct Edge
  {
    HalfEdge a;
    HalfEdge b;
  };

  struct Face
  {
    HalfEdgeIndex first;
  };

  typedef std::vector<Vertex> VertexList;
  typedef std::vector<Edge> EdgeList;
  typedef std::vector<Face> FaceList;

  KHalfEdgeMesh(QObject *parent, const QString &fileName);
  ~KHalfEdgeMesh();

  // Add Commands (Does not check if value already exists!)
  VertexIndex addVertex(const KVector3D &v);
  FaceIndex addFace(index_array &a, index_array &b, index_array &c);

  // Query Commands (start from 1)
  Vertex const *vertex(VertexIndex idx) const;
  Face const *face(FaceIndex idx) const;
  VertexList const &vertices() const;
  EdgeList const &edges() const;
  FaceList const &faces() const;

  // Creates the OpenGL Mesh
  virtual OpenGLMesh* createOpenGLMesh(OpenGLMesh::Options options);

private:
  KHalfEdgeMeshPrivate *m_private;
  KVector3D calculateNormal(const HalfEdge *edge);
};

#endif // KHALFEDGEMESH_H
