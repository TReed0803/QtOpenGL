#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "qtransform3d.h"

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow,
               protected QOpenGLFunctions
{
  Q_OBJECT

// OpenGL Events
public:
  Window();
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
protected slots:
  void teardownGL();
  void update();

private:
  // OpenGL State Information
  QOpenGLBuffer m_vertex;
  QOpenGLVertexArrayObject m_object;
  QOpenGLShaderProgram *m_program;

  // Shader Information
  int u_modelToWorld;
  int u_worldToView;
  QMatrix4x4 m_projection;
  QTransform3D m_transform;

  // Private Helpers
  void printVersionInformation();
};

#endif // WINDOW_H
