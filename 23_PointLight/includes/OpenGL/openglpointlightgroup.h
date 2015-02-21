#ifndef OPENGLPOINTLIGHTGROUP_H
#define OPENGLPOINTLIGHTGROUP_H

class KTransform3D;
#include <QObject>
#include <vector>
#include <OpenGLPointLight>

class OpenGLMesh;
class KMatrix4x4;

class OpenGLPointLightGroupPrivate;
class OpenGLPointLightGroup : public QObject
{
  Q_OBJECT
public:
  typedef std::vector<OpenGLPointLight*> LightList;
  OpenGLPointLightGroup(QObject *parent = 0);
  void update(const KMatrix4x4 &perspective, const KMatrix4x4 &worldToCamera);
  void draw();
  void setMesh(OpenGLMesh *mesh);
  OpenGLPointLight *createLight();
  LightList &lights();
  LightList const &lights() const;
private:
  OpenGLPointLightGroupPrivate *m_private;
};

#endif // OPENGLPOINTLIGHTGROUP_H
