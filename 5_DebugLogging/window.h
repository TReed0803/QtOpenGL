#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLDebugMessage>
#include <QMatrix4x4>
#include "transform3d.h"
#include "camera3d.h"

#include <OpenGLBuffer>
#include <OpenGLVertexArrayObject>

class OpenGLError;
class OpenGLShaderProgram;
class QOpenGLDebugLogger;

class Window : public QOpenGLWindow,
               protected QOpenGLFunctions
{
  Q_OBJECT

// OpenGL Events
public:
  Window();
  ~Window();
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
protected slots:
  void teardownGL();
  void update();
  void messageLogged(const QOpenGLDebugMessage &msg);

protected:
  bool event(QEvent *event);
  void errorEventGL(OpenGLError *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  // OpenGL State Information
  OpenGLBuffer m_vertex;
  OpenGLVertexArrayObject m_object;
  OpenGLShaderProgram *m_program;
  QOpenGLDebugLogger *m_debugLogger;

  // Shader Information
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;
  QMatrix4x4 m_projection;
  Camera3D m_camera;
  Transform3D m_transform;

  // Private Helpers
  void printVersionInformation();
};

#endif // WINDOW_H
