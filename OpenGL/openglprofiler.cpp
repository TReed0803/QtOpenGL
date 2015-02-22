#include "openglprofiler.h"
#include "openglframeresults.h"
#include <stack>
#include <vector>
#include <QOpenGLContext>
#include <QOpenGLTimerQuery>
#include <KMacros>

#if !defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)

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
  // Constructors / Destructor
  GpuMarker(QObject *obj = 0);

  // Marker manipulation
  inline void start();
  inline void stop();

  // Query information
  inline bool isValid() const;
  inline bool isResultAvailable() const;
  inline GLuint64 startTime() const;
  inline GLuint64 endTime() const;

private:
  bool m_valid;
  QOpenGLTimerQuery m_startTimer;
  QOpenGLTimerQuery m_endTimer;
};

GpuMarker::GpuMarker(QObject *obj) :
  m_valid(false), m_startTimer(obj), m_endTimer(obj)
{
  if (!m_startTimer.create()) return;
  if (!m_endTimer.create()) return;
  m_valid = true;
}

inline void GpuMarker::start()
{
  m_startTimer.recordTimestamp();
}

inline void GpuMarker::stop()
{
  m_endTimer.recordTimestamp();
}

bool GpuMarker::isValid() const
{
  return m_valid;
}

inline bool GpuMarker::isResultAvailable() const
{
  return m_startTimer.isResultAvailable() &&
         m_endTimer.isResultAvailable();
}

inline GLuint64 GpuMarker::startTime() const
{
  return m_startTimer.waitForResult();
}

inline GLuint64 GpuMarker::endTime() const
{
  return m_endTimer.waitForResult();
}

/*******************************************************************************
 * Marker Groups
 ******************************************************************************/

#define INVALID_MARKER_INDEX ((size_t)-1)
template <typename T>
struct MarkerGroup
{
  // Typedefs
  typedef T MarkerType;
  typedef std::vector<MarkerType*> MarkerContainer;
  typedef std::stack<size_t> IndexContainer;

  // Constructors / Destructor
  MarkerGroup(QObject *parent = 0);

  // Manipulate Markers
  void pushMarker(const QString &name);
  void popMarker();
  inline void clear();

  // Query information
  bool isResultAvailable() const;
  inline size_t maxDepth() const;
  inline const MarkerContainer &markers() const;

private:
  QObject *m_parent;
  MarkerContainer m_markers; //< Actual marker container
  IndexContainer m_activeMarkerIndex; //< Markers which have been started, but not stopped.
  size_t m_currMarker; //< Our current index through allocated markers.
  size_t m_currDepth; //< The current depth of push/pop of markers.
  size_t m_maxDepth; //< The maximum known depth of push/pop of markers.
  mutable size_t m_statusMarker; //< Caches last failed isResultAvailable index.
};

template <typename T>
MarkerGroup<T>::MarkerGroup(QObject *parent) :
  m_parent(parent), m_currMarker(0), m_statusMarker(0),
  m_currDepth(0), m_maxDepth(0)
{
  // Intentionally Blank
}

template <typename T>
void MarkerGroup<T>::pushMarker(const QString &name)
{
  // See if we need to grow our marker container
  if (m_currMarker >= m_markers.size())
  {
    m_markers.push_back(new MarkerType(m_parent));
  }

  // Early-out to see if the marker is valid
  // If the marker is invalid, we cannot continue.
  MarkerType *currMarker = m_markers[m_currMarker];
  if (!currMarker->isValid())
  {
    m_activeMarkerIndex.push(INVALID_MARKER_INDEX);
  }

  // Instantiate the marker
  currMarker->name = name;
  currMarker->depth = m_currDepth;
  currMarker->start();

  // Alter MarkerGroup meta information
  m_activeMarkerIndex.push(m_currMarker);
  ++m_currDepth;
  ++m_currMarker;
  m_maxDepth = std::max(m_maxDepth, m_currDepth);
}

template <typename T>
void MarkerGroup<T>::popMarker()
{
  size_t topIdx = m_activeMarkerIndex.top();
  m_activeMarkerIndex.pop();

  // Early-out if the last pushed marker was invalid
  if (topIdx == INVALID_MARKER_INDEX) return;

  // Finalize the marker
  m_markers[topIdx]->stop();
  --m_currDepth;
}

template <typename T>
inline void MarkerGroup<T>::clear()
{
  m_currMarker = m_statusMarker = m_currDepth = m_maxDepth = 0;
}

template <typename T>
bool MarkerGroup<T>::isResultAvailable() const
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
inline size_t MarkerGroup<T>::maxDepth() const
{
  return m_maxDepth;
}

