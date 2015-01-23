#include "openglerrorevent.h"
#include <QObject>
#include <QCoreApplication>

QObject* OpenGLErrorEvent::m_errorHandler = NULL;

QEvent::Type OpenGLErrorEvent::type()
{
  static QEvent::Type customEventType =
    static_cast<QEvent::Type>(QEvent::registerEventType());
  return customEventType;
}

bool OpenGLErrorEvent::sendEvent(OpenGLErrorEvent *event)
{
  return QCoreApplication::sendEvent(m_errorHandler, event);
}

void OpenGLErrorEvent::setErrorHandler(QObject *obj)
{
  m_errorHandler = obj;
}
