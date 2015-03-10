#ifndef OPENGLINSTANCEGROUP_H
#define OPENGLINSTANCEGROUP_H OpenGLInstanceGroup

#include <vector>

class OpenGLRenderBlock;
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
  void setMesh(const std::string &fileName);
  void setMesh(const OpenGLMesh &mesh);
  void update(const OpenGLRenderBlock &current, const OpenGLRenderBlock &previous);
  void draw();
  void clear();
  void addInstance(OpenGLInstance *instance);

  // Group Queries
  OpenGLInstance *createInstance();
  InstanceList &instances();
  InstanceList const &instances() const;
private:
  OpenGLInstanceGroupPrivate *m_private;
};

#endif // OPENGLINSTANCEGROUP_H
