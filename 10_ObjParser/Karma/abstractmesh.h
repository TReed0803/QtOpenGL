#ifndef ABSTRACTMESH_H
#define ABSTRACTMESH_H

#include <QObject>
#include "openglmesh.h"

class AbstractMesh : public QObject
{
  Q_OBJECT
public:
  explicit AbstractMesh(QObject *parent = 0);
  virtual OpenGLMesh* createOpenGLMesh(OpenGLMesh::Options options) = 0;
signals:

public slots:
};


#endif // ABSTRACTMESH_H
