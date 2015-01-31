#include "frameresult.h"

FrameResult::FrameResult()
{
  // Intentionally Empty
}

FrameResult::FrameResult(FrameResult &&rhs) :
  m_maxDepth(rhs.m_maxDepth), m_startTime(rhs.m_startTime), m_endTime(rhs.m_endTime),
  m_gpuResults(std::move(rhs.m_gpuResults))
{
  // Intentionally Empty
}

FrameResult::FrameResult(size_t maxDepth, quint64 startTime, quint64 endTime) :
  m_maxDepth(maxDepth), m_startTime(startTime), m_endTime(endTime)
{
  // Intentionally Empty
}

void FrameResult::addGpuResult(const QString &name, size_t depth, quint64 startTime, quint64 endTime)
{
  MarkerResult res;
  res.name = name;
  res.depth = depth;
  res.start = startTime;
  res.end = endTime;
  m_gpuResults.push_back(res);
}

void FrameResult::operator=(FrameResult &&rhs)
{
  m_maxDepth = rhs.m_maxDepth;
  m_startTime = rhs.m_startTime;
  m_endTime = rhs.m_endTime;
  m_gpuResults = std::move(rhs.m_gpuResults);
}

QDebug operator<<(QDebug dbg, const FrameResult::MarkerResult &result)
{
  return dbg << qPrintable(result.name) << ": " << float(result.end - result.start) / 1e06 << "\n";
}

QDebug operator<<(QDebug dbg, const FrameResult &results)
{
  foreach (FrameResult::MarkerResult const& result, results.gpuResults())
  {
    dbg << result;
  }
  return dbg;
}
