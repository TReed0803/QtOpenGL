#ifndef OPENGLSPOTLIGHTGROUP_H
#define OPENGLSPOTLIGHTGROUP_H OpenGLSpotLightGroup

#include <OpenGLLightGroup>
#include <OpenGLSpotLight>
#include <OpenGLSpotLightData>

class KMatrix4x4;
class OpenGLMesh;

class OpenGLSpotLightGroup : public OpenGLLightGroup<OpenGLSpotLight, OpenGLSpotLightData>
{
public:
  void initializeMesh(OpenGLMesh &mesh);
  void translateData(const KMatrix4x4 &perspective, const KMatrix4x4 &view, DataPointer data);
  void draw();
};

#endif // OPENGLSPOTLIGHTGROUP_H
