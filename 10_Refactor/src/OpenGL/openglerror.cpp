#include "openglerror.h"
#include <QCoreApplication>
#include <QDebug>
#include <QObject>

/*******************************************************************************
 * OpenGLError static types
 ******************************************************************************/
QStack<QObject*> OpenGLError::m_errorHandler;

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
  if (m_errorHandler.empty())
  {
    qWarning("Set OpenGLError::setErrorHandler() before calling any GL_DEBUG OpenGL functions!");
    return false;
  }
  return QCoreApplication::sendEvent(m_errorHandler.top(), event);
}

void OpenGLError::pushErrorHandler(QObject *obj)
{
#ifdef    GL_DEBUG
  qDebug() << "GL_DEBUG Error Handler (" << obj << ")";
#endif // GL_DEBUG
  m_errorHandler.push(obj);
}

void OpenGLError::popErrorHandler()
{
  m_errorHandler.pop();
}
