#include "kabstractobjparser.h"
#include "kabstractreader.h"
#include "kcommon.h"
#include "kmacros.h"

#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <Qt>

#define _LEX_ERROR(...) qFatal(__FILE__ ":" STR(__LINE__) ": ERROR - " __VA_ARGS__)
#define LEX_ERROR(fmt,...) _LEX_ERROR(fmt,__VA_ARGS__)

/*******************************************************************************
 * Parser Definitions
 ******************************************************************************/

union TokenAttrib
{
  int asInteger;
  float asFloat;
};

enum ParseToken
{
  PT_ERROR,
  PT_EOF,
  PT_VERTEX,
  PT_TEXTURE,
  PT_NORMAL,
  PT_PARAMETER,
  PT_FACE,
  PT_OBJECT,
  PT_GROUP,
  PT_ENDSTATEMENT,
  PT_STRING,
  PT_INTEGER,
  PT_FLOAT,
  PT_MATERIAL,
  PT_USEMATERIAL,
  PT_SMOOTHING,
  PT_SEPARATOR
};

struct ParseCompare : public std::binary_function<bool, char const*, char const*>
{
  inline bool operator()(char const* lhs, char const* rhs) const
  {
    return std::strcmp(lhs, rhs) < 0;
  }
};

class ParseMap : public std::map<char const*, ParseToken, ParseCompare>
{
public:
  ParseMap()
  {
    insert(value_type("v",PT_VERTEX));
    insert(value_type("vt",PT_TEXTURE));
    insert(value_type("vn",PT_NORMAL));
    insert(value_type("vp",PT_PARAMETER));
    insert(value_type("f",PT_FACE));
    insert(value_type("o",PT_OBJECT));
    insert(value_type("g",PT_GROUP));
    insert(value_type("mtllib",PT_MATERIAL));
    insert(value_type("usemtl",PT_USEMATERIAL));
    insert(value_type("s",PT_SMOOTHING));
  }
};

class Token
{
public:
  ParseToken m_token;
  std::string m_lexicon;
  //ParseString m_lexicon;
  TokenAttrib m_attribute;
  inline Token();
  inline operator ParseToken();
private:
  inline Token(Token const &rhs);
  inline Token(Token &&rhs);
  inline void operator=(Token const &rhs);
  inline void operator=(Token &&rhs);
};

inline Token::Token() :
  m_token(PT_ERROR)
{
  // Intentionally Empty
}

inline Token::operator ParseToken()
{
  return m_token;
}

#define WHITESPACE \
       ' ':\
  case '\t':\
  case '\r'

#define NEWLINE \
  '\n'

static ParseMap const sg_reserved;

/*******************************************************************************
 * ObjParser Private
 ******************************************************************************/

class KAbstractObjParserPrivate
{
public:
  typedef KAbstractObjParser::index_type index_type;
  typedef KAbstractObjParser::index_array index_array;
  KAbstractObjParserPrivate(KAbstractObjParser *parser, KAbstractReader *reader);

  // Reader
  inline int currChar();
  inline int peekChar();
  int nextChar();
  void nextLine();

  // Lexer
  inline Token &currToken();
  inline Token &peekToken();
  Token &nextToken();
  void expectToken(ParseToken t);
  bool checkToken(ParseToken t);
  int lexReadInteger(int *sign);
  int lexReadInteger(int *sign, int *power);
  ParseToken lexToken(Token &token);
  ParseToken lexTokenInteger(Token &token);
  ParseToken lexTokenFloat(Token &token, int sign, int integer);
  ParseToken lexTokenFloatExponent(Token &token, float value);
  ParseToken lexTokenIdentifier(Token &token);
  ParseToken symResolve(Token &token, ParseToken t);

  // Parser
  bool parse();
  bool parseFloat(float &f);
  bool parseIndex(index_type &i);
  void parseVertex();
  void parseTexture();
  void parseNormal();
  void parseParameter();
  void parseFace();
  bool parseFaceIndices();

private:
  KAbstractObjParser *m_parser;
  // Reader
  KAbstractReader *m_reader;
  int m_currChar, m_peekChar;
  // Lexer
  Token m_tokens[2];
  Token *m_currToken, *m_peekToken;
  // Statistics
  int m_currLineCount, m_currCharCount;
  uint64_t m_vertexCount, m_textureCount, m_normalCount, m_parameterCount, m_faceCount;

  //Caches
  float m_float4[4];
  index_array m_index_array;
  std::vector<index_array> m_vector_index_array;
};

KAbstractObjParserPrivate::KAbstractObjParserPrivate(KAbstractObjParser *parser, KAbstractReader *reader) :
  m_parser(parser), m_reader(reader), m_currLineCount(1), m_currCharCount(-1),
  m_vertexCount(0), m_textureCount(0), m_normalCount(0), m_parameterCount(0), m_faceCount(0)
{
  // We have to cache the peek values
  m_currToken = &m_tokens[0];
  m_peekToken = &m_tokens[1];
  nextChar();
  nextToken();
}

