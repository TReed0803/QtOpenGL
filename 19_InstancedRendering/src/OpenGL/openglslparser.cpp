#include "openglslparser.h"
#include <QDir>

#include <KAbstractReader>
#include <KAbstractWriter>
#include <KBufferedFileReader>
#include <KCommon>

/*******************************************************************************
 * OpenGLSL Parsing Definitions
 ******************************************************************************/
enum OpenGLSLToken
{
  // Special Case Tokens
  PT_ERROR,
  PT_EOF,
  PT_ENDSTATEMENT,

  // Preprocessor Tokens
  PT_PP_UNKNOWN,
  PT_PP_INCLUDE,

  // Language Tokens
  PT_CODE
};

static KParseMap<OpenGLSLToken> const sg_reserved =
{
  { "#include", PT_PP_INCLUDE }
};

typedef KParseToken<OpenGLSLToken> ParseToken;

/*******************************************************************************
 * OpenGLSL Parser Private
 ******************************************************************************/
class OpenGLSLParserPrivate : public KAbstractLexer<ParseToken>
{
public:

  // Typedefs
  typedef KAbstractLexer<ParseToken>::token_id token_id;
  typedef KAbstractLexer<ParseToken>::token_type token_type;
  typedef KAbstractLexer<ParseToken>::char_type char_type;

  // Constructors / Destructor
  OpenGLSLParserPrivate(OpenGLSLParser *parent, KAbstractReader *reader, KAbstractWriter *writer);

  // Lexer
  token_id lexToken(token_type &token);
  token_id lexCode(token_type &token);
  token_id lexLine(token_type &token);
  token_id lexPreprocessor(token_type &token);
  token_id lexPreprocessorIdentifier(token_type &token);
  token_id lexTokenInclude(token_type &token);
  token_id lexTokenIncludeString(token_type &token, char_type endChar);
  bool lexTokenIncludeAbsolute(token_type &token);
  bool lexTokenIncludeRelative(token_type &token);
  token_id lexOpenGLSLTokenentifier(token_type &token);
  token_id symResolve(token_type &token, token_id t);

  // Parser
  bool parse();
  void parseInclude();

  // Meta Information
  void setFilePath(char const *filePath);

private:
  OpenGLSLParser *m_parent;
  KAbstractWriter *m_writer;

  // Include Resolution
  QDir m_relativeDirectory;
  static std::vector<std::string> m_includePaths;
};

OpenGLSLParserPrivate::OpenGLSLParserPrivate(OpenGLSLParser *parent, KAbstractReader *reader, KAbstractWriter *writer) :
  KAbstractLexer<ParseToken>(reader), m_parent(parent), m_writer(writer)
{
  // Intentionally Empty
}

