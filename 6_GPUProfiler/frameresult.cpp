#include "frameresult.h"

QDebug operator<<(QDebug dbg, const FrameResult::GpuMarkerResult &result)
{
  return dbg << qPrintable(result.name) << ": " << float(result.end - result.start) / 1e06 << "\n";
}

QDebug operator<<(QDebug dbg, const FrameResult &results)
{
  foreach (FrameResult::GpuMarkerResult const& result, results.m_gpuResults)
  {
    dbg << result;
  }
  return dbg;
}
