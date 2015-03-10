#ifndef   OPENGLINSTANCE_H
#define   OPENGLINSTANCE_H OpenGLInstance

class KTransform3D;
class OpenGLMaterial;
class KHalfEdgeMesh;
class OpenGLMesh;
#include <string>

class OpenGLInstancePrivate;
class OpenGLInstance
{
public:
  OpenGLInstance();
  ~OpenGLInstance();

  KTransform3D &transform();
  KTransform3D &currentTransform();
  KTransform3D &previousTransform();
  void setMesh(const std::string &fileName);
  const std::string &mesh() const;
  void setMaterial(OpenGLMaterial const &mat);
  OpenGLMaterial &material();
  OpenGLMaterial const &material() const;
  void update();

  static int Stride();
  static int CurrentTransformOffset();
  static int PreviousTransformOffset();
  static int NormalsTransformOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
private:
  OpenGLInstancePrivate *m_private;
};

inline int OpenGLInstance::Stride()
{
  return sizeof(float) * (16 + 16 + 16 + 3 + 4);
}

inline int OpenGLInstance::DiffuseOffset()
{
  return 0;
}

inline int OpenGLInstance::SpecularOffset()
{
  return sizeof(float) * (3);
}

inline int OpenGLInstance::CurrentTransformOffset()
{
  return sizeof(float) * (3 + 4);
}

inline int OpenGLInstance::PreviousTransformOffset()
{
  return sizeof(float) * (3 + 4 + 16);
}

inline int OpenGLInstance::NormalsTransformOffset()
{
  return sizeof(float) * (3 + 4 + 16 + 16);
}

#endif // OPENGLINSTANCE_H