std::vector<std::string> OpenGLSLParserPrivate::m_includePaths;

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexToken(token_type &token)
{
  for (;;)
  {
    token.m_lexicon.clear();
    switch (nextChar())
    {
    case WHITESPACE:
      continue;
    case KAbstractReader::EndOfFile:
      return PT_EOF;
    case '#':
      token.m_lexicon.push_back('#');
      return lexPreprocessor(token);
    default:
      return lexCode(token);
    }
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexCode(token_type &token)
{
  (void)token;

  // Re-append whitespace
  for (int i = 1; i < this->currCharCount(); ++i)
  {
    token.m_lexicon.push_back(' ');
  }
  token.m_lexicon.push_back(currChar());

  // Lex actual line
  return lexLine(token);
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexLine(OpenGLSLParserPrivate::token_type &token)
{
  do
  {
    if (peekChar() == -1) return PT_ENDSTATEMENT;
    token.m_lexicon.push_back(nextChar());
  } while (peekChar() != '\n');
  return PT_CODE;
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexPreprocessor(token_type &token)
{
  for (;;)
  {
    switch (nextChar())
    {
    case WHITESPACE:
      continue;
    case NEWLINE:
      LEX_ERROR("Unexpected newline after preprocessor token!");
      return PT_ERROR;
    default:
      return lexPreprocessorIdentifier(token);
    }
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexPreprocessorIdentifier(token_type &token)
{
  switch (lexOpenGLSLTokenentifier(token))
  {
  case PT_PP_INCLUDE:
    return lexTokenInclude(token);
  case PT_PP_UNKNOWN:
    return lexLine(token);
  default:
    LEX_ERROR("Failed to tokenize!");
    return PT_ERROR;
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexTokenInclude(token_type &token)
{
  for (;;)
  {
    switch (nextChar())
    {
    case WHITESPACE:
      continue;
    case KAbstractReader::EndOfFile:
      return PT_EOF;
    case '<':
      return lexTokenIncludeString(token, '>');
    case '"':
      return lexTokenIncludeString(token, '"');
    default:
      LEX_ERROR("Unexpected symbol!");
      return PT_ERROR;
    }
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexTokenIncludeString(token_type &token, char_type endChar)
{
  token.m_lexicon.clear();
  while (peekChar() != endChar)
  {
    if (peekChar() == '\n') return PT_ERROR;
    token.m_lexicon += nextChar();
  }
  nextChar(); // Eat endChar

  if (token.m_lexicon.empty())
  {
    LEX_ERROR("Empty include statement found!");
  }

  if (endChar == '>')
  {
    if (lexTokenIncludeAbsolute(token)) return PT_PP_INCLUDE;
    if (lexTokenIncludeRelative(token)) return PT_PP_INCLUDE;
  }
  else if (endChar == '"')
  {
    if (lexTokenIncludeRelative(token)) return PT_PP_INCLUDE;
    if (lexTokenIncludeAbsolute(token)) return PT_PP_INCLUDE;
  }
  else
  {
    LEX_ERROR("Unexpected end character provided for lexTokenIncludeString!");
  }

  LEX_ERROR("No include file found named `%s`!", token.m_lexicon.c_str());
  return PT_ERROR;
}

bool OpenGLSLParserPrivate::lexTokenIncludeAbsolute(token_type &token)
{
  for (auto const &currPath : m_includePaths)
  {
    QDir path(currPath.c_str());
    QFileInfo file(path, token.m_lexicon.c_str());
    if (file.exists())
    {
      token.m_lexicon = file.absoluteFilePath().toUtf8().constData();
      return true;
    }
  }

  return false;
}

bool OpenGLSLParserPrivate::lexTokenIncludeRelative(token_type &token)
{
  QFileInfo file(m_relativeDirectory, token.m_lexicon.c_str());
  if (file.exists())
  {
    token.m_lexicon = file.absoluteFilePath().toUtf8().constData();
    return true;
  }
  return false;
}


OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexOpenGLSLTokenentifier(token_type &token)
{
  // Read and resolve symbol
  token.m_lexicon += static_cast<unsigned char>(currChar());
  for (;;)
  {
    if (Karma::isAlpha(peekChar()))
    {
      token.m_lexicon += nextChar();
    }
    else
    {
      return symResolve(token, PT_PP_UNKNOWN);
    }
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::symResolve(token_type &token, token_id t)
{
  auto it = sg_reserved.find(token.m_lexicon.data());
  if (it != sg_reserved.end()) return it->second;
  return t;
}

bool OpenGLSLParserPrivate::parse()
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
    case PT_PP_INCLUDE:
      parseInclude();
      break;
    case PT_PP_UNKNOWN:
    case PT_CODE:
      m_writer->append(currToken().m_lexicon.c_str());
      break;
    }
  }
}

void OpenGLSLParserPrivate::parseInclude()
{
  char const *absolutePath = currToken().m_lexicon.c_str();
  KBufferedFileReader reader(absolutePath, 2014);
  OpenGLSLParserPrivate subParse(m_parent, &reader, m_writer);
  subParse.setFilePath(absolutePath);
  subParse.initializeLexer();
  subParse.parse();
}

void OpenGLSLParserPrivate::setFilePath(const char *filePath)
{
  QFileInfo file(filePath);
  m_relativeDirectory = file.absoluteDir();
}


/////////////
#define P(c) c &p = *m_private
OpenGLSLParser::OpenGLSLParser(KAbstractReader *reader, KAbstractWriter *writer) :
  m_private(new OpenGLSLParserPrivate(this, reader, writer))
{
  // Intentionall Empty
}

OpenGLSLParser::~OpenGLSLParser()
{
  delete m_private;
}

void OpenGLSLParser::initialize()
{
  P(OpenGLSLParserPrivate);
  p.initializeLexer();
}

void OpenGLSLParser::setFilePath(const char *filePath)
{
  P(OpenGLSLParserPrivate);
  p.setFilePath(filePath);
}

bool OpenGLSLParser::parse()
{
  P(OpenGLSLParserPrivate);
  return p.parse();
}
