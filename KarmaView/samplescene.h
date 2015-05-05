#ifndef SAMPLESCENE_H
#define SAMPLESCENE_H SampleScene

#include <OpenGLScene>
#include <KUniquePointer>

class SampleScenePrivate;
class SampleScene : public OpenGLScene
{
public:
  SampleScene();
  ~SampleScene();
  void start();
  void update(KUpdateEvent *event);
  void end();
  void openMesh();
  void setMaterial(float r, float g, float b, float metal, float rough);
  void setRenderer(int fresnel, int geometric, int dist, int sample);
  void setFloorActive(bool f);
  void setFloorMaterial(float r, float g, float b, float metal, float rough);
  void setInstance(int metal, int rough, float mSep, float rSep);
  void setLightsAnimated(bool a);
  void setLights(int count, int baseTemp, int stepTemp, float intensity, float height, float radius);
  void setSphereLightsAnimated(bool a);
  void setSphereLights(int count, int baseTemp, int stepTemp, float intensity, float height, float radius, float vRadius);
  void setRectLightsAnimated(bool a);
  void setRectLights(int count, int baseTemp, int stepTemp, float intensity, float height, float radius, float vWidth, float vHeight);
  void setBvAabb(bool bv);
  void setBvObb(bool bv);
  void setBvSphereRitters(bool bv);
  void setBvSpherePca(bool bv);
  void setBvSphereCentroid(bool bv);
  void setBvEllipse(bool bv);
  void setBvSphereLarssons(bool bv);
  void setObjectRotation(float x, float y, float z);
private:
  KUniquePointer<SampleScenePrivate> m_private;
};

#endif // SAMPLESCENE_H
