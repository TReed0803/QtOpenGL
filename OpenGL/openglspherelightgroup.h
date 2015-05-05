#ifndef OPENGLSPHERELIGHTGROUP_H
#define OPENGLSPHERELIGHTGROUP_H OpenGLSphereLightGroup

class OpenGLViewport;
class OpenGLSphereLight;
#include <KUniquePointer>
#include <vector>

class OpenGLSphereLightGroupPrivate;
class OpenGLSphereLightGroup
{
public:
  typedef std::vector<OpenGLSphereLight*> LightContainer;
  OpenGLSphereLightGroup();
  ~OpenGLSphereLightGroup();

  bool create();
  void commit(const OpenGLViewport &view);
  void draw();
  OpenGLSphereLight *createLight();

  LightContainer::iterator begin();
  LightContainer::iterator end();
  OpenGLSphereLight *operator[](int idx);
  int size() const;
private:
  KUniquePointer<OpenGLSphereLightGroupPrivate> m_private;
};

#endif // OPENGLSPHERELIGHTGROUP_H
