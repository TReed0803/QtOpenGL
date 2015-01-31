#ifndef FILEREADER_H
#define FILEREADER_H

#include "abstractreader.h"
class QFile;

class FileReader : public AbstractReader
{
public:
  FileReader(QFile *file);
  int next();
private:
  QFile *m_file;
};

#endif // FILEREADER_H
