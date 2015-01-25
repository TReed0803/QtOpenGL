#include "profiler.h"
#include <vector>
#include <stack>
#include <QOpenGLTimerQuery>
#include <QDebug>
#include "frameresult.h"

struct Marker
{
  int depth;
  QString name;
  QColor color;
};

/*******************************************************************************
 * GpuMarker Type
 ******************************************************************************/

struct GpuMarker : public Marker
{
  GpuMarker(QObject *obj = 0);
  ~GpuMarker();
  inline bool isResultAvailable();
  inline GLuint64 startTime() const;
  inline GLuint64 endTime() const;
  inline void start();
  inline void stop();
private:
  QOpenGLTimerQuery m_startTimer;
  QOpenGLTimerQuery m_endTimer;
};

GpuMarker::GpuMarker(QObject *obj) :
  m_startTimer(obj), m_endTimer(obj)
{
  m_startTimer.create();
  m_endTimer.create();
}

GpuMarker::~GpuMarker()
{
  m_startTimer.destroy();
  m_endTimer.destroy();
}

inline bool GpuMarker::isResultAvailable()
{
  return (m_startTimer.isResultAvailable() && m_endTimer.isResultAvailable());
}

inline GLuint64 GpuMarker::startTime() const
{
  return m_startTimer.waitForResult();
}

inline GLuint64 GpuMarker::endTime() const
{
  return m_endTimer.waitForResult();
}

inline void GpuMarker::start()
{
  m_startTimer.recordTimestamp();
}

inline void GpuMarker::stop()
{
  m_endTimer.recordTimestamp();
}

/*******************************************************************************
 * Marker Groups
 ******************************************************************************/

template <typename T>
struct MarkerGroup
{
  typedef T* MarkerPointer;
  typedef std::vector<MarkerPointer> MarkerContainer;
  typedef std::stack<size_t> IndexContainer;

  MarkerGroup(QObject *parent = 0);
  bool isResultAvailable();
  inline const MarkerContainer &markers() const;
  void pushMarker(const QString &name, const QColor &color);
  void popMarker();
  inline void clear();

private:
  MarkerContainer m_markers;
  IndexContainer m_activeMarkerIndex;
  size_t m_currMarker;
  size_t m_statusMarker;
  QObject *m_parent;
};

template <typename T>
MarkerGroup<T>::MarkerGroup(QObject *parent = 0) :
  m_currMarker(0), m_statusMarker(0), m_parent(parent)
{
  // Intentionally Blank
}

template <typename T>
bool MarkerGroup<T>::isResultAvailable()
{
  while(m_statusMarker < m_currMarker)
  {
    if (!m_markers[m_statusMarker]->isResultAvailable())
    {
      return false;
    }
    ++m_statusMarker;
  }
  return true;
}

template <typename T>
inline const typename MarkerGroup<T>::MarkerContainer &MarkerGroup<T>::markers() const
{
  return m_markers;
}

template <typename T>
void MarkerGroup<T>::pushMarker(const QString &name, const QColor &color)
{
  MarkerPointer marker;
  if (m_currMarker >= m_markers.size())
  {
    marker = new T(m_parent);
    m_markers.push_back(marker);
  }
  else
  {
    marker = m_markers[m_currMarker];
  }

  // Instantiate the marker
  marker->name = name;
  marker->color = color;
  marker->start();

  m_activeMarkerIndex.push(m_currMarker);
  ++m_currMarker;
}

template <typename T>
void MarkerGroup<T>::popMarker()
{
  size_t topIdx = m_activeMarkerIndex.top();
  m_activeMarkerIndex.pop();
  MarkerPointer marker = m_markers[topIdx];
  marker->stop();
}

template <typename T>
inline void MarkerGroup<T>::clear()
{
  m_currMarker = m_statusMarker = 0;
}

/*******************************************************************************
 * Frame Info
 ******************************************************************************/