/*******************************************************************************
 * Reader Definitions
 ******************************************************************************/
inline int KAbstractObjParserPrivate::currChar()
{
  return m_currChar;
}

inline int KAbstractObjParserPrivate::peekChar()
{
  return m_peekChar;
}

int KAbstractObjParserPrivate::nextChar()
{
  m_currChar = m_peekChar;
  m_peekChar = m_reader->next();

  // Increment line/character counter
  if (m_currChar == '\n')
  {
    ++m_currLineCount;
    m_currCharCount = 0;
  }
  else
  {
    ++m_currCharCount;
  }

  return m_currChar;
}

void KAbstractObjParserPrivate::nextLine()
{
  // Batch Statistic manipulation
  ++m_currLineCount;
  m_currCharCount = 0;

  // Read until newline ignoring everything.
  m_currChar = m_peekChar;
  while (m_currChar != '\n')
  {
    m_currChar = m_reader->next();
    if (m_currChar == KAbstractReader::EndOfFile) break;
  }
  m_peekChar = m_reader->next();
}

/*******************************************************************************
 * Lexer Definitions
 ******************************************************************************/
inline Token &KAbstractObjParserPrivate::currToken()
{
  return *m_currToken;
}

inline Token &KAbstractObjParserPrivate::peekToken()
{
  return *m_peekToken;
}

Token &KAbstractObjParserPrivate::nextToken()
{
  // Swap to save dynamic allocations
  std::swap(m_currToken, m_peekToken);

  // Lex over peek token
  peekToken().m_token = lexToken(peekToken());

  // Return current token
  return currToken();
}

void KAbstractObjParserPrivate::expectToken(ParseToken t)
{
  if (nextToken() != t)
  {
    LEX_ERROR("Expected token %d, received %d!\n", t, currToken().m_token);
  }
}

bool KAbstractObjParserPrivate::checkToken(ParseToken t)
{
  if (peekToken() == t)
  {
    nextToken();
    return true;
  }
  return false;
}

ParseToken KAbstractObjParserPrivate::lexToken(Token &token)
{

  // Skip unimplemented features
  switch (currToken())
  {
  case PT_GROUP:
  case PT_OBJECT:
  case PT_SMOOTHING:
  case PT_MATERIAL:
    nextLine();
    break;
  }

  // Tokenization
  for (;;)
  {
    switch (nextChar())
    {
    case KAbstractReader::EndOfFile:
      return PT_EOF;
    case WHITESPACE:
      continue;
    case NEWLINE:
      return PT_ENDSTATEMENT;
    case '#':
      nextLine();
      return PT_ENDSTATEMENT;
    case '/':
      return PT_SEPARATOR;
    case '.':
    default:
      if (Karma::isNumeric(m_currChar))
        return lexTokenInteger(token);
      else if (Karma::isAlpha(m_currChar))
        return lexTokenIdentifier(token);
      else
        LEX_ERROR("Unexpected character at (%d:%d) '%c'!\n", (int)m_currLineCount, (int)m_currCharCount, (char)m_currChar);
    }
  }
}

int KAbstractObjParserPrivate::lexReadInteger(int *sign)
{
  *sign = 1;
  int integer = 0;

  // Check for negation
  if (m_currChar == '-')
    *sign = -1;
  else if (m_currChar == '+')
    ; // Do nothing, sign is already 1
  else
    integer = Karma::ctoi(m_currChar);

  // Read the integer value
  while (Karma::isNumeric(m_peekChar))
  {
    integer *= 10;
    integer += Karma::ctoi(nextChar());
  }

  return integer;
}

int KAbstractObjParserPrivate::lexReadInteger(int *sign, int *power)
{
  *sign = 1;
  int pow = 10;
  int integer = 0;

  // Check for negation
  if (m_currChar == '-')
    *sign = -1;
  else if (m_currChar == '+')
    ; // Do nothing, sign is already 1
  else
    integer = Karma::ctoi(m_currChar);

  // Read the integer value
  while (Karma::isNumeric(m_peekChar))
  {
    pow *= 10;
    integer *= 10;
    integer += Karma::ctoi(nextChar());
  }

  (*power) = pow;
  return integer;
}

ParseToken KAbstractObjParserPrivate::lexTokenInteger(Token &token)
{
  int sign;
  int integer = lexReadInteger(&sign);

  if (m_peekChar == '.')
  {
    nextChar(); nextChar(); // Eat the decimal
    return lexTokenFloat(token, sign, integer);
  }

  // We've read an integer, set token attributes.
  token.m_attribute.asInteger = sign * integer;
  return PT_INTEGER;
}

ParseToken KAbstractObjParserPrivate::lexTokenFloat(Token &token, int sign, int integer)
{
  int power, powSign;
  int fraction = lexReadInteger(&powSign, &power);
  float decimal = double(fraction) / power;
  float value = sign * (integer + decimal);

  if (Karma::toLower(m_peekChar) == 'e')
  {
    nextChar(); nextChar(); // Eat exponent
    return lexTokenFloatExponent(token, value);
  }

  // We've read a float, set token attributes.
  token.m_attribute.asFloat = value;
  return PT_FLOAT;
}

