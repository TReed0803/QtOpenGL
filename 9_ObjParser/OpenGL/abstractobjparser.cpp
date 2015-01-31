#include "abstractobjparser.h"
#include "common.h"
#include <QVector3D>
#include <QVector4D>
#include <string>
#include <map>
#include <QDebug>
#include "abstractreader.h"

/*******************************************************************************
 * Parsing Definition
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

class ParseMap : public std::map<std::string, ParseToken>
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

static ParseMap const sg_reserved;

/*******************************************************************************
 * ObjParser Private
 ******************************************************************************/

class AbstractObjParserPrivate
{
public:
  AbstractObjParserPrivate(AbstractReader *reader);

private:
  int peekChar();
  int nextChar();
  void nextLine();
  ParseToken lexToken();
  ParseToken lexTokenInteger();
  ParseToken lexTokenFloat(int sign);
  ParseToken lexTokenIdentifier();
  ParseToken symResolve(ParseToken t);

public:
  void skipLine();
  void lexDelay();
  ParseToken nextToken();
  ParseToken currToken();
  bool expectToken(ParseToken t);
  TokenAttrib const &attribute() const;
  uint64_t vertexCount() const;
  uint64_t textureCount() const;
  uint64_t normalCount() const;
  void vertexIncrement();
  void textureIncrement();
  void normalIncrement();

  // Static Parse variables
  AbstractObjParser::Vec3 p_vec3;
  AbstractObjParser::Vec4 p_vec4;
  AbstractObjParser::FaceIndex p_faceIndex;
  AbstractObjParser::Face p_face;
  uint64_t m_vertexCount, m_textureCount, m_normalCount, m_parameterCount, m_faceCount;

private:
  TokenAttrib m_attrib;
  std::string m_lexicon;
  ParseToken m_currToken;
  AbstractReader *m_reader;
  int m_peekChar, m_currChar;
  bool m_delay;

  // Statistics
  int m_currLineCount, m_currCharCount;
};

AbstractObjParserPrivate::AbstractObjParserPrivate(AbstractReader *reader) :
  m_reader(reader), m_peekChar(0), m_delay(false), m_currLineCount(1), m_currCharCount(-1),
  m_vertexCount(0), m_textureCount(0), m_normalCount(0), m_parameterCount(0), m_faceCount(0)
{
  // We have to cache the peek value
  nextChar();
}

int AbstractObjParserPrivate::peekChar()
{
  return m_peekChar;
}

int AbstractObjParserPrivate::nextChar()
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

void AbstractObjParserPrivate::nextLine()
{
  ++m_currLineCount;
  m_currCharCount = 0;

  // Special case: Peek char is newline
  if (m_peekChar == '\n')
  {
    m_currChar = m_peekChar;
    m_peekChar = m_reader->next();
    return;
  }

  // Iterative case: Look for newline.
  do
  {
    m_currChar = m_reader->next();
    if (m_currChar == -1) break;
  } while (m_currChar != '\n');
  m_peekChar = m_reader->next();
}
#define WHITESPACE \
       ' ':\
  case '\t':\
  case '\r'

#define NEWLINE \
  '\n'

#define NONPRINTING \
  WHITESPACE:\
  case NEWLINE

void AbstractObjParserPrivate::lexDelay()
{
  m_delay = true;
}

ParseToken AbstractObjParserPrivate::lexToken()
{
  for (;;)
  {
    m_lexicon.clear();
    switch (nextChar())
    {
    case -1:
      return PT_EOF;
    case WHITESPACE:
      // Skip whitespace
      break;
    case NEWLINE:
      return PT_ENDSTATEMENT;
    case '#':
      nextLine();
      return PT_ENDSTATEMENT;
    case '/':
      return PT_SEPARATOR;
    default:
      if (isNumeric(m_currChar))
        return lexTokenInteger();
      else if (isAlpha(m_currChar))
        return lexTokenIdentifier();
      else
        qFatal("Unexpected character at (%d:%d) '%c'!\n", static_cast<int>(m_currLineCount), static_cast<int>(m_currCharCount), static_cast<char>(m_currChar));
    }
  }
}

