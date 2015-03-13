#ifndef OPENGLSPOTLIGHTGROUP_H
#define OPENGLSPOTLIGHTGROUP_H OpenGLSpotLightGroup

#include <OpenGLLightGroup>
#include <OpenGLSpotLight>
#include <OpenGLLightData>

class KMatrix4x4;
class OpenGLMesh;

class OpenGLSpotLightGroup : public OpenGLLightGroup<OpenGLSpotLight, OpenGLLightData>
{
public:
  bool create();
  void initializeMesh(OpenGLMesh &mesh);
  void translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end);
  void translateUniforms(const OpenGLRenderBlock &stats, Byte *data, SizeType step, ConstLightIterator begin, ConstLightIterator end);
};

#endif // OPENGLSPOTLIGHTGROUP_H
