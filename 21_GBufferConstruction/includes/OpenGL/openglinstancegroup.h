#ifndef OPENGLINSTANCEGROUP_H
#define OPENGLINSTANCEGROUP_H

class OpenGLMesh;
class KTransform3D;
#include <QObject>
#include <vector>
#include <OpenGLInstance>

class OpenGLInstanceGroupPrivate;
class OpenGLInstanceGroup : public QObject
{
  Q_OBJECT
public:
  typedef std::vector<OpenGLInstance*> InstanceList;
  OpenGLInstanceGroup(QObject *parent = 0);
  void setMesh(OpenGLMesh *mesh);
  void update();
  void draw();
  OpenGLInstance *createInstance();
  InstanceList &instances();
  InstanceList const &instances() const;
private:
  OpenGLInstanceGroupPrivate *m_private;
};

#endif // OPENGLINSTANCEGROUP_H