struct FrameInfo
{
  typedef MarkerGroup<GpuMarker> GpuGroup;
  GpuGroup m_gpuMarkers;

  FrameInfo(QObject *parent = 0);
  inline bool isResultAvailable();
  FrameResult waitForResult();
  inline void pushGpuMarker(const QString &name, const QColor &color);
  inline void popGpuMarker();
  inline void clear();

private:
  QObject *m_parent;
};

FrameInfo::FrameInfo(QObject *parent) :
  m_parent(parent)
{
  // Intentionally Empty
}

inline bool FrameInfo::isResultAvailable()
{
  return m_gpuMarkers.isResultAvailable();
}

FrameResult FrameInfo::waitForResult()
{
  // Poll until available
  while (!m_gpuMarkers.isResultAvailable());

  // Aggregate the results
  FrameResult results;
  FrameResult::GpuMarkerResult markerResult;
  const GpuGroup::MarkerContainer &gpuMarkers = m_gpuMarkers.markers();
  for (size_t i = 0; i < gpuMarkers.size(); ++i)
  {
    const GpuMarker &marker = *gpuMarkers[i];
    markerResult.name = marker.name;
    markerResult.start = marker.startTime();
    markerResult.end = marker.endTime();
    results.m_gpuResults.push_back(markerResult);
  }

  return results;
}

inline void FrameInfo::pushGpuMarker(const QString &name, const QColor &color)
{
  m_gpuMarkers.pushMarker(name, color);
}

inline void FrameInfo::popGpuMarker()
{
  m_gpuMarkers.popMarker();
}

inline void FrameInfo::clear()
{
  m_gpuMarkers.clear();
}

/*******************************************************************************
 * ProfilerPrivate
 ******************************************************************************/

struct ProfilerPrivate
{
  typedef std::vector<FrameInfo*> FrameContainer;
  size_t m_currFrame;
  FrameContainer m_frames;
  ProfilerPrivate();
  ~ProfilerPrivate();
};

ProfilerPrivate::ProfilerPrivate() :
  m_currFrame(0)
{
  // Intentionally Empty
}

ProfilerPrivate::~ProfilerPrivate()
{
  for (size_t i = 0; i < m_frames.size(); ++i)
  {
    delete m_frames[i];
  }
}

/*******************************************************************************
 * Profiler
 ******************************************************************************/

#define P(t) t &p = *m_private
Profiler::Profiler(QObject *parent) :
  QObject(parent), m_private(new ProfilerPrivate)
{
  // Intentionally Empty
}

Profiler::~Profiler()
{
  delete m_private;
}

void Profiler::pushGpuMarker(const char *name, const QColor &color)
{
  (void)color;
  P(ProfilerPrivate);
  FrameInfo *frame = p.m_frames[p.m_currFrame];
  frame->pushGpuMarker(name, color);
}

void Profiler::popGpuMarker()
{
  P(ProfilerPrivate);
  FrameInfo *frame = p.m_frames[p.m_currFrame];
  frame->popGpuMarker();
}

void Profiler::synchronizeFrame()
{
  P(ProfilerPrivate);
  if (p.m_currFrame >= p.m_frames.size())
  {
    p.m_frames.push_back(new FrameInfo(this));
  }
}

void Profiler::emitResults()
{
  P(ProfilerPrivate);
  ++p.m_currFrame;

  // Emit all valid results
  FrameInfo *frame;
  size_t frameIdx = 0;
  while (frameIdx < p.m_currFrame)
  {
    frame = p.m_frames[frameIdx];
    if (!frame->isResultAvailable())
    {
      break;
    }
    ++frameIdx;
    emit onFrameResult(frame->waitForResult());
    frame->clear();
    p.m_frames.push_back(frame);
  }

  // Move first N frames to the back
  p.m_currFrame -= frameIdx;
  p.m_frames.erase(p.m_frames.begin(), p.m_frames.begin() + frameIdx);
}

