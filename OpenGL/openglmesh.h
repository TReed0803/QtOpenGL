#ifndef OPENGLMESH_H
#define OPENGLMESH_H OpenGLMesh

#include <cstdint>
#include <KSharedPointer>
#include <OpenGLBuffer>
#include <OpenGLElementType>

class KHalfEdgeMesh;
class KAabbBoundingVolume;

class OpenGLMeshPrivate;
class OpenGLMesh
{
public:

  typedef OpenGLBuffer::UsagePattern UsagePattern;

  // Constructors / Destructor
  OpenGLMesh();
  ~OpenGLMesh();

  // Public Methods
  void bind();
  void setUsagePattern(UsagePattern pattern);
  void create(const char *filename);
  void create(const KHalfEdgeMesh &mesh);
  void draw();
  void drawInstanced(size_t begin, size_t end);
  void vertexAttribPointer(int location, int elements, OpenGLElementType type, bool normalized, int stride, int offset);
  void vertexAttribPointer(int location, int elements, int count, OpenGLElementType type, bool normalized, int stride, int offset);
  void vertexAttribPointerDivisor(int location, int elements, OpenGLElementType type, bool normalized, int stride, int offset, int divisor);
  void vertexAttribPointerDivisor(int location, int elements, int count, OpenGLElementType type, bool normalized, int stride, int offset, int divisor);
  void release();
  bool isCreated() const;
  int objectId() const;
  KAabbBoundingVolume const &aabb() const;

private:
  KSharedPointer<OpenGLMeshPrivate> m_private;
};

#endif // OPENGLMESH_H
