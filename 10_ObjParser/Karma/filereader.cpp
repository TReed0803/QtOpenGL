#include "filereader.h"
#include <QFile>

FileReader::FileReader(QFile *file) :
  m_file(file)
{
  file->open(QFile::ReadOnly | QFile::Text);
}

int FileReader::next()
{
  char c;
  if (!m_file->getChar(&c))
    return EndOfFile;
 return c;
}
