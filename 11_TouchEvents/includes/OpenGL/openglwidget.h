#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <OpenGLFunctions>
#include <QOpenGLWidget>

class KUpdateEvent;
class OpenGLError;
class OpenGLFrameResults;
class QEvent;
class QGestureEvent;
class QKeyEvent;
class QMouseEvent;
class QMoveEvent;
class QOpenGLDebugMessage;
class QTouchEvent;
class QWheelEvent;

class OpenGLWidgetPrivate;
class OpenGLWidget : public QOpenGLWidget,
                     protected OpenGLFunctions
{
  Q_OBJECT

public:
  // Constructors / Destructors
  OpenGLWidget(QWidget *parent = 0);
  ~OpenGLWidget();

  // Public Methods
  void printVersionInformation();

protected:

  // OpenGL Protected Methods
  virtual void initializeGL();
  virtual void resizeGL(int width, int height);
  virtual void paintGL();
  virtual void teardownGL();

  // Event Methods
  virtual bool event(QEvent *event);
  virtual void errorEventGL(OpenGLError *event);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void moveEvent(QMoveEvent *event);
  virtual void touchEvent(QTouchEvent *event);
  virtual void gestureEvent(QGestureEvent *event);
  virtual void updateEvent(KUpdateEvent *event);

public slots:
  void update();
  void frameTimeout(float fps);
  void messageLogged(const QOpenGLDebugMessage &msg);
  void frameResultAvailable(const OpenGLFrameResults &result);

private:
  OpenGLWidgetPrivate *m_private;
};

#endif // OPENGLWINDOW_H
