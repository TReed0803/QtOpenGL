#include "kabstracthdrparser.h"

#include <KMath>
#include <KMacros>
#include <KParseToken>
#include <KAbstractLexer>
#include <KCommon>
#include <KAbstractReader>

union Rgbe
{
  struct
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char e;
  };
  unsigned char data[4];
};

struct RleCode
{
  unsigned char color;
  unsigned char run;
};

/*******************************************************************************
 * Parser Definitions
 ******************************************************************************/

union TokenAttrib
{
  // Unused
};

enum TokenId
{
  PT_ERROR,
  PT_EOF,
  PT_KEYVALUE,
  PT_ENDOFHEADER,
  PT_ENDSTATEMENT
};

typedef KParseToken<TokenId, TokenAttrib> ParseToken;

/*******************************************************************************
 * KAbstractHdrParserPrivate
 ******************************************************************************/
class KAbstractHdrParserPrivate : public KAbstractLexer<ParseToken>
{
public:
  typedef KAbstractHdrParser::PixelOrder PixelOrder;
  KAbstractHdrParserPrivate(KAbstractHdrParser *parser, KAbstractReader *reader);

  // Helpers
  int readInteger();
  int readInteger(int *sign);
  Rgbe nextColor();
  RleCode nextRle();
  void writeColor(float *dest, Rgbe color);
  void writeColor(float *dest, unsigned char r, unsigned char g, unsigned char b, unsigned char e);
  void writeScanline(float *dest, unsigned char *src, int scanline);

  // Lexer
  token_id lexToken(token_type &token);
  token_id lexTokenKeyValue(token_type &token);

  // Parser
  bool parse();
  void parseDimension();

private:
  KAbstractHdrParser *m_parser;
  std::string m_key, m_value;
  PixelOrder m_xOrder, m_yOrder;
  int m_xSize, m_ySize;
};

KAbstractHdrParserPrivate::KAbstractHdrParserPrivate(KAbstractHdrParser *parser, KAbstractReader *reader) :
  KAbstractLexer<ParseToken>(reader), m_parser(parser)
{
  // Intentionally Empty
}

int KAbstractHdrParserPrivate::readInteger(int *sign)
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

Rgbe KAbstractHdrParserPrivate::nextColor()
{
  Rgbe color;
  color.r = nextChar();
  color.g = nextChar();
  color.b = nextChar();
  color.e = nextChar();
  return color;
}

RleCode KAbstractHdrParserPrivate::nextRle()
{
  RleCode code;
  code.run = nextChar();
  code.color = nextChar();
  return code;
}

void KAbstractHdrParserPrivate::writeColor(float *dest, Rgbe color)
{
  writeColor(dest, color.r, color.g, color.b, color.e);
}

void KAbstractHdrParserPrivate::writeColor(float *dest, unsigned char r, unsigned char g, unsigned char b, unsigned char e)
{
  if (e)
  {
    float f = ldexp(1.0, e - int(128+8));
    dest[0] = r * f;
    dest[1] = g * f;
    dest[2] = b * f;
  }
  else
  {
    dest[0] = dest[1] = dest[2] = 0.0f;
  }
}

void KAbstractHdrParserPrivate::writeScanline(float *dest, unsigned char *src, int scanline)
{
  switch (m_yOrder)
  {
  case KAbstractHdrParser::Positive:
    for (int i = 0; i < m_xSize; ++i)
    {
      writeColor(&dest[scanline*m_xSize*3 + i*3], src[i], src[i + m_xSize], src[i + 2 * m_xSize], src[i + 3 * m_xSize]);
    }
    break;
  case KAbstractHdrParser::Negative:
    for (int i = 0; i < m_xSize; ++i)
    {
      writeColor(&dest[(m_ySize - scanline - 1)*m_xSize*3 + i*3], src[i], src[i + m_xSize], src[i + 2 * m_xSize], src[i + 3 * m_xSize]);
    }
    break;
  }
}

int KAbstractHdrParserPrivate::readInteger()
{
  int sign;
  int integer = readInteger(&sign);
  return sign * integer;
}

/*******************************************************************************
 * Lexer Definitions
 ******************************************************************************/
KAbstractHdrParserPrivate::token_id KAbstractHdrParserPrivate::lexToken(KAbstractLexer::token_type &token)
{
  // Tokenization
  for (;;)
  {
    switch (nextChar())
    {
    case KAbstractReader::EndOfFile:
      return PT_EOF;
    case WHITESPACE:
      return PT_ENDSTATEMENT;
    case '\n':
      return PT_ENDOFHEADER;
    case '#':
      nextLine();
      return PT_ENDSTATEMENT;
    default:
      return lexTokenKeyValue(token);
    }
  }
}

