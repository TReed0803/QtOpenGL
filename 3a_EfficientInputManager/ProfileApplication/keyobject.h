#ifndef KEYOBJECT_H
#define KEYOBJECT_H

#include <QObject>

class KeyObject {
 Q_GADGET
 Q_PROPERTY(Qt::Key KeyEnum READ KeyEnum)
};

#endif // KEYOBJECT_H
