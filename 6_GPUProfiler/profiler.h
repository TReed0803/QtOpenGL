#ifndef PROFILER_H
#define PROFILER_H

#include <QObject>

class FrameResult;

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
signals:
  void onFrameResult(const FrameResult& result);
private:
  ProfilerPrivate *m_private;
};

// Note: Must define GL_PROFILER for macros to work.
#ifdef    GL_DEBUG
# define PROFILER_SYNC_FRAME() GL_PROFILER->synchronizeFrame();
# define PROFILER_PUSH_GPU_MARKER(name) GL_PROFILER->pushGpuMarker(name);
# define PROFILER_POP_GPU_MARKER() GL_PROFILER->popGpuMarker();
# define PROFILER_EMIT_RESULTS() GL_PROFILER->emitResults();
#else
# define PROFILER_SYNC_FRAME()
# define PROFILER_PUSH_GPU_MARKER(name)
# define PROFILER_POP_GPU_MARKER()
# define PROFILER_EMIT_RESULTS()
#endif // GL_DEBUG

#endif // PROFILER_H
