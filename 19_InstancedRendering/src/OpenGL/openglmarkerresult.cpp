#include "openglmarkerresult.h"
#include <QDebug>

QDebug &operator<<(QDebug &dbg, const OpenGLMarkerResult &result)
{
  return dbg << qPrintable(result.name()) << ": " << result.elapsedMilliseconds() << "\n";
}