ParseToken KAbstractObjParserPrivate::lexTokenFloatExponent(Token &token, float value)
{
  int sign;
  int power = lexReadInteger(&sign);

  token.m_attribute.asFloat = value * std::pow(10.0f, sign * power);
  return PT_FLOAT;
}

ParseToken KAbstractObjParserPrivate::lexTokenIdentifier(Token &token)
{
  // Read and resolve symbol
  token.m_lexicon = m_currChar;
  for (;;)
  {
    if (Karma::isAlpha(m_peekChar))
    {
      token.m_lexicon += nextChar();
    }
    else
    {
      return symResolve(token, PT_STRING);
    }
  }
}

ParseToken KAbstractObjParserPrivate::symResolve(Token &token, ParseToken t)
{
  ParseMap::const_iterator it = sg_reserved.find(token.m_lexicon.data());
  if (it != sg_reserved.end()) return it->second;
  return t;
}

/*******************************************************************************
 * Parser Definitions
 ******************************************************************************/

bool KAbstractObjParserPrivate::parse()
{
  for (;;)
  {
    switch (nextToken())
    {
    case PT_ERROR:
      qFatal("Encountered an error! Aborting");
      return false;
    case PT_EOF:
      return true;
    case PT_VERTEX:
      parseVertex();
      break;
    case PT_TEXTURE:
      parseTexture();
      break;
    case PT_NORMAL:
      parseNormal();
      break;
    case PT_PARAMETER:
      parseParameter();
      break;
    case PT_FACE:
      parseFace();
    case PT_ENDSTATEMENT:
      break;
    }
  }
}

bool KAbstractObjParserPrivate::parseFloat(float &f)
{
  switch (peekToken().m_token)
  {
  case PT_FLOAT:
    f = nextToken().m_attribute.asFloat;
    break;
  case PT_INTEGER:
    f = static_cast<float>(nextToken().m_attribute.asInteger);
    break;
  default:
    return false;
  }
  return true;
}

bool KAbstractObjParserPrivate::parseIndex(index_type &i)
{
  if (peekToken().m_token == PT_INTEGER)
  {
    i = static_cast<uint64_t>(nextToken().m_attribute.asInteger);
    return true;
  }
  return false;
}

void KAbstractObjParserPrivate::parseVertex()
{
  ++m_vertexCount;
  parseFloat(m_float4[0]);
  parseFloat(m_float4[1]);
  parseFloat(m_float4[2]);
  if (!parseFloat(m_float4[3]))
    m_float4[3] = 1.0f;

  m_parser->onVertex(m_float4);
}

void KAbstractObjParserPrivate::parseTexture()
{
  ++m_textureCount;
  parseFloat(m_float4[0]);
  parseFloat(m_float4[1]);
  if (!parseFloat(m_float4[2]))
    m_float4[2] = 1.0f;

  m_parser->onTexture(m_float4);
}

void KAbstractObjParserPrivate::parseNormal()
{
  ++m_normalCount;
  parseFloat(m_float4[0]);
  parseFloat(m_float4[1]);
  parseFloat(m_float4[2]);

  m_parser->onNormal(m_float4);
}

void KAbstractObjParserPrivate::parseParameter()
{
  ++m_parameterCount;
  parseFloat(m_float4[0]);
  if (!parseFloat(m_float4[1]))
    m_float4[1] = 0.0f;
  else if (!parseFloat(m_float4[2]))
    m_float4[2] = 0.0f;

  m_parser->onParameter(m_float4);
}

void KAbstractObjParserPrivate::parseFace()
{
  m_vector_index_array.clear();

  while ( parseFaceIndices() )
  {
    m_vector_index_array.push_back(m_index_array);
  }

  m_parser->onFace(m_vector_index_array.data(), m_vector_index_array.size());
}

bool KAbstractObjParserPrivate::parseFaceIndices()
{
  // If there is no starting integer, there is no index
  if (!parseIndex(m_index_array[0]))
  {
    return false;
  }

  // Check for subequent indices (texture)
  if (checkToken(PT_SEPARATOR))
  {
    if (!parseIndex(m_index_array[1]))
    {
      m_index_array[1] = 0;
    }
  }
  else
  {
    m_index_array[1] = 0;
  }

  // Check for subequent indices (normal)
  if (checkToken(PT_SEPARATOR))
  {
    if (!parseIndex(m_index_array[2]))
    {
       m_index_array[2] = 0;
    }
  }
  else
  {
    m_index_array[2] = 0;
  }

  return true;
}

/*******************************************************************************
 * ObjParser
 ******************************************************************************/
#define P(c) c &p = *m_private

KAbstractObjParser::KAbstractObjParser(KAbstractReader *reader) :
  m_private(new KAbstractObjParserPrivate(this, reader))
{
  // Intentionally Empty
}

bool KAbstractObjParser::parse()
{
  P(KAbstractObjParserPrivate);
  return p.parse();
}
