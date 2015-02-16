#ifndef OPENGLINSTANCEGROUP_H
#define OPENGLINSTANCEGROUP_H

class OpenGLMesh;
class KTransform3D;
#include <QObject>
#include <vector>
#include <KTransform3D>

class OpenGLInstanceGroupPrivate;
class OpenGLInstanceGroup : public QObject
{
  Q_OBJECT
public:
  OpenGLInstanceGroup(QObject *parent = 0);
  void setMesh(OpenGLMesh *mesh);
  void pushTransform(KTransform3D const &t);
  void updateBuffers();
  void clear();
  void draw();
  std::vector<KTransform3D> &transforms();
private:
  OpenGLInstanceGroupPrivate *m_private;
};

#endif // OPENGLINSTANCEGROUP_H
