#ifndef OPENGLERROREVENT
#define OPENGLERROREVENT

#include <QEvent>
#include <QGuiApplication>

class OpenGLErrorEvent : public QEvent
{
public:
  OpenGLErrorEvent(char const *name) : QEvent(OpenGLErrorEvent::type()), m_name(name) {}
  virtual ~OpenGLErrorEvent() {}
  char const *functionName() const { return m_name; }
  static QEvent::Type type()
  {
    static QEvent::Type customEventType =
      static_cast<QEvent::Type>(QEvent::registerEventType());
    return customEventType;
  }
  static bool sendEvent(OpenGLErrorEvent *event)
  {
    return QGuiApplication::sendEvent(QGuiApplication::instance(), event);
  }
private:
  char const *m_name;
};


#endif // OPENGLERROREVENT

