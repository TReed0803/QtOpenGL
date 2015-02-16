#ifndef KSTRING_H
#define KSTRING_H

#include <QString>

class KString : public QString
{
public:
  inline KString() : QString() {}
  inline KString(QString const &rhs) : QString(rhs) {}
  explicit KString(const QChar *unicode, int size = -1) : QString(unicode, size) {}
  KString(QChar c) : QString(c) {}
  KString(int size, QChar c) : QString(size, c) {}
  inline KString(QLatin1String latin1) : QString(latin1) {}
};

#endif // KSTRING_H
