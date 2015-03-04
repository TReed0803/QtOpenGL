#ifndef OPENGLPOINTLIGHTGROUP_H
#define OPENGLPOINTLIGHTGROUP_H OpenGLPointLightGroup

#include <OpenGLPointLight>
#include <OpenGLPointLightData>
#include <OpenGLLightGroup>

class KMatrix4x4;
class OpenGLMesh;

class OpenGLPointLightGroupPrivate;
class OpenGLPointLightGroup : public OpenGLLightGroup<OpenGLPointLight, OpenGLPointLightData>
{
public:
  void setMesh(const OpenGLMesh &mesh);
  void update(const KMatrix4x4 &perspective, const KMatrix4x4 &view);
  void draw();
};

#endif // OPENGLPOINTLIGHTGROUP_H
