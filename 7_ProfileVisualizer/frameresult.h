#ifndef FRAMERESULT_H
#define FRAMERESULT_H

#include <QString>
#include <QVector>
#include <QDebug>

class FrameResult
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

  FrameResult();
  FrameResult(FrameResult &&rhs);
  FrameResult(size_t maxDepth, quint64 startTime, quint64 endTime);
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

inline const FrameResult::MarkerResultContainer &FrameResult::gpuResults() const { return m_gpuResults; }
inline size_t FrameResult::maxDepth() const { return m_maxDepth; }
inline quint64 FrameResult::startTime() const { return m_startTime; }
inline quint64 FrameResult::endTime() const { return m_endTime; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const FrameResult::MarkerResult &results);
QDebug operator<<(QDebug dbg, const FrameResult &results);
#endif

#endif // FRAMERESULT_H
