#ifndef OPENGLPROFILERVISUALIZER_H
#define OPENGLPROFILERVISUALIZER_H OpenGLProfilerVisualizer

#include <QObject>

class QMoveEvent;
class OpenGLFrameResults;

class OpenGLProfilerVisualizerPrivate;
class OpenGLProfilerVisualizer : public QObject
{
  Q_OBJECT
public:

  // Constructors / Destructors
  explicit OpenGLProfilerVisualizer(QObject *parent = 0);
  ~OpenGLProfilerVisualizer();

  // Public Methods
  void setBorder(int left, int right, int top, int bottom);
  void setOffset(float left, float right, float top, float bottom);

  // OpenGL Methods
  void resizeGL(int width, int height);
  void paintGL();

  // Events
  void moveEvent(const QMoveEvent *ev);

public slots:
  void frameResultsAvailable(OpenGLFrameResults const &results);

private:
  OpenGLProfilerVisualizerPrivate *m_private;
};

#endif // OPENGLPROFILERVISUALIZER_H
