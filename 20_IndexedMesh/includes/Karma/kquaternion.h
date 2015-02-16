#ifndef KQUATERNION_H
#define KQUATERNION_H

#include <KVector3D>
#include <QQuaternion>

class KQuaternion : public QQuaternion
{
public:
  KQuaternion() : QQuaternion() {}
  KQuaternion(QQuaternion const& rhs) : QQuaternion(rhs) {}
  KQuaternion(float scalar, float xpos, float ypos, float zpos) : QQuaternion(scalar, xpos, ypos, zpos) {}
#ifndef QT_NO_VECTOR3D
  KQuaternion(float scalar, const KVector3D& vector) : QQuaternion(scalar, vector) {}
#endif
};

#endif // KQUATERNION_H

