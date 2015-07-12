#include "kimage.h"

#include <KMacros>

static size_t FormatSize(KImage::Format fmt)
{
  switch (fmt)
  {
  case KImage::Float:
    return sizeof(float);
  }
  return 0;
}

class KImagePrivate
{
public:
  KImagePrivate();
  ~KImagePrivate();
  KImage::Byte *m_data;
};

KImagePrivate::KImagePrivate() :
  m_data(0)
{
  // Intentionally Empty
}

KImagePrivate::~KImagePrivate()
{
  delete [] m_data;
}

KImage::KImage() :
  m_private(new KImagePrivate)
{
  // Intentionally Empty
}

KImage::~KImage()
{
  // Intentionally Empty
}

void KImage::allocate(int width, int height, KImage::Format fmt, int channels)
{
  m_private = new KImagePrivate;
  m_private->m_data = new Byte[FormatSize(fmt)*width*height*channels];
}
