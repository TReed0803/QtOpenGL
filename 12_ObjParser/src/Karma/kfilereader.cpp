#include "kfilereader.h"
#include <QFile>
#include <QString>

/*******************************************************************************
 * KFileReaderPrivate
 ******************************************************************************/
class KFileReaderPrivate
{
public:
  inline KFileReaderPrivate();
  inline KFileReaderPrivate(FILE *file);
  inline KFileReaderPrivate(const QString &fileName);
  inline int next();
  QFile m_file;
  char m_currChar;
};

inline KFileReaderPrivate::KFileReaderPrivate() :
  m_file()
{
  // Intentionally Empty
}

inline KFileReaderPrivate::KFileReaderPrivate(const QString &fileName) :
  m_file(fileName)
{
  m_file.open(QFile::ReadOnly | QFile::Text);
}

inline int KFileReaderPrivate::next()
{
  if (!m_file.getChar(&m_currChar))
    return KFileReader::EndOfFile;
 return m_currChar;
}

/*******************************************************************************
 * KFileReader
 ******************************************************************************/
#define P(c) c &p = *m_private

KFileReader::KFileReader() :
  m_private(new KFileReaderPrivate())
{
  // Intentionally Empty
}

KFileReader::KFileReader(const QString &fileName) :
  m_private(new KFileReaderPrivate(fileName))
{
  // Intentionally Empty
}

KFileReader::~KFileReader()
{
  // Intentionally Empty
}

int KFileReader::next()
{
  P(KFileReaderPrivate);
  return p.next();
}