template <typename T>
inline const typename MarkerGroup<T>::MarkerContainer &MarkerGroup<T>::markers() const
{
  return m_markers;
}

/*******************************************************************************
 * Frame Info
 ******************************************************************************/

struct FrameInfo
{
  // Typedefs
  typedef MarkerGroup<GpuMarker> GpuGroup;

  // Constructors / Destructor
  FrameInfo(QObject *parent = 0);

  // Frame manipulation
  inline void startFrame();
  inline void pushGpuMarker(const QString &name);
  inline void popGpuMarker();
  inline void endFrame();
  inline void clear();

  // Aggregate results
  OpenGLFrameResults waitForResult();

  // Query information
  inline bool isValid() const;
  inline bool isResultAvailable() const;

private:
  bool m_valid;
  QObject *m_parent;
  GpuGroup m_gpuMarkers;
  QOpenGLTimerQuery m_startTimer;
  QOpenGLTimerQuery m_endTimer;
};

FrameInfo::FrameInfo(QObject *parent) :
  m_valid(false), m_parent(parent), m_startTimer(parent), m_endTimer(parent)
{
  if (!m_startTimer.create()) return;
  if (!m_endTimer.create()) return;
  m_valid = true;
}

inline void FrameInfo::startFrame()
{
  m_startTimer.recordTimestamp();
}

inline void FrameInfo::pushGpuMarker(const QString &name)
{
  m_gpuMarkers.pushMarker(name);
}

inline void FrameInfo::popGpuMarker()
{
  m_gpuMarkers.popMarker();
}

inline void FrameInfo::endFrame()
{
  m_endTimer.recordTimestamp();
}

inline void FrameInfo::clear()
{
  m_gpuMarkers.clear();
}

OpenGLFrameResults FrameInfo::waitForResult()
{
  // Poll until available
  while (!isResultAvailable());

  // Grab useful information for the results
  quint64 startTime = m_startTimer.waitForResult();
  quint64 endTime = m_endTimer.waitForResult();
  size_t maxDepth = m_gpuMarkers.maxDepth();
  OpenGLFrameResults results(maxDepth, startTime, endTime);

  // Aggregate frame information
  const GpuGroup::MarkerContainer &gpuMarkers = m_gpuMarkers.markers();
  for (GpuMarker const* marker : gpuMarkers)
  {
    // If the current marker is invalid,
    // subsequent markers also will be.
    if (!marker->isValid()) break;

    results.addGpuResult(
      marker->name,
      marker->depth,
      marker->startTime(),
      marker->endTime()
    );
  }

  return std::move(results);
}

bool FrameInfo::isValid() const
{
  return m_valid;
}

inline bool FrameInfo::isResultAvailable() const
{
  return m_startTimer.isResultAvailable() &&
         m_endTimer.isResultAvailable() &&
         m_gpuMarkers.isResultAvailable();
}

/*******************************************************************************
 * ProfilerPrivate
 ******************************************************************************/

class OpenGLProfilerPrivate
{
public:
  // Type Definitions
  typedef std::vector<FrameInfo*> FrameContainer;

  // Constructors / Destructor
  OpenGLProfilerPrivate();
  ~OpenGLProfilerPrivate();

  // Member Information
  bool m_valid;
  bool m_started;
  size_t m_currFrame;
  FrameContainer m_frames;

  // Static Information
  static OpenGLProfiler *CurrentProfiler;
};

// Start by pointing to a dummy profiler
OpenGLProfiler *OpenGLProfilerPrivate::CurrentProfiler = new OpenGLProfiler(Q_NULLPTR);

OpenGLProfilerPrivate::OpenGLProfilerPrivate() :
  m_valid(false), m_started(false), m_currFrame(0)
{
  // Intentionally Empty
}

OpenGLProfilerPrivate::~OpenGLProfilerPrivate()
{
  for (size_t i = 0; i < m_frames.size(); ++i)
  {
    delete m_frames[i];
  }
}

/*******************************************************************************
 * Profiler
 ******************************************************************************/
OpenGLProfiler::OpenGLProfiler(QObject *parent) :
  QObject(parent), m_private(new OpenGLProfilerPrivate)
{
  // Intentionally Empty
}

OpenGLProfiler::~OpenGLProfiler()
{
  delete m_private;
}

bool OpenGLProfiler::initialize()
{
  P(OpenGLProfilerPrivate);
  QOpenGLContext *ctx = QOpenGLContext::currentContext();

  // Note: OpenGL ES does not support timer queries
  if (ctx->isOpenGLES())
    return false;

  // Attempt to create the first frame
  // This can fail if OpenGL Desktop without Timer Query support.
  FrameInfo *frame = new FrameInfo(this);
  if (!frame->isValid())
  {
    delete frame;
    return false;
  }

  // Profiler is valid
  p.m_valid = true;
  p.m_frames.push_back(frame);
  OpenGLProfilerPrivate::CurrentProfiler = this;

  return true;
}

