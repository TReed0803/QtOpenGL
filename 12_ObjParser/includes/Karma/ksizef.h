#ifndef KSIZEF_H
#define KSIZEF_H

#include <QSizeF>

class KSizeF : public QSizeF
{
public:
  Q_DECL_CONSTEXPR KSizeF() : QSizeF() {}
  Q_DECL_CONSTEXPR KSizeF(const QSize &sz) : QSizeF(sz) {}
  Q_DECL_CONSTEXPR KSizeF(const QSizeF &sz) : QSizeF(sz) {}
  Q_DECL_CONSTEXPR KSizeF(qreal w, qreal h) : QSizeF(w,h) {}
};

#endif // KSIZEF_H
