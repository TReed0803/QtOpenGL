#include "openglupdateevent.h"

/*******************************************************************************
 * OpenGLError methods
 ******************************************************************************/
QEvent::Type OpenGLUpdateEvent::eventType()
{
  static QEvent::Type customEventType =
    static_cast<QEvent::Type>(QEvent::registerEventType());
  return customEventType;
}
