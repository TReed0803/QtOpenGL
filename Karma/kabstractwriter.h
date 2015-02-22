#ifndef KABSTRACTWRITER_H
#define KABSTRACTWRITER_H KAbstractWriter

class KAbstractWriter
{
public:
  virtual void append(char const *buffer) = 0;
  virtual void push_back(char c) = 0;
};

#endif // KABSTRACTWRITER_H
