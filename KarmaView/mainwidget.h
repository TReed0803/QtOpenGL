#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QtOpenGL/qgl.h>
#include <OpenGLWidget>

class OpenGLScene;
class KUpdateEvent;

class MainWidgetPrivate;
class MainWidget : public OpenGLWidget
{
  Q_OBJECT

public:
  MainWidget(QWidget *parent = 0);
  ~MainWidget();
  void setScene(OpenGLScene *scene);

protected:

  // OpenGL Methods
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  // Events
  void updateEvent(KUpdateEvent *event);

private:
  MainWidgetPrivate *m_private;
};

#endif // MAINWIDGET_H
