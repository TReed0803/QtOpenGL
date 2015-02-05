#ifndef KFILEREADER_H
#define KFILEREADER_H

#include <KAbstractReader>
#include <QScopedPointer>
class QString;

class KFileReaderPrivate;
class KFileReader : public KAbstractReader
{
public:
  KFileReader();
  KFileReader(const QString &fileName);
  ~KFileReader();
  int next();
private:
  QScopedPointer<KFileReaderPrivate> m_private;
};

#endif // KFILEREADER_H
