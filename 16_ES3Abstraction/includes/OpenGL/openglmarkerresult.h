#ifndef OPENGLMARKERRESULT_H
#define OPENGLMARKERRESULT_H

#include <vector>
#include <QString>
class QDebug;

class OpenGLMarkerResult
{
public:
  // Constructors / Destructor
  inline OpenGLMarkerResult();
  inline OpenGLMarkerResult(OpenGLMarkerResult const &rhs);
  inline OpenGLMarkerResult(OpenGLMarkerResult &&rhs);

  // Query Information
  inline QString const &name() const;
  inline void setName(QString const &name);
  inline int depth() const;
  inline void setDepth(int depth);
  inline quint64 startTime() const;
  inline void setStartTime(quint64 startTime);
  inline quint64 endTime() const;
  inline void setEndTime(quint64 endTime);
  inline float elapsedSeconds() const;
  inline float elapsedMilliseconds() const;
  inline float elapsedNanoseconds() const;

  // Operators
  inline void operator=(OpenGLMarkerResult const &rhs);
  inline void operator=(OpenGLMarkerResult &&rhs);

private:
  int m_depth;
  QString m_name;
  quint64 m_startTime;
  quint64 m_endTime;
};

// Constructors / Destructor
inline OpenGLMarkerResult::OpenGLMarkerResult() {}
inline OpenGLMarkerResult::OpenGLMarkerResult(OpenGLMarkerResult const &rhs) : m_depth(rhs.m_depth), m_name(rhs.m_name), m_startTime(rhs.m_startTime), m_endTime(rhs.m_endTime) {}
inline OpenGLMarkerResult::OpenGLMarkerResult(OpenGLMarkerResult &&rhs) : m_depth(rhs.m_depth), m_name(std::move(rhs.m_name)), m_startTime(rhs.m_startTime), m_endTime(rhs.m_endTime) {}

// Query Information
inline QString const &OpenGLMarkerResult::name() const { return m_name; }
inline void OpenGLMarkerResult::setName(QString const &name) { m_name = name; }
inline int OpenGLMarkerResult::depth() const { return m_depth; }
inline void OpenGLMarkerResult::setDepth(int depth) { m_depth = depth; }
inline quint64 OpenGLMarkerResult::startTime() const { return m_startTime; }
inline void OpenGLMarkerResult::setStartTime(quint64 startTime) { m_startTime = startTime; }
inline quint64 OpenGLMarkerResult::endTime() const { return m_endTime; }
inline void OpenGLMarkerResult::setEndTime(quint64 endTime) { m_endTime = endTime; }
inline float OpenGLMarkerResult::elapsedSeconds() const { return elapsedNanoseconds() / 1e9f; }
inline float OpenGLMarkerResult::elapsedMilliseconds() const { return elapsedNanoseconds() / 1e6f; }
inline float OpenGLMarkerResult::elapsedNanoseconds() const { return m_endTime - m_startTime; }

// Operators
inline void OpenGLMarkerResult::operator=(OpenGLMarkerResult const &rhs)
{
  m_depth = rhs.m_depth;
  m_name = rhs.m_name;
  m_startTime = rhs.m_startTime;
  m_endTime = rhs.m_endTime;
}

inline void OpenGLMarkerResult::operator=(OpenGLMarkerResult &&rhs)
{
  m_depth = rhs.m_depth;
  m_name = std::move(rhs.m_name);
  m_startTime = rhs.m_startTime;
  m_endTime = rhs.m_endTime;
}

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug &operator<<(QDebug &dbg, const OpenGLMarkerResult &results);
#endif

typedef std::vector<OpenGLMarkerResult> OpenGLMarkerResults;

#endif // OPENGLMARKERRESULT_H
