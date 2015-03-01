#ifndef KABSTRACTLEXER_H
#define KABSTRACTLEXER_H KAbstractLexer

#include <cstdint>

#include <KMacros>
#include <KParseToken>
#include <QtGlobal>

class KAbstractReader;

class KAbstractLexerBasePrivate;
class KAbstractLexerBase
{
public:
  typedef uint64_t size_type;
  typedef int32_t char_type;

  KAbstractLexerBase(KAbstractReader *reader);
  ~KAbstractLexerBase();

  // Validity
  void initializeLexer();
  bool isValid() const;

  // Reader
  inline char_type currChar() const;
  inline char_type peekChar() const;
  size_type currCharCount() const;
  size_type currLineCount() const;
  char_type nextChar();
  void nextLine();

private:
  KAbstractLexerBasePrivate *m_private;
  int m_currChar, m_peekChar;
};

inline KAbstractLexerBase::char_type KAbstractLexerBase::currChar() const
{
  return m_currChar;
}

inline KAbstractLexerBase::char_type KAbstractLexerBase::peekChar() const
{
  return m_peekChar;
}

template <typename TokenType>
class KAbstractLexer : public KAbstractLexerBase
{
public:
  typedef TokenType token_type;
  typedef typename token_type::token_id token_id;
  typedef typename token_type::attrib_type attrib_type;

  KAbstractLexer(KAbstractReader *reader);
  inline void initializeLexer();

  inline token_type &currToken();
  inline token_type &peekToken();
  inline const token_type &currToken() const;
  inline const token_type &peekToken() const;
  token_type &nextToken();
  void expectToken(token_id t);
  bool checkToken(token_id t);

  virtual token_id lexToken(token_type &token) = 0;

private:
  token_type m_tokens[2];
  token_type *m_currToken, *m_peekToken;
};

#define WHITESPACE \
       ' ':\
  case '\t':\
  case '\v':\
  case '\f':\
  case '\r'

#define NEWLINE \
  '\n':\
  case KAbstractReader::EndOfFile

#define _LEX_ERROR(...) qFatal(__FILE__ ":" STR(__LINE__) ": ERROR - " __VA_ARGS__)
#define LEX_ERROR(...) _LEX_ERROR(__VA_ARGS__)

template <typename TokenType>
inline KAbstractLexer<TokenType>::KAbstractLexer(KAbstractReader *reader) :
  KAbstractLexerBase(reader)
{
  m_currToken = &m_tokens[0];
  m_peekToken = &m_tokens[1];
}

template <typename TokenType>
inline void KAbstractLexer<TokenType>::initializeLexer()
{
  KAbstractLexerBase::initializeLexer();
  nextToken();
}

template <typename TokenType>
inline typename KAbstractLexer<TokenType>::token_type &KAbstractLexer<TokenType>::currToken()
{
  return *m_currToken;
}

template <typename TokenType>
inline typename KAbstractLexer<TokenType>::token_type &KAbstractLexer<TokenType>::peekToken()
{
  return *m_peekToken;
}

template <typename TokenType>
inline const typename KAbstractLexer<TokenType>::token_type &KAbstractLexer<TokenType>::currToken() const
{
  return *m_currToken;
}

template <typename TokenType>
inline const typename KAbstractLexer<TokenType>::token_type &KAbstractLexer<TokenType>::peekToken() const
{
  return *m_peekToken;
}

template <typename TokenType>
inline typename KAbstractLexer<TokenType>::token_type &KAbstractLexer<TokenType>::nextToken()
{
  Q_ASSERT(isValid());

  // Swap to save dynamic allocations
  std::swap(m_currToken, m_peekToken);

  // Lex over peek token
  peekToken().m_token = lexToken(peekToken());

  // Return current token
  return currToken();
}

template <typename TokenType>
inline void KAbstractLexer<TokenType>::expectToken(token_id t)
{
  if (nextToken() != t)
  {
    LEX_ERROR("Expected token %d, received %d!\n", t, currToken().m_token);
  }
}

template <typename TokenType>
inline bool KAbstractLexer<TokenType>::checkToken(token_id t)
{
  if (peekToken() == t)
  {
    nextToken();
    return true;
  }
  return false;
}

#endif // KABSTRACTLEXER_H
