#include "profiler.h"
#include <vector>
#include <stack>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QToolTip>
#include <QOpenGLTimerQuery>
#include <QMoveEvent>
#include "debugdraw.h"
#include "frameresult.h"
#include "input.h"

struct Marker
{
  QString name;
  size_t depth;
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
  void pushMarker(const QString &name);
  void popMarker();
  inline void clear();
  inline size_t maxDepth() const;

private:
  MarkerContainer m_markers;
  IndexContainer m_activeMarkerIndex;
  size_t m_currMarker;
  size_t m_statusMarker;
  size_t m_currDepth;
  size_t m_maxDepth;
  QObject *m_parent;
};

template <typename T>
MarkerGroup<T>::MarkerGroup(QObject *parent = 0) :
  m_currMarker(0), m_statusMarker(0), m_currDepth(0), m_parent(parent)
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
void MarkerGroup<T>::pushMarker(const QString &name)
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
  marker->depth = m_currDepth;
  marker->start();

  m_activeMarkerIndex.push(m_currMarker);
  ++m_currMarker;
  ++m_currDepth;
  if (m_currDepth > m_maxDepth) m_maxDepth = m_currDepth;
}

template <typename T>
void MarkerGroup<T>::popMarker()
{
  size_t topIdx = m_activeMarkerIndex.top();
  m_activeMarkerIndex.pop();
  MarkerPointer marker = m_markers[topIdx];
  marker->stop();
  --m_currDepth;
}

template <typename T>
inline void MarkerGroup<T>::clear()
{
  m_currMarker = m_statusMarker = m_currDepth = m_maxDepth = 0;
}

template <typename T>
inline size_t MarkerGroup<T>::maxDepth() const
{
  return m_maxDepth;
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
  inline void pushGpuMarker(const QString &name);
  inline void popGpuMarker();
  inline void clear();
  inline void startFrame();
  inline void endFrame();

private:
  QObject *m_parent;
  QOpenGLTimerQuery m_startTimer;
  QOpenGLTimerQuery m_endTimer;
};

FrameInfo::FrameInfo(QObject *parent) :
  m_parent(parent), m_startTimer(parent), m_endTimer(parent)
{
  m_startTimer.create();
  m_endTimer.create();
}

inline bool FrameInfo::isResultAvailable()
{
  return m_startTimer.isResultAvailable() && m_endTimer.isResultAvailable() && m_gpuMarkers.isResultAvailable();
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
    markerResult.depth = marker.depth;
    markerResult.start = marker.startTime();
    markerResult.end = marker.endTime();
    results.m_gpuResults.push_back(markerResult);
  }
  results.m_maxDepth = m_gpuMarkers.maxDepth();
  results.m_startTime = m_startTimer.waitForResult();
  results.m_endTime = m_endTimer.waitForResult();

  return results;
}

inline void FrameInfo::pushGpuMarker(const QString &name)
{
  m_gpuMarkers.pushMarker(name);
}

inline void FrameInfo::popGpuMarker()
{
  m_gpuMarkers.popMarker();
}

inline void FrameInfo::clear()
{
  m_gpuMarkers.clear();
}

inline void FrameInfo::startFrame()
{
  m_startTimer.recordTimestamp();
}

inline void FrameInfo::endFrame()
{
  m_endTimer.recordTimestamp();
}

/*******************************************************************************
 * ProfilerPrivate
 ******************************************************************************/

struct ProfilerPrivate
{
  typedef std::vector<FrameInfo*> FrameContainer;
  size_t m_currFrame;
  QPoint m_windowPosition;
  QSize m_windowSize;
  QString m_currToolTip;
  FrameContainer m_frames;
  FrameResult m_lastResultSet;
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

void Profiler::pushGpuMarker(const char *name)
{
  P(ProfilerPrivate);
  FrameInfo *frame = p.m_frames[p.m_currFrame];
  frame->pushGpuMarker(name);
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
  p.m_frames[p.m_currFrame]->startFrame();
}

void Profiler::emitResults()
{
  P(ProfilerPrivate);

  p.m_frames[p.m_currFrame]->endFrame();
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
    p.m_lastResultSet = frame->waitForResult();
    emit onFrameResult(p.m_lastResultSet);
    p.m_frames.push_back(frame);
    frame->clear();
  }

  // Move first N frames to the back
  p.m_currFrame -= frameIdx;
  p.m_frames.erase(p.m_frames.begin(), p.m_frames.begin() + frameIdx);
}

void Profiler::resizeGL(int width, int height)
{
  P(ProfilerPrivate);
  p.m_windowSize.setWidth(width);
  p.m_windowSize.setHeight(height);
}

void Profiler::paintGL()
{
  P(ProfilerPrivate);

  // Find our bounds
  float xBorder = 0.015f;
  float yBorder = 0.015f;
  QPointF topLeft(0.0f + xBorder, 0.9f + yBorder);
  QPointF bottomRight(1.0f - xBorder, 1.0f - yBorder);
  QSizeF bounds(bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
  QRectF area(topLeft, bounds);

  DebugDraw::drawRect(area, Qt::white);

  // Find our step
  float ySteps = bounds.height() / p.m_lastResultSet.m_maxDepth;
  uint64_t begin = p.m_lastResultSet.m_startTime;
  uint64_t end = p.m_lastResultSet.m_endTime;
  float frameTime = end - begin;

  // Find mouse pos
  QPoint pos = Input::mousePosition() - p.m_windowPosition;
  QPointF posF(float(pos.x()) / p.m_windowSize.width(), float(pos.y()) / p.m_windowSize.height());

  const QVector<FrameResult::GpuMarkerResult> &gpuResults = p.m_lastResultSet.m_gpuResults;
  for (int i = 0; i < gpuResults.size(); ++i)
  {
    const FrameResult::GpuMarkerResult &result = gpuResults[i];
    float start = (result.start - begin) / frameTime;
    float end = (result.end - begin) / frameTime;
    area.setLeft(topLeft.x() + bounds.width() * start);
    area.setRight(topLeft.x() + bounds.width() * end);
    area.setTop(topLeft.y() + ySteps * result.depth);
    area.setBottom(topLeft.y() + ySteps * (result.depth + 1));
    if (area.contains(posF))
    {
      if (p.m_currToolTip != result.name)
      {
        p.m_currToolTip = result.name;
        QString str = result.name + " " + QString::number((result.end - result.start) / 1e6f);
        QToolTip::showText(Input::mousePosition(), str);
      }
      DebugDraw::drawRect(area, Qt::yellow);
    }
    else
    {
      if (p.m_currToolTip == result.name)
      {
        p.m_currToolTip = "";
        QToolTip::hideText();
      }
      DebugDraw::drawRect(area, Qt::red);
    }

  }
}

void Profiler::moveEvent(const QMoveEvent *ev)
{
  P(ProfilerPrivate);

  p.m_windowPosition = ev->pos();
}
