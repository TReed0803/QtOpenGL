#ifndef QKEYCONVERTERCUSTOM_H
#define QKEYCONVERTERCUSTOM_H

#include <Qt>

class KeyConverterCustom
{
public:
  static int convertKey(Qt::Key k);
  static Qt::Key revertKey(int k);
  static const int KeyCount = 155;
};


#endif // QKEYCONVERTERCUSTOM_H
