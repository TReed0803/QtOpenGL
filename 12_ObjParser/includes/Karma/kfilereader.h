#ifndef KFILEREADER_H
#define KFILEREADER_H

#include <KAbstractReader>
class QFile;

class KFileReader : public KAbstractReader
{
public:
  KFileReader(QFile *file);
  int next();
private:
  QFile *m_file;
};

#endif // KFILEREADER_H
