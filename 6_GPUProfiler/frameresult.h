#ifndef FRAMERESULT_H
#define FRAMERESULT_H

#include <QString>
#include <QVector>
#include <QDebug>
#include <QtOpenGL/QGL>

class FrameResult
{
public:

  struct GpuMarkerResult
  {
    QString name;
    GLuint64 start;
    GLuint64 end;
  };
  QVector<GpuMarkerResult> m_gpuResults;

};

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const FrameResult::GpuMarkerResult &results);
QDebug operator<<(QDebug dbg, const FrameResult &results);
#endif

#endif // FRAMERESULT_H
