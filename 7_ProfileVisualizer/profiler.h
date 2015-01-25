#ifndef PROFILER_H
#define PROFILER_H

#include <QObject>

class FrameResult;
class QMoveEvent;

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
signals:
  void onFrameResult(const FrameResult& result);
private:
  ProfilerPrivate *m_private;
};

// Note: Must define GL_PROFILER for macros to work.
#ifdef    GL_DEBUG
# define PROFILER_SYNC_FRAME() GL_PROFILER->synchronizeFrame()
# define PROFILER_PUSH_GPU_MARKER(name) GL_PROFILER->pushGpuMarker(name)
# define PROFILER_POP_GPU_MARKER() GL_PROFILER->popGpuMarker()
# define PROFILER_EMIT_RESULTS() GL_PROFILER->emitResults()
# define PROFILER_MOVE_EVENT(ev) GL_PROFILER->moveEvent(ev);
# define PROFILER_RESIZE_GL(width,height) GL_PROFILER->resizeGL(width,height)
# define PROFILER_PAINT_GL() GL_PROFILER->paintGL()
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
