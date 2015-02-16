#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include <vector>
#include <OpenGLBuffer>
#include <QObject>
class OpenGLVertexArrayObject;

class OpenGLMeshPrivate;
class OpenGLMesh : public QObject
{
  Q_OBJECT
public:
  enum Options
  {
    Contiguous,
    Interleaved,
    Indexed
  };

  typedef std::vector<OpenGLBuffer> OpenGLBufferList;
  explicit OpenGLMesh(QObject *parent = 0);
  OpenGLBufferList const &getBuffers() const;
  OpenGLVertexArrayObject *vertexArrayObject();
  OpenGLBuffer createBuffer(OpenGLBuffer::UsagePattern hint = OpenGLBuffer::StaticDraw);
  void setMode(OpenGLMesh::Options options);
  void setDrawArrays(GLenum mode, int count);
  void draw();
  GLenum mode() const;
  uint64_t count() const;
private:
  OpenGLMeshPrivate *m_private;
};

inline OpenGLMesh::Options operator|(OpenGLMesh::Options lhs, OpenGLMesh::Options rhs)
{
  return static_cast<OpenGLMesh::Options>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

#endif // OPENGLMESH_H
