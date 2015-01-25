#include "profiler.h"
#include <vector>
#include <stack>
#include <QOpenGLTimerQuery>
#include <QDebug>
#include "frameresult.h"

#define INVALID_TIMER -1
#define INVALID_TIME ((uint64_t)-1)

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/

struct Marker
{
  int depth;
  QString name;
  QColor color;
};

enum Status
{
  Inactive,
  Pending,
  Available
};

struct GpuMarker : public Marker
{

  GpuMarker(QObject *obj = 0) : m_startTimer(obj), m_endTimer(obj), m_currStatus(Inactive)
  {
    m_startTimer.create();
    m_endTimer.create();
  }

  ~GpuMarker()
  {
    m_startTimer.destroy();
    m_endTimer.destroy();
  }

  Status status()
  {
    if (m_currStatus == Pending)
    {
      if (m_startTimer.isResultAvailable() && m_endTimer.isResultAvailable())
      {
        m_currStatus = Available;
      }
    }
    return m_currStatus;
  }

  inline GLuint64 startTime() const
  {
    return m_startTimer.waitForResult();
  }

  inline GLuint64 endTime() const
  {
    return m_endTimer.waitForResult();
  }

  inline void start()
  {
    m_startTimer.recordTimestamp();
  }

  inline void stop()
  {
    m_endTimer.recordTimestamp();
  }

private:
  Status m_currStatus;
  QOpenGLTimerQuery m_startTimer;
  QOpenGLTimerQuery m_endTimer;
};

template <typename T>
struct MarkerGroup
{
  typedef T* MarkerPointer;
  typedef std::vector<MarkerPointer> MarkerContainer;
  typedef std::stack<size_t> IndexContainer;

  MarkerGroup(QObject *parent = 0) : m_currMarker(0), m_statusMarker(0), m_parent(parent) {}

  Status status()
  {
    while(m_statusMarker < m_currMarker)
    {
      if (m_markers[m_statusMarker]->status() == Pending)
      {
        return Pending;
      }
      ++m_statusMarker;
    }
    return Available;
  }

  inline const MarkerContainer &markers() const
  {
    return m_markers;
  }

  void pushMarker(const QString &name, const QColor &color)
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

  void popMarker()
  {
    size_t topIdx = m_activeMarkerIndex.top();
    m_activeMarkerIndex.pop();
    MarkerPointer marker = m_markers[topIdx];
    marker->stop();
  }

  void clear()
  {
    m_currMarker = m_statusMarker = 0;
  }

private:
  MarkerContainer m_markers;
  IndexContainer m_activeMarkerIndex;
  size_t m_currMarker;
  size_t m_statusMarker;
  QObject *m_parent;
};

struct FrameInfo
{
  typedef MarkerGroup<GpuMarker> GpuGroup;
  GpuGroup m_gpuMarkers;

  FrameInfo(QObject *parent = 0) : m_parent(parent) {}

  bool isResultAvailable()
  {
    return (m_gpuMarkers.status() == Available);
  }

  FrameResult waitForResult()
  {
    // Poll until available
    while (m_gpuMarkers.status() != Available);

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

  void pushGpuMarker(const QString &name, const QColor &color)
  {
    m_gpuMarkers.pushMarker(name, color);
  }

  void popGpuMarker()
  {
    m_gpuMarkers.popMarker();
  }

  void clear()
  {
    m_gpuMarkers.clear();
  }

private:
  QObject *m_parent;
};

struct ProfilerPrivate
{
  typedef std::vector<FrameInfo*> FrameContainer;

  size_t m_currFrame;
  FrameContainer m_frames;

  ProfilerPrivate() : m_currFrame(0) {}

  ~ProfilerPrivate()
  {
    for (size_t i = 0; i < m_frames.size(); ++i)
    {
      delete m_frames[i];
    }
  }
};

#define P(t) t &p = *m_private
Profiler::Profiler(QObject *parent) :
  QObject(parent), m_private(new ProfilerPrivate)
{
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

void Profiler::draw()
{
  P(ProfilerPrivate);
  FrameInfo *frame = p.m_frames[0];
  ++p.m_currFrame;
  if (frame->isResultAvailable())
  {
    size_t frameIdx = 1;
    qDebug() << frame->waitForResult();
    frame->clear();
    while (frameIdx < p.m_currFrame)
    {
      p.m_frames[frameIdx - 1] = p.m_frames[frameIdx];
      ++frameIdx;
    }
    p.m_frames[frameIdx - 1] = frame;
    --p.m_currFrame;
  }
  else
  {
    qDebug() << "Frame Count: " << p.m_currFrame;
  }
}

