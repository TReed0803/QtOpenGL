#ifndef KSIZE_H
#define KSIZE_H KSize

#include <QSize>

class KSize : public QSize
{
public:
  Q_DECL_CONSTEXPR KSize() : QSize() {}
  Q_DECL_CONSTEXPR KSize(QSize const &rhs) : QSize(rhs) {}
  Q_DECL_CONSTEXPR KSize(int w, int h) : QSize(w,h) {}
};

#endif // KSIZE_H
