#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

class AbstractReader
{
public:
  static const int EndOfFile = -1;
  virtual int next() = 0;
};

#endif // ABSTRACTREADER_H
