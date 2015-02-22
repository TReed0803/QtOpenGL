#include "openglframetimer.h"
#include <QElapsedTimer>
#include <KMacros>

/*******************************************************************************
 * OpenGLFrameTimerPrivate
 ******************************************************************************/
class OpenGLFrameTimerPrivate
{
public:
  // Constructor / Destructor
  OpenGLFrameTimerPrivate();

  // Public Members
  int m_frameCount;
  int m_frameCountFrequency;
  QElapsedTimer m_frameTimer;
};

OpenGLFrameTimerPrivate::OpenGLFrameTimerPrivate() :
  m_frameCount(0), m_frameCountFrequency(60)
{
  m_frameTimer.start();
}

/*******************************************************************************
 * OpenGLFrameTimer
 ******************************************************************************/


OpenGLFrameTimer::OpenGLFrameTimer(QObject *parent) :
  QObject(parent), m_private(new OpenGLFrameTimerPrivate)
{
  // Intentionally Empty
}

OpenGLFrameTimer::~OpenGLFrameTimer()
{
  delete m_private;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/
void OpenGLFrameTimer::setFrequency(int hz)
{
  P(OpenGLFrameTimerPrivate);
  if (hz <= 0) hz = 1;
  p.m_frameCountFrequency = hz;
}

int OpenGLFrameTimer::frequency() const
{
  P(OpenGLFrameTimerPrivate);
  return p.m_frameCountFrequency;
}

/*******************************************************************************
 * Public Slots
 ******************************************************************************/
void OpenGLFrameTimer::frameSwapped()
{
  P(OpenGLFrameTimerPrivate);
  if (++p.m_frameCount > p.m_frameCountFrequency)
  {
    qint64 ms = p.m_frameTimer.elapsed();
    float sec = ms / 1000.0f;
    emit timeout(p.m_frameCount / sec);
    p.m_frameCount = 0;
    p.m_frameTimer.start();
  }
}

