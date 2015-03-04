#ifndef OPENGLINSTANCEGROUP_H
#define OPENGLINSTANCEGROUP_H OpenGLInstanceGroup

class OpenGLMesh;
class KTransform3D;
#include <QObject>
#include <vector>
#include <OpenGLInstance>

class KMatrix4x4;

class OpenGLInstanceGroupPrivate;
class OpenGLInstanceGroup : public QObject
{
  Q_OBJECT
public:
  typedef std::vector<OpenGLInstance*> InstanceList;
  OpenGLInstanceGroup(QObject *parent = 0);
  void setMesh(const OpenGLMesh &mesh);
  void update(const KMatrix4x4 &currWorldToCamera, const KMatrix4x4 &prevWorldToCamera);
  void draw();
  OpenGLInstance *createInstance();
  InstanceList &instances();
  InstanceList const &instances() const;
private:
  OpenGLInstanceGroupPrivate *m_private;
};

#endif // OPENGLINSTANCEGROUP_H
