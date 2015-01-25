#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

class QObject;
class QColor;
class QRectF;
class DebugVertex;

struct DebugDraw
{
  static const int MaxDebugVertex = 1024;
  static void initialize(QObject *parent = 0);
  struct Screen
  {
    static void drawRect(const QRectF &rect, const QColor &color);
  };
  static void draw();
};

#endif // DEBUGDRAW_H
