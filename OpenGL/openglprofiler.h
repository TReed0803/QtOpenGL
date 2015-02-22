#ifndef OPENGLPROFILER_H
#define OPENGLPROFILER_H OpenGLProfiler

#include <QObject>
class OpenGLFrameResults;

class OpenGLProfilerPrivate;
class OpenGLProfiler : public QObject
{
  Q_OBJECT
public:
  // Constructors / Destructors
  explicit OpenGLProfiler(QObject *parent = 0);
  ~OpenGLProfiler();

  // Profiler Actions
  bool initialize();
  void beginFrame();
  void pushGpuMarker(char const *name);
  void popGpuMarker();
  void endFrame();

  // Global Profiler Action
  inline static void BeginFrame();
  inline static void PushGpuMarker(char const *name);
  inline static void PopGpuMarker();
  inline static void EndFrame();

  // Global Settings
  static OpenGLProfiler* profiler();
  static void setProfiler(OpenGLProfiler *profiler);

signals:
  void frameResultsAvailable(const OpenGLFrameResults& result);

private:
  OpenGLProfilerPrivate *m_private;
};

#if !defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)
inline void OpenGLProfiler::BeginFrame() { profiler()->beginFrame(); }
inline void OpenGLProfiler::PushGpuMarker(char const *name) { profiler()->pushGpuMarker(name); }
inline void OpenGLProfiler::PopGpuMarker() { profiler()->popGpuMarker(); }
inline void OpenGLProfiler::EndFrame() { profiler()->endFrame(); }
#else
inline void OpenGLProfiler::BeginFrame() { }
inline void OpenGLProfiler::PushGpuMarker(char const *name) { (void)name; }
inline void OpenGLProfiler::PopGpuMarker() { }
inline void OpenGLProfiler::EndFrame() { }
#endif

#endif // OPENGLPROFILER_H
