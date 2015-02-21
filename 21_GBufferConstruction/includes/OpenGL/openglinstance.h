#ifndef   OPENGLINSTANCE_H
#define   OPENGLINSTANCE_H

class KTransform3D;
class OpenGLMaterial;

class OpenGLInstancePrivate;
class OpenGLInstance
{
public:
  OpenGLInstance();
  ~OpenGLInstance();

  KTransform3D &transform();
  KTransform3D &currentTransform();
  KTransform3D &previousTransform();
  void setMaterial(OpenGLMaterial const &mat);
  OpenGLMaterial &material();
  OpenGLMaterial const &material() const;
  void update();

  static int Stride();
  static int CurrentTransformOffset();
  static int PreviousTransformOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
private:
  OpenGLInstancePrivate *m_private;
};

inline int OpenGLInstance::Stride()
{
  return sizeof(float) * (16 + 16 + 3 + 3 + 1);
}

inline int OpenGLInstance::CurrentTransformOffset()
{
  return 0;
}

inline int OpenGLInstance::PreviousTransformOffset()
{
  return sizeof(float) * (16);
}

inline int OpenGLInstance::DiffuseOffset()
{
  return sizeof(float) * (16 + 16);
}

inline int OpenGLInstance::SpecularOffset()
{
  return sizeof(float) * (16 + 16 + 3);
}

#endif // OPENGLINSTANCE_H
