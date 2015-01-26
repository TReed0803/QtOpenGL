#ifndef PROFILER_H
#define PROFILER_H

#include <QObject>

class FrameResult;
class QMoveEvent;

extern bool sg_debug;

struct ProfilerPrivate;
class Profiler : public QObject
{
  Q_OBJECT
public:
  explicit Profiler(QObject *parent = 0);
  ~Profiler();
  void pushGpuMarker(char const *name);
  void popGpuMarker();
  void synchronizeFrame();
  void emitResults();
  void resizeGL(int width, int height);
  void paintGL();
  void moveEvent(const QMoveEvent *ev);

  // Options
  void setBorder(int left, int right, int top, int bottom);
  void setOffset(float left, float right, float top, float bottom);

  static void setProfiler(Profiler *profiler);
  static Profiler* profiler();
signals:
  void onFrameResult(const FrameResult& result);
private:
  ProfilerPrivate *m_private;
};

#ifdef    GL_DEBUG
# define PROFILER_SYNC_FRAME() ::Profiler::profiler()->synchronizeFrame()
# define PROFILER_PUSH_GPU_MARKER(name) ::Profiler::profiler()->pushGpuMarker(name)
# define PROFILER_POP_GPU_MARKER() ::Profiler::profiler()->popGpuMarker()
# define PROFILER_EMIT_RESULTS() ::Profiler::profiler()->emitResults()
# define PROFILER_MOVE_EVENT(ev) ::Profiler::profiler()->moveEvent(ev);
# define PROFILER_RESIZE_GL(width,height) ::Profiler::profiler()->resizeGL(width,height)
# define PROFILER_PAINT_GL() ::Profiler::profiler()->paintGL()
#else
# define PROFILER_SYNC_FRAME()
# define PROFILER_PUSH_GPU_MARKER(name)
# define PROFILER_POP_GPU_MARKER()
# define PROFILER_EMIT_RESULTS()
# define PROFILER_MOVE_EVENT(ev)
# define PROFILER_RESIZE_GL(width,height)
# define PROFILER_PAINT_GL()
#endif // GL_DEBUG

#endif // PROFILER_H
