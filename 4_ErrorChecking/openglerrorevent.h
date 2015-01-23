#ifndef OPENGLERROREVENT
#define OPENGLERROREVENT

#include <QEvent>

class OpenGLErrorEvent : public QEvent
{
public:
  OpenGLErrorEvent(char const *fnName);
  virtual ~OpenGLErrorEvent();
  char const *functionName() const;
  static QEvent::Type type();
  static bool sendEvent(OpenGLErrorEvent *event);
  static void setErrorHandler(QObject *obj);

private:
  char const *m_functionName;
  static QObject *m_errorHandler;
};

inline OpenGLErrorEvent::OpenGLErrorEvent(char const *fnName) : QEvent(type()), m_functionName(fnName) {}
inline OpenGLErrorEvent::~OpenGLErrorEvent() {}
inline char const* OpenGLErrorEvent::functionName() const { return m_functionName; }


#endif // OPENGLERROREVENT

