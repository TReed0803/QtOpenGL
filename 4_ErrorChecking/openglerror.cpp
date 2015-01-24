#include "openglerror.h"
#include <QObject>
#include <QCoreApplication>

QObject* OpenGLError::m_errorHandler = NULL;

QEvent::Type OpenGLError::type()
{
  static QEvent::Type customEventType =
    static_cast<QEvent::Type>(QEvent::registerEventType());
  return customEventType;
}

bool OpenGLError::sendEvent(OpenGLError *event)
{
  return QCoreApplication::sendEvent(m_errorHandler, event);
}

void OpenGLError::setErrorHandler(QObject *obj)
{
  m_errorHandler = obj;
}
