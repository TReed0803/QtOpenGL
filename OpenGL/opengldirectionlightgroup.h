#ifndef OPENGLDIRECTIONLIGHTGROUP_H
#define OPENGLDIRECTIONLIGHTGROUP_H OpenGLDirectionLightGroup

#include <OpenGLLightGroup>
#include <OpenGLDirectionLight>
#include <OpenGLDirectionLightData>

class KMatrix4x4;
class OpenGLMesh;

class OpenGLDirectionLightGroup : public OpenGLLightGroup<OpenGLDirectionLight, OpenGLDirectionLightData>
{
public:
  void initializeMesh(OpenGLMesh &mesh);
  void translateData(const KMatrix4x4 &perspective, const KMatrix4x4 &view, DataPointer data);
  void draw();
};

#endif // OPENGLDIRECTIONLIGHTGROUP_H
