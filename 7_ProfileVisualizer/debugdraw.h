#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <QRectF>
#include <QColor>
#include <QOpenGLBuffer>
#include "fwdopengl.h"

class DebugVertex;

class DebugDraw
{
public:
  static const int MaxDebugVertex = 1024;
  static void initialize(QObject *parent = 0);
  static void drawRect(const QRectF &rect, const QColor &color);
  static void drawArray(const DebugVertex *array, size_t size);
  static void draw();
};

#endif // DEBUGDRAW_H
