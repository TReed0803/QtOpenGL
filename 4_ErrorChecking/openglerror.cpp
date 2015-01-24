#include "openglerror.h"
#include <QObject>
#include <QCoreApplication>

/*******************************************************************************
 * OpenGLError static types
 ******************************************************************************/
QObject* OpenGLError::m_errorHandler = NULL;

/*******************************************************************************
 * OpenGLError methods
 ******************************************************************************/
QEvent::Type OpenGLError::type()
{
  static QEvent::Type customEventType =
    static_cast<QEvent::Type>(QEvent::registerEventType());
  return customEventType;
}

bool OpenGLError::sendEvent(OpenGLError *event)
{
  if (!m_errorHandler)
  {
    qWarning("Set OpenGLError::setErrorHandler() before calling any GL_DEBUG OpenGL functions!");
    return false;
  }
  return QCoreApplication::sendEvent(m_errorHandler, event);
}

void OpenGLError::setErrorHandler(QObject *obj)
{
  m_errorHandler = obj;
}
