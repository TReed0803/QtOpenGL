#ifndef OPENGLINSTANCEGROUP_H
#define OPENGLINSTANCEGROUP_H OpenGLInstanceGroup

#include <vector>

class KMatrix4x4;
class OpenGLInstance;
class OpenGLMesh;

class OpenGLInstanceGroupPrivate;
class OpenGLInstanceGroup
{
public:
  typedef std::vector<OpenGLInstance*> InstanceList;

  // Constructors / Destructor
  OpenGLInstanceGroup();
  ~OpenGLInstanceGroup();

  // Group Manipulation
  void create();
  void setMesh(const OpenGLMesh &mesh);
  void update(const KMatrix4x4 &currView, const KMatrix4x4 &prevView);
  void draw();

  // Group Queries
  OpenGLInstance *createInstance();
  InstanceList &instances();
  InstanceList const &instances() const;
private:
  OpenGLInstanceGroupPrivate *m_private;
};

#endif // OPENGLINSTANCEGROUP_H
