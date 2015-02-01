#include "openglframeresults.h"

OpenGLFrameResults::OpenGLFrameResults()
{
  // Intentionally Empty
}

OpenGLFrameResults::OpenGLFrameResults(OpenGLFrameResults &&rhs) :
  m_maxDepth(rhs.m_maxDepth), m_startTime(rhs.m_startTime), m_endTime(rhs.m_endTime),
  m_gpuResults(std::move(rhs.m_gpuResults))
{
  // Intentionally Empty
}

OpenGLFrameResults::OpenGLFrameResults(size_t maxDepth, quint64 startTime, quint64 endTime) :
  m_maxDepth(maxDepth), m_startTime(startTime), m_endTime(endTime)
{
  // Intentionally Empty
}

void OpenGLFrameResults::addGpuResult(const QString &name, size_t depth, quint64 startTime, quint64 endTime)
{
  MarkerResult res;
  res.name = name;
  res.depth = depth;
  res.start = startTime;
  res.end = endTime;
  m_gpuResults.push_back(res);
}

void OpenGLFrameResults::operator=(OpenGLFrameResults &&rhs)
{
  m_maxDepth = rhs.m_maxDepth;
  m_startTime = rhs.m_startTime;
  m_endTime = rhs.m_endTime;
  m_gpuResults = std::move(rhs.m_gpuResults);
}

QDebug operator<<(QDebug dbg, const OpenGLFrameResults::MarkerResult &result)
{
  return dbg << qPrintable(result.name) << ": " << float(result.end - result.start) / 1e06 << "\n";
}

QDebug operator<<(QDebug dbg, const OpenGLFrameResults &results)
{
  foreach (OpenGLFrameResults::MarkerResult const& result, results.gpuResults())
  {
    dbg << result;
  }
  return dbg;
}
