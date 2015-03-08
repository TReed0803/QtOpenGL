#ifndef OPENGLPOINTLIGHTGROUP_H
#define OPENGLPOINTLIGHTGROUP_H OpenGLPointLightGroup

#include <OpenGLPointLight>
#include <OpenGLPointLightData>
#include <OpenGLLightGroup>

#include <KMatrix4x4>
extern KMatrix4x4 testView;
extern KMatrix4x4 testPerspective;

class KMatrix4x4;
class OpenGLMesh;
class OpenGLRenderBlock;

class OpenGLPointLightGroupPrivate;
class OpenGLPointLightGroup : public OpenGLLightGroup<OpenGLPointLight, OpenGLPointLightData>
{
public:
  void initializeMesh(OpenGLMesh &mesh);
  void translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end);
  void translateUniforms(const OpenGLRenderBlock &stats, Byte *data, SizeType step, ConstLightIterator begin, ConstLightIterator end);
};

#endif // OPENGLPOINTLIGHTGROUP_H
