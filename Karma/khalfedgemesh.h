#ifndef KHALFEDGEMESH_H
#define KHALFEDGEMESH_H KHalfEdgeMesh

#include <KAbstractMesh>
#include <KVector3D>

class QString;

class KHalfEdgeMeshPrivate;
class KHalfEdgeMesh : public KAbstractMesh
{
private:

  // Private Type Definitions
  struct IndexType
  {
    typedef KAbstractMesh::index_type index_type;
    inline IndexType(index_type value);
    inline operator index_type const&() const;
    index_type m_value;
  };

public:

  struct VertexIndex;
  struct HalfEdgeIndex;
  struct FaceIndex;

  // Public Type Definitions (Indices)
  struct VertexIndex : public IndexType
  {
    inline VertexIndex(index_type value);
  private:
    VertexIndex(HalfEdgeIndex const&);
    VertexIndex(FaceIndex const&);
  };
  struct HalfEdgeIndex : public IndexType
  {
    inline HalfEdgeIndex(index_type value);
  private:
    HalfEdgeIndex(VertexIndex const&);
    HalfEdgeIndex(FaceIndex const&);
  };
  struct FaceIndex : public IndexType
  {
    inline FaceIndex(index_type value);
  private:
    FaceIndex(HalfEdgeIndex const&);
    FaceIndex(VertexIndex const&);
  };

  // Public Type Definitions (Elements)
  struct Vertex
  {
    inline Vertex(KVector3D const &p, HalfEdgeIndex const& t);
    KVector3D position;
    KVector3D normal;
    HalfEdgeIndex to;
  };
  struct HalfEdge
  {
    inline HalfEdge(VertexIndex const &to);
    VertexIndex to;
    FaceIndex face;
    HalfEdgeIndex next;
  };
  struct Face
  {
    inline Face(HalfEdgeIndex const &f);
    KVector3D normal;
    HalfEdgeIndex first;
  };

  // Public Type Definitions (Containers)
  typedef std::vector<Vertex> VertexContainer;
  typedef std::vector<HalfEdge> HalfEdgeContainer;
  typedef std::vector<Face> FaceContainer;

public:

  struct VertexPositionPred : public std::unary_function<KVector3D const&, Vertex const&>
  {
    KVector3D const& operator()(Vertex const& v) const
    {
      return v.position;
    }
  };

public:

  // Constructors / Destructor
  explicit KHalfEdgeMesh(QObject *parent = 0);
  KHalfEdgeMesh(QObject *parent, const QString &fileName);
  ~KHalfEdgeMesh();

  // Add Commands (Does not check if value already exists!)
  VertexIndex addVertex(const KVector3D &v);
  FaceIndex addFace(index_array &a, index_array &b, index_array &c);

  // Query Commands (index -> element)
  Vertex const *vertex(VertexIndex idx) const;
  HalfEdge const *halfEdge(HalfEdgeIndex idx) const;
  Face const *face(FaceIndex idx) const;
  VertexContainer const &vertices() const;
  HalfEdgeContainer const &halfEdges() const;
  FaceContainer const &faces() const;

  // Query Commands (element -> index)
  VertexIndex index(Vertex const *v) const;
  HalfEdgeIndex index(HalfEdge const *he) const;
  FaceIndex index(Face const *f) const;
  HalfEdge const *twin(HalfEdge const *he) const;
  HalfEdge const *twin(HalfEdgeIndex const &idx) const;
  HalfEdgeIndex twinIndex(HalfEdgeIndex const &he) const;

  // Mutation Commands
  void calculateFaceNormals();
  void calculateVertexNormals();

private:
  KHalfEdgeMeshPrivate *m_private;
};

inline KHalfEdgeMesh::IndexType::IndexType(index_type value) :
  m_value(value)
{
  // Intentionally Empty
}

inline KHalfEdgeMesh::IndexType::operator index_type const&() const
{
  return m_value;
}

inline KHalfEdgeMesh::VertexIndex::VertexIndex(index_type value) :
  KHalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline KHalfEdgeMesh::HalfEdgeIndex::HalfEdgeIndex(index_type value) :
  KHalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline KHalfEdgeMesh::FaceIndex::FaceIndex(index_type value) :
  KHalfEdgeMesh::IndexType(value)
{
  // Intentionally Empty
}

inline KHalfEdgeMesh::Vertex::Vertex(KVector3D const &p, HalfEdgeIndex const& t) :
  position(p), to(t)
{
  // Intentionally Empty
}

inline KHalfEdgeMesh::HalfEdge::HalfEdge(VertexIndex const &to) :
  to(to), face(0), next(0)
{
  // Intentionally Empty
}

inline KHalfEdgeMesh::Face::Face(HalfEdgeIndex const &f) :
  first(f)
{
  // Intentionally Empty
}

#endif // KHALFEDGEMESH_H
