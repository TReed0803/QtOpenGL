#include "kfilereader.h"
#include <QFile>

KFileReader::KFileReader(QFile *file) :
  m_file(file)
{
  file->open(QFile::ReadOnly | QFile::Text);
}

int KFileReader::next()
{
  char c;
  if (!m_file->getChar(&c))
    return EndOfFile;
 return c;
}
