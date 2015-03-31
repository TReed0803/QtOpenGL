#ifndef KBUFFEREDBINARYFILEREADER_H
#define KBUFFEREDBINARYFILEREADER_H KBufferedBinaryFileReader

#include <KAbstractReader>
#include <QScopedPointer>
class QString;

class KBufferedBinaryFileReaderPrivate;
class KBufferedBinaryFileReader : public KAbstractReader
{
public:
  KBufferedBinaryFileReader();
  KBufferedBinaryFileReader(const QString &fileName, size_t buffsize);
  ~KBufferedBinaryFileReader();
  int next();
  bool valid();
private:
  QScopedPointer<KBufferedBinaryFileReaderPrivate> m_private;
};

#endif // KBUFFEREDBINARYFILEREADER_H
