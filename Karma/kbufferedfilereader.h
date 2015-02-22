#ifndef KBUFFEREDFILEREADER_H
#define KBUFFEREDFILEREADER_H KBufferedFileReader

#include <KAbstractReader>
#include <QScopedPointer>
class QString;

class KBufferedFileReaderPrivate;
class KBufferedFileReader : public KAbstractReader
{
public:
  KBufferedFileReader();
  KBufferedFileReader(const QString &fileName, size_t buffsize);
  ~KBufferedFileReader();
  int next();
  bool valid();
private:
  QScopedPointer<KBufferedFileReaderPrivate> m_private;
};

#endif // KBUFFEREDFILEREADER_H