ParseToken AbstractObjParserPrivate::lexTokenInteger()
{
  int sign = 1;
  m_attrib.asInteger = 0;
  if (m_currChar == '-')
    sign = -1;
  else
    m_attrib.asInteger = ctoi(m_currChar);

  // Read integer until we prove is a float
  // or until we reach the end of the integer.
  for (;;)
  {
    switch (peekChar())
    {
    case -1:
    case NONPRINTING:
      goto lexFinish;
    case '.':
      nextChar(); // Eat the decimal
      return lexTokenFloat(sign);
    default:
      if (isNumeric(m_peekChar))
      {
        m_attrib.asInteger *= 10;
        m_attrib.asInteger += ctoi(nextChar());
      }
      else
      {
        goto lexFinish;
      }
      break;
    }
  }

lexFinish:
  m_attrib.asInteger *= sign;
  return PT_INTEGER;
}

ParseToken AbstractObjParserPrivate::lexTokenFloat(int sign)
{
  int integer;
  int power = 1;
  float fraction = 0;
  for (;;)
  {
    switch (peekChar())
    {
    case -1:
    case NONPRINTING:
      integer = m_attrib.asInteger;
      m_attrib.asFloat = integer;
      m_attrib.asFloat += fraction / power;
      m_attrib.asFloat *= sign;
      return PT_FLOAT;
    default:
      power *= 10;
      fraction *= 10;
      fraction += ctof(nextChar());
      break;
    }
  }
}

ParseToken AbstractObjParserPrivate::lexTokenIdentifier()
{
  m_lexicon += m_currChar;
  for (;;)
  {
    switch (peekChar())
    {
    case -1:
    case NONPRINTING:
      return symResolve(PT_STRING);
    default:
      m_lexicon += nextChar();
      break;
    }
  }
}

bool AbstractObjParserPrivate::expectToken(ParseToken t)
{
  return (nextToken() == t);
}

TokenAttrib const &AbstractObjParserPrivate::attribute() const
{
  return m_attrib;
}

ParseToken AbstractObjParserPrivate::symResolve(ParseToken t)
{
  ParseMap::const_iterator it = sg_reserved.find(m_lexicon);
  if (it != sg_reserved.end()) return it->second;
  return t;
}

/////////////

void AbstractObjParserPrivate::skipLine()
{
  nextLine();
}

ParseToken AbstractObjParserPrivate::nextToken()
{
  // Handle lexical delay
  if (m_delay)
  {
    m_delay = false;
    return m_currToken;
  }

  // Otherwise, process normal lexer.
  m_currToken = lexToken();
  return m_currToken;
}

ParseToken AbstractObjParserPrivate::currToken()
{
  return m_currToken;
}

/*******************************************************************************
 * ObjParser
 ******************************************************************************/
#define P(c) c &p = *m_private

AbstractObjParser::AbstractObjParser(AbstractReader *reader) :
  m_private(new AbstractObjParserPrivate(reader))
{

}

bool AbstractObjParser::parse()
{
  P(AbstractObjParserPrivate);

  for (;;)
  {
    switch (p.nextToken())
    {
    case PT_ERROR:
      qDebug() << "Encountered an error! Aborting";
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
      // Do nothing on end statement.
      break;

    // Not implemented
    case PT_GROUP:
    case PT_OBJECT:
    case PT_SMOOTHING:
    case PT_MATERIAL:
      p.skipLine();
      break;
    }
  }
}

bool AbstractObjParser::parseFloat(float &f)
{
  P(AbstractObjParserPrivate);
  switch (p.nextToken())
  {
  case PT_FLOAT:
    f = p.attribute().asFloat;
    break;
  case PT_INTEGER:
    f = static_cast<float>(p.attribute().asInteger);
    break;
  default:
    p.lexDelay();
    return false;
  }
  return true;
}

void AbstractObjParser::parseVertex()
{
  P(AbstractObjParserPrivate);
  ++p.m_vertexCount;
  parseFloat(p.p_vec4.x);
  parseFloat(p.p_vec4.y);
  parseFloat(p.p_vec4.z);
  if (!parseFloat(p.p_vec4.w))
    p.p_vec4.w =1.0f;
  onVertex(&p.p_vec4);
}

