#ifndef KABSTRACTSCENE_H
#define KABSTRACTSCENE_H KAbstractScene

class KAbstractScene
{
public:
  virtual void start() = 0;
  virtual void update(KUpdateEvent *event) = 0;
  virtual void end() = 0;
};

#endif // KABSTRACTSCENE_H

