#include "kabstractobjparser.h"
#include "kabstractlexer.h"
#include "kabstractreader.h"
#include "kcommon.h"
#include "kmacros.h"
#include "kparsetoken.h"

#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <Qt>

/*******************************************************************************
 * Parser Definitions
 ******************************************************************************/

union TokenAttrib
{
  int asInteger;
  float asFloat;
};

enum TokenId
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

typedef KParseToken<TokenId, TokenAttrib> ParseToken;

static const KParseMap<TokenId> sg_reserved =
{
  { "v", PT_VERTEX },
  { "vt", PT_TEXTURE },
  { "vn", PT_NORMAL },
  { "vp", PT_PARAMETER },
  { "f", PT_FACE },
  { "o", PT_OBJECT },
  { "g", PT_GROUP },
  { "mtllib", PT_MATERIAL },
  { "usemtl", PT_USEMATERIAL },
  { "s", PT_SMOOTHING }
};

/*******************************************************************************
 * ObjParser Private
 ******************************************************************************/

class KAbstractObjParserPrivate : public KAbstractLexer<ParseToken>
{
public:
  typedef KAbstractObjParser::index_type index_type;
  typedef KAbstractObjParser::index_array index_array;
  KAbstractObjParserPrivate(KAbstractObjParser *parser, KAbstractReader *reader);

  // Lexer
  int lexReadInteger(int *sign);
  int lexReadInteger(int *sign, int *power);
  token_id lexToken(token_type &token);
  token_id lexTokenInteger(token_type &token);
  token_id lexTokenFloat(token_type &token, int sign, int integer);
  token_id lexTokenFloatExponent(token_type &token, float value);
  token_id lexTokenIdentifier(token_type &token);
  token_id symResolve(token_type &token, token_id t);

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

  // Statistics
  uint64_t m_vertexCount;
  uint64_t m_textureCount;
  uint64_t m_normalCount;
  uint64_t m_parameterCount;
  uint64_t m_faceCount;

  //Caches
  float m_float4[4];
  index_array m_index_array;
  std::vector<index_array> m_vector_index_array;

};

KAbstractObjParserPrivate::KAbstractObjParserPrivate(KAbstractObjParser *parser, KAbstractReader *reader) :
  KAbstractLexer<ParseToken>(reader), m_parser(parser),
  m_vertexCount(0), m_textureCount(0), m_normalCount(0), m_parameterCount(0), m_faceCount(0)
{
  // Intentionally Empty
}

/*******************************************************************************
 * Lexer Definitions
 ******************************************************************************/
KAbstractObjParserPrivate::token_id KAbstractObjParserPrivate::lexToken(token_type &token)
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
    case '\n':
      return PT_ENDSTATEMENT;
    case '#':
      nextLine();
      return PT_ENDSTATEMENT;
    case '/':
      return PT_SEPARATOR;
    case '.':
    default:
      if (Karma::isNumeric(currChar()))
        return lexTokenInteger(token);
      else if (Karma::isAlpha(currChar()))
        return lexTokenIdentifier(token);
      else
        LEX_ERROR("Unexpected character at (%d:%d) '%c'!\n", (int)currLineCount(), (int)currCharCount(), (char)currChar());
    }
  }
}

int KAbstractObjParserPrivate::lexReadInteger(int *sign)
{
  *sign = 1;
  int integer = 0;

  // Check for negation
  if (currChar() == '-')
    *sign = -1;
  else if (currChar() == '+')
    ; // Do nothing, sign is already 1
  else
    integer = Karma::ctoi(currChar());

  // Read the integer value
  while (Karma::isNumeric(peekChar()))
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
  if (currChar() == '-')
    *sign = -1;
  else if (currChar() == '+')
    ; // Do nothing, sign is already 1
  else
    integer = Karma::ctoi(currChar());

  // Read the integer value
  while (Karma::isNumeric(peekChar()))
  {
    pow *= 10;
    integer *= 10;
    integer += Karma::ctoi(nextChar());
  }

  (*power) = pow;
  return integer;
}

KAbstractObjParserPrivate::token_id KAbstractObjParserPrivate::lexTokenInteger(token_type &token)
{
  int sign;
  int integer = lexReadInteger(&sign);

  if (peekChar() == '.')
  {
    nextChar(); nextChar(); // Eat the decimal
    return lexTokenFloat(token, sign, integer);
  }

  // We've read an integer, set token attributes.
  token.m_attribute.asInteger = sign * integer;
  return PT_INTEGER;
}

KAbstractObjParserPrivate::token_id KAbstractObjParserPrivate::lexTokenFloat(token_type &token, int sign, int integer)
{
  int power, powSign;
  int fraction = lexReadInteger(&powSign, &power);
  float decimal = double(fraction) / power;
  float value = sign * (integer + decimal);

  if (Karma::toLower(peekChar()) == 'e')
  {
    nextChar(); nextChar(); // Eat exponent
    return lexTokenFloatExponent(token, value);
  }

  // We've read a float, set token attributes.
  token.m_attribute.asFloat = value;
  return PT_FLOAT;
}

KAbstractObjParserPrivate::token_id KAbstractObjParserPrivate::lexTokenFloatExponent(token_type &token, float value)
{
  int sign;
  int power = lexReadInteger(&sign);

  token.m_attribute.asFloat = value * std::pow(10.0f, sign * power);
  return PT_FLOAT;
}

KAbstractObjParserPrivate::token_id KAbstractObjParserPrivate::lexTokenIdentifier(token_type &token)
{
  // Read and resolve symbol
  token.m_lexicon = currChar();
  for (;;)
  {
    if (Karma::isAlpha(peekChar()))
    {
      token.m_lexicon += nextChar();
    }
    else
    {
      return symResolve(token, PT_STRING);
    }
  }
}

KAbstractObjParserPrivate::token_id KAbstractObjParserPrivate::symResolve(token_type &token, token_id t)
{
  auto it = sg_reserved.find(token.m_lexicon.data());
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

void KAbstractObjParser::initialize()
{
  P(KAbstractObjParserPrivate);
  p.initializeLexer();
}
