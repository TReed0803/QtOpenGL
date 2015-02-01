#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <OpenGLFunctions>
#include <QOpenGLWindow>

class QEvent;
class OpenGLError;
class OpenGLFrameResults;
class QOpenGLDebugMessage;
class QKeyEvent;
class QMouseEvent;
class QMoveEvent;
class QWheelEvent;
class QTouchEvent;
class KUpdateEvent;

class OpenGLWindowPrivate;
class OpenGLWindow : public QOpenGLWindow,
                     protected OpenGLFunctions
{
  Q_OBJECT

public:
  // Constructors / Destructors
  OpenGLWindow(UpdateBehavior updateBehavior = NoPartialUpdate, QWindow *parent = 0);
  ~OpenGLWindow();

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
  virtual void updateEvent(KUpdateEvent *event);

public slots:
  void update();
  void frameTimeout(float fps);
  void messageLogged(const QOpenGLDebugMessage &msg);
  void frameResultAvailable(const OpenGLFrameResults &result);

private:
  OpenGLWindowPrivate *m_private;
};

#endif // OPENGLWINDOW_H
