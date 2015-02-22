#ifndef OPENGLFRAMERESULTS_H
#define OPENGLFRAMERESULTS_H OpenGLFrameResults

#include <OpenGLMarkerResult>
#include <QString>
#include <QVector>

class OpenGLFrameResults
{
public:
  // Constructors / Destructors
  OpenGLFrameResults();
  OpenGLFrameResults(OpenGLFrameResults &&rhs);
  OpenGLFrameResults(size_t maxDepth, quint64 startTime, quint64 endTime);

  // Public Methods
  void addGpuResult(const QString &name, size_t depth, quint64 startTime, quint64 endTime);

  // Operators
  void operator=(OpenGLFrameResults const &rhs);
  void operator=(OpenGLFrameResults &&rhs);

  // Query Methods
  inline size_t maxDepth() const;
  inline quint64 startTime() const;
  inline quint64 endTime() const;
  inline const OpenGLMarkerResults &gpuResults() const;

private:
  size_t m_maxDepth;
  quint64 m_startTime, m_endTime;
  OpenGLMarkerResults m_gpuResults;
};

inline size_t OpenGLFrameResults::maxDepth() const { return m_maxDepth; }
inline quint64 OpenGLFrameResults::startTime() const { return m_startTime; }
inline quint64 OpenGLFrameResults::endTime() const { return m_endTime; }
inline const OpenGLMarkerResults &OpenGLFrameResults::gpuResults() const { return m_gpuResults; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug &operator<<(QDebug &dbg, const OpenGLFrameResults &results);
#endif

#endif // OPENGLFRAMERESULTS_H
