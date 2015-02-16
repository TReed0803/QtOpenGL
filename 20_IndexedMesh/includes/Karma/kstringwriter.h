#ifndef KSTRINGWRITER_H
#define KSTRINGWRITER_H

#include <string>

#include <KAbstractWriter>

class KStringWriterPrivate;
class KStringWriter : public KAbstractWriter
{
public:
  KStringWriter(std::string &ref);
  virtual void append(char const *buffer);
  virtual void push_back(char c);
private:
  std::string &m_reference;
};

inline KStringWriter::KStringWriter(std::string &ref) :
  m_reference(ref)
{
  // Intentionally Empty
}

inline void KStringWriter::append(char const *buffer)
{
  m_reference.append(buffer);
}

inline void KStringWriter::push_back(char c)
{
  m_reference.push_back(c);
}

#endif // KSTRINGWRITER_H
