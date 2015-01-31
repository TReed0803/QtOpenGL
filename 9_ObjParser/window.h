#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLDebugMessage>
#include <QMatrix4x4>
#include "transform3d.h"
#include "camera3d.h"

// Custom Includes
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include "openglmesh.h"
#include "halfedgemesh.h"

class Profiler;
class FrameResult;
class OpenGLError;
class OpenGLShaderProgram;
class QOpenGLDebugLogger;
class QMoveEvent;
class HalfEdgeMesh;

class Window : public QOpenGLWindow,
               protected OpenGLFunctions
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
  void onFrameResult(const FrameResult& result);

protected:
  bool event(QEvent *event);
  void errorEventGL(OpenGLError *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void moveEvent(QMoveEvent *ev);

private:
  // OpenGL State Information
  OpenGLShaderProgram *m_program;
  QOpenGLDebugLogger *m_debugLogger;
  Profiler *m_profiler;

  // Shader Information
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;
  QMatrix4x4 m_projection;
  Camera3D m_camera;
  Transform3D m_transform;

  // Object Information
  OpenGLMesh *m_mesh;
  HalfEdgeMesh *m_heMesh;
  std::vector<uint64_t> m_res;

  // Private Helpers
  void printVersionInformation();
};

#endif // WINDOW_H