KAbstractHdrParserPrivate::token_id KAbstractHdrParserPrivate::lexTokenKeyValue(KAbstractLexer::token_type &token)
{
  // Read Key
  m_key = currChar();
  for (;;)
  {
    if (peekChar() != '=')
    {
      m_key += nextChar();
    }
    else
    {
      nextChar();
      break;
    }
  }

  // Read Value
  m_value = nextChar();
  for (;;)
  {
    if (peekChar() != '\n')
    {
      m_value += nextChar();
    }
    else
    {
      nextChar();
      break;
    }
  }

  return PT_KEYVALUE;
}

/*******************************************************************************
 * Parser Definitions
 ******************************************************************************/
bool KAbstractHdrParserPrivate::parse()
{
  // Initialize lexer
  nextChar();
  if(!readExpect("#?RADIANCE\n"))
  {
    qFatal("No #?RADIANCE header, the file may be corrupt or invalid.");
    return false;
  }
  forceValidate();
  nextToken();

  // Read the Key/Value pairs
  for (;;)
  {
    if (peekToken() == PT_ENDOFHEADER) break;
    switch (nextToken())
    {
    case PT_ERROR:
      qFatal("Encountered an error! Aborting");
      return false;
    case PT_EOF:
      return true;
    case PT_KEYVALUE:
      m_parser->onKeyValue(m_key.c_str(), m_value.c_str());
    case PT_ENDOFHEADER:
      break;
    }
  }

  // Read the data
  parseDimension();
  parseDimension();
  m_parser->onResolution(m_xOrder, m_yOrder, m_xSize, m_ySize);

  // Start parsing the data
  Rgbe color;
  RleCode rle;
  float *dest = m_parser->beginData();

  int count;
  size_t repeat = 0;
  unsigned invalidCount = 0;
  unsigned char *scanline = new unsigned char[4 * m_xSize];
  unsigned char *ptr, *end;

  int scanlines = 0;
  color = nextColor();
  while (scanlines != m_ySize)
  {
    // Check for invalid color which marks RLE pixel repeat
    // Consecutive repeat invalid pixels increments repeat count.
    if (color.r == 1 && color.g == 1 && color.b == 1)
    {
      qFatal("Untested! Possibly incorrect!");
      while (color.r == 1 && color.g == 1 && color.b == 1)
      {
        repeat += (color.e << (invalidCount * 8));
        color = nextColor();
        ++invalidCount;
      }
      while (repeat)
      {
        writeColor(dest, color);
        --repeat;
      }
    }

    // Check for invalid color which marks per-element RLE
    if (color.r == 2 && color.g == 2)
    {
      // Check scanline width
      if (((color.b << 8) | color.e) != m_xSize)
      {
        qFatal("Incorrect encoded scanline width! Expected `%d`, got `%d`", m_xSize, int((color.r << 8) | color.e));
      }

      // Read all channels
      ptr = &scanline[0];
      int written = 0;
      for (int channel = 0; channel < 4; ++channel)
      {
        end = &scanline[(channel+1)*m_xSize];
        while (ptr < end)
        {
          rle = nextRle();
          if (rle.run > 128)
          {
            count = int(rle.run) - 128;
            Q_ASSERT(count != 0);
            Q_ASSERT(count <= end - ptr);
            while (count > 0)
            {
              ++written;
              *ptr++ = rle.color;
              --count;
            }
          }
          else
          {
            count = int(rle.run) - 1;
            Q_ASSERT(count != -1);
            Q_ASSERT(count <= end - ptr);
            *ptr++ = rle.color;
            ++written;
            while (count > 0)
            {
              ++written;
              *ptr++ = nextChar();
              --count;
            }
          }
        }
      }

      // Output the scanline data
      writeScanline(dest, scanline, scanlines);
      ++scanlines;
    }

    color = nextColor();
  }
  m_parser->endData();

  delete [] scanline;

  return false;
}

void KAbstractHdrParserPrivate::parseDimension()
{
  int *size;
  PixelOrder order;

  // First, get the dimension order
  switch (nextChar())
  {
  case '-':
    order = PixelOrder::Negative;
    break;
  case '+':
    order = PixelOrder::Positive;
    break;
  default:
    qFatal("Expected {-+} sign for pixel order! Got, `%c`", currChar());
    break;
  }

  // Next, get what dimension we are parsing.
  switch (nextChar())
  {
  case 'Y':
    m_yOrder = order;
    size = &m_ySize;
    break;
  case 'X':
    m_xOrder = order;
    size = &m_xSize;
    break;
  default:
    qFatal("Expected {XY} sign for dimension! Got, `%c`", currChar());
    break;
  }

  // Find the dimensionality
  nextChar(); nextChar();
  (*size) = readInteger();
  nextChar();
}

/*******************************************************************************
 * KAbstractHdrParser
 ******************************************************************************/
KAbstractHdrParser::KAbstractHdrParser(KAbstractReader *reader) :
  m_private(new KAbstractHdrParserPrivate(this, reader))
{
  // Intentionally Empty
}

KAbstractHdrParser::~KAbstractHdrParser()
{
  delete m_private;
}

void KAbstractHdrParser::initialize()
{
  m_private->initializeLexer();
}

bool KAbstractHdrParser::parse()
{
  return m_private->parse();
}
