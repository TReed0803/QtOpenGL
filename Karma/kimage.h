#ifndef KIMAGE_H
#define KIMAGE_H

#include <KSharedPointer>

class KImagePrivate;
class KImage
{
public:
  KImage();
  ~KImage();

  typedef unsigned char Byte;

  enum Format
  {
    Float
  };

  void allocate(int width, int height, Format fmt, int channels);
  Byte const *data() const;

private:
  KSharedPointer<KImagePrivate> m_private;
};

#endif // KIMAGE_H
