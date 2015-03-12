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
private:
  KUniquePointer<SampleScenePrivate> m_private;
};

#endif // SAMPLESCENE_H
