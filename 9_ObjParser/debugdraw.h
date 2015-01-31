#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

class QObject;
class QColor;
class QRectF;
class DebugVertex;
class QVector3D;

struct DebugDraw
{
  static const int MaxDebugVertex = 16384;
  static void initialize(QObject *parent = 0);
  struct Screen
  {
    static void drawRect(const QRectF &rect, const QColor &color);
  };
  struct World
  {
    static void drawLine(const QVector3D &from, const QVector3D &to, const QColor &color);
  };
  static void draw();
};

#endif // DEBUGDRAW_H
