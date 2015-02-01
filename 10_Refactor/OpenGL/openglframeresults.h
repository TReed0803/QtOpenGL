#ifndef OPENGLFRAMERESULTS_H
#define OPENGLFRAMERESULTS_H

#include <QString>
#include <QVector>
#include <QDebug>

class OpenGLFrameResults
{
public:

  struct MarkerResult
  {
    QString name;
    size_t depth;
    quint64 start;
    quint64 end;
  };
  typedef QVector<MarkerResult> MarkerResultContainer;

  OpenGLFrameResults();
  OpenGLFrameResults(OpenGLFrameResults &&rhs);
  void operator=(OpenGLFrameResults &&rhs);
  OpenGLFrameResults(size_t maxDepth, quint64 startTime, quint64 endTime);
  void addGpuResult(const QString &name, size_t depth, quint64 startTime, quint64 endTime);
  inline const MarkerResultContainer &gpuResults() const;
  inline size_t maxDepth() const;
  inline quint64 startTime() const;
  inline quint64 endTime() const;

private:
  size_t m_maxDepth;
  quint64 m_startTime, m_endTime;
  MarkerResultContainer m_gpuResults;
};

inline const OpenGLFrameResults::MarkerResultContainer &OpenGLFrameResults::gpuResults() const { return m_gpuResults; }
inline size_t OpenGLFrameResults::maxDepth() const { return m_maxDepth; }
inline quint64 OpenGLFrameResults::startTime() const { return m_startTime; }
inline quint64 OpenGLFrameResults::endTime() const { return m_endTime; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const OpenGLFrameResults::MarkerResult &results);
QDebug operator<<(QDebug dbg, const OpenGLFrameResults &results);
#endif

#endif // OPENGLFRAMERESULTS_H
