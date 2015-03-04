#ifndef KABSTRACTMESH_H
#define KABSTRACTMESH_H KAbstractMesh

#include <QObject>
#include <OpenGLMesh>
#include <KAbstractObjParser>

class KAbstractMesh : public QObject
{
  Q_OBJECT
public:
  typedef KAbstractObjParser::index_type index_type;
  typedef KAbstractObjParser::index_pair index_pair;
  typedef KAbstractObjParser::index_array index_array;
  explicit KAbstractMesh(QObject *parent = 0);
signals:

public slots:
};

inline KAbstractMesh::KAbstractMesh(QObject *parent) :
  QObject(parent)
{
  // Intentionally Empty
}

#endif // KABSTRACTMESH_H
