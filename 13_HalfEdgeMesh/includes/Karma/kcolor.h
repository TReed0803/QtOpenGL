#ifndef KCOLOR_H
#define KCOLOR_H

#include <QColor>

class KColor : public QColor
{
public:
  KColor() : QColor() {}
  KColor(QColor const& rhs) : QColor(rhs) {}
  KColor(Qt::GlobalColor color) : QColor(color) {}
  KColor(int r, int g, int b, int a = 255) : QColor(r,g,b,a) {}
  KColor(QRgb rgb) : QColor(rgb) {}
  KColor(const QString& name) : QColor(name) {}
  KColor(const char *name) : QColor(name) {}
  KColor(Spec spec) : QColor(spec) {}
};

#endif // KCOLOR_H

