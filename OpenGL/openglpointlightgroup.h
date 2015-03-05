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
  void initializeMesh(OpenGLMesh &mesh);
  void translateData(const KMatrix4x4 &perspective, const KMatrix4x4 &view, DataPointer data);
  void draw();
};

#endif // OPENGLPOINTLIGHTGROUP_H
