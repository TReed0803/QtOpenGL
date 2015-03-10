#ifndef SAMPLESCENE_H
#define SAMPLESCENE_H SampleScene

#include <KScene>
#include <KUniquePointer>

class SampleScenePrivate;
class SampleScene : public KScene
{
public:
  SampleScene();
  void start();
  void update(KUpdateEvent *event);
  void end();
private:
  KUniquePointer<SampleScenePrivate> m_private;
};

#endif // SAMPLESCENE_H
