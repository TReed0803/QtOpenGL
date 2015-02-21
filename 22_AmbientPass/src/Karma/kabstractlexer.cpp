#include "kabstractlexer.h"
#include "kabstractreader.h"

#include <QtGlobal>

class KAbstractLexerBasePrivate
{
public:
  explicit KAbstractLexerBasePrivate(KAbstractReader *reader);
  KAbstractReader *m_reader;
  bool m_initialized;
  int m_currLineCount, m_currCharCount;
};

KAbstractLexerBasePrivate::KAbstractLexerBasePrivate(KAbstractReader *reader) :
  m_reader(reader), m_initialized(false), m_currLineCount(1), m_currCharCount(-1)
{
  // Intentionally Empty
}

#define P(c) c &p = *m_private
KAbstractLexerBase::KAbstractLexerBase(KAbstractReader *reader) :
  m_private(new KAbstractLexerBasePrivate(reader))
{
  // Intentionally Empty
}

KAbstractLexerBase::~KAbstractLexerBase()
{
  delete m_private;
}

void KAbstractLexerBase::initializeLexer()
{
  P(KAbstractLexerBasePrivate);
  nextChar();
  p.m_initialized = true;
}

bool KAbstractLexerBase::isValid() const
{
  P(KAbstractLexerBasePrivate);
  return p.m_initialized;
}

KAbstractLexerBase::char_type KAbstractLexerBase::nextChar()
{
  P(KAbstractLexerBasePrivate);

  m_currChar = m_peekChar;
  m_peekChar = p.m_reader->next();

  // Increment line/character counter
  if (m_currChar == '\n')
  {
    ++p.m_currLineCount;
    p.m_currCharCount = 0;
  }
  else
  {
    ++p.m_currCharCount;
  }

  return m_currChar;
}

void KAbstractLexerBase::nextLine()
{
  P(KAbstractLexerBasePrivate);

  // Batch Statistic manipulation
  ++p.m_currLineCount;
  p.m_currCharCount = 0;

  // Read until newline ignoring everything.
  m_currChar = m_peekChar;
  while (m_currChar != '\n')
  {
    m_currChar = p.m_reader->next();
    if (m_currChar == KAbstractReader::EndOfFile) break;
  }
  m_peekChar = p.m_reader->next();
}

KAbstractLexerBase::size_type KAbstractLexerBase::currCharCount() const
{
  return m_private->m_currCharCount;
}

KAbstractLexerBase::size_type KAbstractLexerBase::currLineCount() const
{
  return m_private->m_currLineCount;
}
