#ifndef PROFILER_H
#define PROFILER_H

#include <QColor>
#include <QObject>

class FrameResult;

struct ProfilerPrivate;
class Profiler : public QObject
{
  Q_OBJECT
public:
  explicit Profiler(QObject *parent = 0);
  ~Profiler();
  void pushGpuMarker(char const *name, QColor const &color);
  void popGpuMarker();
  void synchronizeFrame();
  void draw();
signals:
  void onFrameResult(const FrameResult& result);
private:
  ProfilerPrivate *m_private;
};

// Note: Must define GL_PROFILER for macros to work.
#ifdef    GL_DEBUG
# define PROFILER_SYNC_FRAME() GL_PROFILER->synchronizeFrame();
# define PROFILER_PUSH_CPU_MARKER(name,color) GL_PROFILER->pushCpuMarker(name, color);
# define PROFILER_POP_CPU_MARKER() GL_PROFILER->popCpuMarker();
# define PROFILER_PUSH_GPU_MARKER(name,color) GL_PROFILER->pushGpuMarker(name, color);
# define PROFILER_POP_GPU_MARKER() GL_PROFILER->popGpuMarker();
# define PROFILER_DRAW() GL_PROFILER->draw();
#else
# define PROFILER_SYNC_FRAME()
# define PROFILER_PUSH_CPU_MARKER(name,color)
# define PROFILER_POP_CPU_MARKER()
# define PROFILER_PUSH_GPU_MARKER(name,color)
# define PROFILER_POP_GPU_MARKER()
# define PROFILER_DRAW()
#endif // GL_DEBUG

#endif // PROFILER_H