void AbstractObjParser::parseTexture()
{
  P(AbstractObjParserPrivate);
  ++p.m_textureCount;
  p.expectToken(PT_FLOAT);
  p.p_vec3.x = (p.attribute().asFloat);
  p.expectToken(PT_FLOAT);
  p.p_vec3.y = (p.attribute().asFloat);
  if (p.nextToken() == PT_FLOAT)
    p.p_vec3.z = (p.attribute().asFloat);
  else
    p.p_vec3.z =(1.0f);
  onTexture(&p.p_vec3);
}

void AbstractObjParser::parseNormal()
{
  P(AbstractObjParserPrivate);
  ++p.m_normalCount;
  p.expectToken(PT_FLOAT);
  p.p_vec3.x = (p.attribute().asFloat);
  p.expectToken(PT_FLOAT);
  p.p_vec3.y = (p.attribute().asFloat);
  p.expectToken(PT_FLOAT);
  p.p_vec3.z = (p.attribute().asFloat);
  onNormal(&p.p_vec3);
}

void AbstractObjParser::parseParameter()
{
  P(AbstractObjParserPrivate);
  ++p.m_parameterCount;
  p.expectToken(PT_FLOAT);
  p.p_vec3.x = (p.attribute().asFloat);
  if (p.nextToken() == PT_FLOAT)
  {
    p.p_vec3.y = (p.attribute().asFloat);
    if (p.nextToken() == PT_FLOAT)
    {
      p.p_vec3.z = (p.attribute().asFloat);
    }
    else
    {
      p.p_vec3.z = 0.0f;
    }
  }
  else
  {
    p.p_vec3.y = 0.0f;
    p.p_vec3.z = 0.0f;
  }
  onParameter(&p.p_vec3);
}

void AbstractObjParser::parseFace()
{
  P(AbstractObjParserPrivate);
  ++p.m_faceCount;
  p.p_face.clear();

  // Read in as many indices as provided.
  while (parseFaceVertex())
  {
    // Intentionally Empty
  }

  onFace(&p.p_face);
}

bool AbstractObjParser::parseFaceVertex()
{
  P(AbstractObjParserPrivate);

  switch (p.nextToken())
  {
  case PT_INTEGER:
    p.p_faceIndex.v = p.attribute().asInteger;
    while (p.p_faceIndex.v > p.m_vertexCount)
      p.p_faceIndex.v -= p.m_vertexCount;
    break;
  case PT_EOF:
  case PT_ENDSTATEMENT:
    return false;
  }

  switch (p.nextToken())
  {
  case PT_SEPARATOR:
    parseFaceTexture();
    break;
  case PT_INTEGER:
    p.lexDelay();
    p.p_faceIndex.t = 0;
    p.p_faceIndex.n = 0;
    break;
  case PT_EOF:
  case PT_ENDSTATEMENT:
    p.p_face.push_back(p.p_faceIndex);
    return false;
  }

  // Apply Face Index
  p.p_face.push_back(p.p_faceIndex);
  return true;
}

void AbstractObjParser::parseFaceTexture()
{
  P(AbstractObjParserPrivate);

  switch (p.nextToken())
  {
  case PT_SEPARATOR:
    parseFaceNormal();
    return;
  case PT_INTEGER:
    p.p_faceIndex.t = p.attribute().asInteger;
    while (p.p_faceIndex.t > p.m_textureCount)
      p.p_faceIndex.t -= p.m_textureCount;
    break;
  }

  switch (p.nextToken())
  {
  case PT_SEPARATOR:
    parseFaceNormal();
    break;
  case PT_INTEGER:
    p.lexDelay();
    p.p_faceIndex.n = 0;
    break;
  case PT_EOF:
  case PT_ENDSTATEMENT:
    p.lexDelay();
    p.p_faceIndex.n = 0;
    break;
  }
}

void AbstractObjParser::parseFaceNormal()
{
  P(AbstractObjParserPrivate);

  // Read Face Index
  p.expectToken(PT_INTEGER);
  p.p_faceIndex.n = p.attribute().asInteger;
  while (p.p_faceIndex.n > p.m_normalCount)
    p.p_faceIndex.n -= p.m_normalCount;
}
