#ifndef HALFEDGEMESH_H
#define HALFEDGEMESH_H

class QFile;
class QString;
#include <QVector3D>
#include "abstractobjparser.h"
#include "abstractmesh.h"

class HalfEdgeMeshPrivate;
class HalfEdgeMesh : public AbstractMesh
{
public:

  struct HalfEdge;

  struct Vertex
  {
    QVector3D position;
    uint64_t i_from;
  };

  struct Face
  {
    uint64_t i_first;
  };

  struct HalfEdge
  {
    uint64_t i_to;
    uint64_t i_face;
    uint64_t i_next;
  };

  HalfEdgeMesh(QObject *parent, QFile *file);
  HalfEdgeMesh(QObject *parent, const QString &fileName);
  ~HalfEdgeMesh();
  uint64_t addVertex(const QVector3D &v);
  uint64_t addVertex(const AbstractObjParser::Vec4 &v);
  void addFace(const AbstractObjParser::Face &f);
  void addFace(const AbstractObjParser::FaceIndex &v1, const AbstractObjParser::FaceIndex &v2, const AbstractObjParser::FaceIndex &v3);
  uint64_t resolveEdge(const AbstractObjParser::FaceIndex &v1, const AbstractObjParser::FaceIndex &v2);
  Face *getFace(uint64_t idx);
  HalfEdge *getEdge(uint64_t idx);
  HalfEdge *getTwin(uint64_t edge);
  Vertex *getVertex(uint64_t idx);
  virtual OpenGLMesh* createOpenGLMesh(OpenGLMesh::Options options);

  uint64_t countVertexes() const;
  uint64_t countFaces() const;
  uint64_t countHalfEdges() const;
  uint64_t countEdges() const;
  std::vector<uint64_t> findFaceless();
private:
  HalfEdgeMeshPrivate *m_private;
  void triangulateFace(const AbstractObjParser::Face &f);
};

#endif // HALFEDGEMESH_H
