#ifndef OPENGLDIRECTIONLIGHTGROUP_H
#define OPENGLDIRECTIONLIGHTGROUP_H OpenGLDirectionLightGroup

#include <OpenGLLightGroup>
#include <OpenGLDirectionLight>
#include <OpenGLLightData>

class KMatrix4x4;
class OpenGLMesh;

class OpenGLDirectionLightGroup : public OpenGLLightGroup<OpenGLDirectionLight, OpenGLLightData>
{
public:
  bool create();
  void initializeMesh(OpenGLMesh &mesh);
  void translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end);
  void translateUniforms(const OpenGLRenderBlock &stats, Byte *data, SizeType step, ConstLightIterator begin, ConstLightIterator end);
};

#endif // OPENGLDIRECTIONLIGHTGROUP_H