void OpenGLProfiler::beginFrame()
{
  P(OpenGLProfilerPrivate);

  // Early-out if Profiler doesn't support Timers
  if (!p.m_valid) return;

  // Even though we recycle frames, it's possible
  // that one frame may not be done processing.
  // So we must create new frames if needed.
  if (p.m_currFrame >= p.m_frames.size())
  {
    p.m_frames.push_back(new FrameInfo(this));
  }

  // Likewise, the frame may have run out of
  // timer queries. We must check if it is valid.
  FrameInfo *currFrame = p.m_frames[p.m_currFrame];
  if (currFrame->isValid())
  {
    currFrame->startFrame();
  }

  p.m_started = true;
}

void OpenGLProfiler::pushGpuMarker(const char *name)
{
  P(OpenGLProfilerPrivate);

  // Early-out if Profiler doesn't support Timers
  if (!p.m_valid) return;
  if (!p.m_started) return;

  // At this point, it's impossible for there not
  // to at least be one allocated frame. Since initialize()
  // both allocates a frame, and sets the Profiler to valid.
  FrameInfo *currFrame = p.m_frames[p.m_currFrame];

  // However, the frame can still be invalid.
  if (currFrame->isValid())
  {
    currFrame->pushGpuMarker(name);
  }
}

void OpenGLProfiler::popGpuMarker()
{
  P(OpenGLProfilerPrivate);

  // Early-out if Profiler doesn't support Timers
  if (!p.m_valid) return;
  if (!p.m_started) return;

  // At this point, it's impossible for there not
  // to at least be one allocated frame. Since initialize()
  // both allocates a frame, and sets the Profiler to valid.
  FrameInfo *currFrame = p.m_frames[p.m_currFrame];

  // However, the frame can still be invalid.
  if (currFrame->isValid())
  {
    currFrame->popGpuMarker();
  }
}

void OpenGLProfiler::endFrame()
{
  P(OpenGLProfilerPrivate);

  // Early-out if Profiler doesn't support Timers
  if (!p.m_valid) return;
  if (!p.m_started) return;

  // Mark the frame as completed
  p.m_frames[p.m_currFrame]->endFrame();
  ++p.m_currFrame;

  // Loop through all completed frames, emitting results
  size_t idx;
  FrameInfo *currFrame;
  OpenGLFrameResults currResults;
  for (idx = 0; idx < p.m_currFrame; ++idx)
  {
    currFrame = p.m_frames[idx];

    if (currFrame->isValid())
    {
      // If results of current frame are unavailable,
      // the results of the next frame surely won't be
      // available either. So we can break from our loop.
      if (!currFrame->isResultAvailable()) break;

      // Otherwise, we'll simply emit the results
      currResults = std::move(currFrame->waitForResult());
      emit frameResultsAvailable(currResults);
      currFrame->clear();
    }

    // Always push frames back, this moves the
    // pre-allocated information to the back for re-use.
    p.m_frames.push_back(currFrame);
  }

  // Remove the duplicate N first frames from
  // when we moved them to the back of the vector.
  p.m_currFrame -= idx;
  p.m_frames.erase(p.m_frames.begin(), p.m_frames.begin() + idx);
  p.m_started = false;
}

void OpenGLProfiler::setProfiler(OpenGLProfiler *profiler)
{
  OpenGLProfilerPrivate::CurrentProfiler = profiler;
}

OpenGLProfiler *OpenGLProfiler::profiler()
{
  return OpenGLProfilerPrivate::CurrentProfiler;
}

#else

bool OpenGLProfiler::initialize()
{
  return false;
}

OpenGLProfiler::OpenGLProfiler(QObject *parent)
{
  (void)parent;
}

OpenGLProfiler::~OpenGLProfiler()
{
  // Intentionally Empty
}

void OpenGLProfiler::beginFrame()
{
  // Intentionally Empty
}

void OpenGLProfiler::pushGpuMarker(const char *name)
{
  (void)name;
}

void OpenGLProfiler::popGpuMarker()
{
  // Intentionally Empty
}

void OpenGLProfiler::endFrame()
{
  // Intentionally Empty
}

void OpenGLProfiler::setProfiler(OpenGLProfiler *profiler)
{
  (void)profiler;
}

OpenGLProfiler *OpenGLProfiler::profiler()
{
  return Q_NULLPTR;
}

#endif
