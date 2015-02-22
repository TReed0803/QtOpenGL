#ifndef OPENGLFRAMETIMER_H
#define OPENGLFRAMETIMER_H OpenGLFrameTimer

#include <QObject>

class OpenGLFrameTimerPrivate;
class OpenGLFrameTimer : public QObject
{
  Q_OBJECT
public:

  // Constructors / Destructor
  explicit OpenGLFrameTimer(QObject *parent = 0);
  ~OpenGLFrameTimer();

  // Public Methods
  void setFrequency(int hz);
  int frequency() const;

signals:
  void timeout(float fps);

public slots:
  void frameSwapped();

private:
  OpenGLFrameTimerPrivate *m_private;
};

#endif // OPENGLFRAMETIMER_H
