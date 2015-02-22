#ifndef KCOLOR_H
#define KCOLOR_H KColor

#include <QColor>

class KColor : public QColor
{
public:
  KColor() : QColor() {}
  KColor(QColor const& rhs) : QColor(rhs) {}
  KColor(Qt::GlobalColor color) : QColor(color) {}
  KColor(int r, int g, int b, int a = 255) : QColor(r,g,b,a) {}
  KColor(float r, float g, float b, float a = 1.0f) : QColor(r * 255, g * 255, b * 255, a * 255) {}
  KColor(QRgb rgb) : QColor(rgb) {}
  KColor(const QString& name) : QColor(name) {}
  KColor(const char *name) : QColor(name) {}
  KColor(Spec spec) : QColor(spec) {}
};

#endif // KCOLOR_H

