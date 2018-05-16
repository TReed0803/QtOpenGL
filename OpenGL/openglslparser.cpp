#include "openglslparser.h"
#include <QDir>

#include <KAbstractReader>
#include <KAbstractWriter>
#include <KBufferedFileReader>
#include <KCommon>

// GLSL 3.30r6
// (https://www.opengl.org/registry/doc/GLSLangSpec.3.30.6.clean.pdf)

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

  // GLSL Tokens
  PT_UNKNOWN,

  // Language Extensions
  PT_PP_INCLUDE,
  PT_PP_AUTOBIND,
  PT_PP_AUTOSAMPLE,

  // Language Tokens
  PT_CODE
};

static KParseMap<OpenGLSLToken> const sg_reserved =
{
  // Preprocessor
  { "#include", PT_PP_INCLUDE },
  { "#autobind", PT_PP_AUTOBIND },
  { "#autosample", PT_PP_AUTOSAMPLE }
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
  typedef OpenGLSLParser::Autoresolver Autoresolver;
  typedef OpenGLSLParser::Autosampler Autosampler;

  // Constructors / Destructor
  OpenGLSLParserPrivate(OpenGLSLParser *parent, KAbstractReader *reader, KAbstractWriter *writer);
  virtual ~OpenGLSLParserPrivate();

  // Lexer
  token_id lexToken(token_type &token);
  token_id lexCode(token_type &token);
  token_id lexLine(token_type &token);
  token_id lexPreprocessor(token_type &token);
  token_id lexPreprocessorIdentifier(token_type &token);
  token_id lexTokenInclude(token_type &token);
  token_id lexTokenIncludeString(token_type &token, char_type endChar);
  bool lexTokenIncludeAbsolute(token_type &token);
  bool lexTokenIncludeShared(token_type &token);
  bool lexTokenIncludeRelative(token_type &token);
  token_id lexTokenAutoresolve(token_type &token);
  token_id lexTokenAutosample(token_type &token);
  token_id lexOpenGLSLTokenIdentifier(token_type &token);
  token_id symResolve(token_type &token, token_id t);

  // Parser
  bool parse();
  void parseInclude();
  void autobindIdentifier();
  void autosampleIdentifier();

  // Meta Information
  void setFilePath(char const *filePath);
  void setAutoresolver(Autoresolver *a);
  void setAutosampler(Autosampler *a);
  void addIncludePath(char const *path);
  static void addSharedIncludePath(char const *path);

private:
  OpenGLSLParser *m_parent;
  KAbstractWriter *m_writer;

  // Include Resolution
  QDir m_relativeDirectory;
  std::vector<std::string> m_includePaths;
  static std::vector<std::string> m_sharedIncludePaths;
  Autoresolver *m_autobinder;
  Autosampler *m_autosampler;
};

OpenGLSLParserPrivate::OpenGLSLParserPrivate(OpenGLSLParser *parent, KAbstractReader *reader, KAbstractWriter *writer) :
  KAbstractLexer<ParseToken>(reader), m_parent(parent), m_writer(writer)
{
  // Intentionally Empty
}

OpenGLSLParserPrivate::~OpenGLSLParserPrivate()
{
  // Intentionally Empty
}

std::vector<std::string> OpenGLSLParserPrivate::m_sharedIncludePaths;

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
  for (size_type i = 1; i < this->currCharCount(); ++i)
  {
    token.m_lexicon.push_back(' ');
  }
  token.m_lexicon.push_back(currChar());

  // Lex actual line
  return lexLine(token);
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexLine(OpenGLSLParserPrivate::token_type &token)
{
  // Blank Line
  if (currChar() == '\n') return PT_CODE;

  // Otherwise
  do
  {
    if (peekChar() == -1) return PT_CODE;
    token.m_lexicon.push_back(nextChar());
  } while (currChar() != '\n');

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
  switch (lexOpenGLSLTokenIdentifier(token))
  {
  case PT_PP_INCLUDE:
    return lexTokenInclude(token);
  case PT_PP_AUTOBIND:
    return lexTokenAutoresolve(token);
  case PT_PP_AUTOSAMPLE:
    return lexTokenAutosample(token);
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
    return PT_ERROR;
  }

  if (endChar == '>')
  {
    if (lexTokenIncludeAbsolute(token)) return PT_PP_INCLUDE;
    if (lexTokenIncludeRelative(token)) return PT_PP_INCLUDE;
    if (lexTokenIncludeShared(token)) return PT_PP_INCLUDE;
  }
  else if (endChar == '"')
  {
    if (lexTokenIncludeRelative(token)) return PT_PP_INCLUDE;
    if (lexTokenIncludeAbsolute(token)) return PT_PP_INCLUDE;
    if (lexTokenIncludeShared(token)) return PT_PP_INCLUDE;
  }
  else
  {
    LEX_ERROR("Unexpected end character provided for lexTokenIncludeString!");
    return PT_ERROR;
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

bool OpenGLSLParserPrivate::lexTokenIncludeShared(token_type &token)
{
  for (auto const &currPath : m_sharedIncludePaths)
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

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexTokenAutoresolve(OpenGLSLParserPrivate::token_type &token)
{
  token.m_lexicon.clear();
  for (;;)
  {
    switch (peekChar())
    {
    case WHITESPACE:
      nextChar();
      continue;
    case NEWLINE:
      if (token.m_lexicon.empty())
      {
        LEX_ERROR("Expected an identifier for #autobind. Read nothing.");
        return PT_ERROR;
      }
      return PT_PP_AUTOBIND;
    default:
      token.m_lexicon += nextChar();
      break;
    }
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexTokenAutosample(OpenGLSLParserPrivate::token_type &token)
{
  token.m_lexicon.clear();
  for (;;)
  {
    switch (peekChar())
    {
    case WHITESPACE:
      nextChar();
      continue;
    case NEWLINE:
      if (token.m_lexicon.empty())
      {
        LEX_ERROR("Expected an identifier for #autobind. Read nothing.");
        return PT_ERROR;
      }
      return PT_PP_AUTOSAMPLE;
    default:
      token.m_lexicon += nextChar();
      break;
    }
  }
}

OpenGLSLParserPrivate::token_id OpenGLSLParserPrivate::lexOpenGLSLTokenIdentifier(token_type &token)
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
    case PT_PP_AUTOBIND:
      autobindIdentifier();
      break;
    case PT_PP_AUTOSAMPLE:
      autosampleIdentifier();
      break;
    case PT_PP_UNKNOWN:
    case PT_CODE:
      m_writer->append(currToken().m_lexicon.c_str());
      break;
    default:
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
  subParse.setAutoresolver(m_autobinder);
  subParse.setAutosampler(m_autosampler);
  subParse.initializeLexer();
  subParse.parse();
}

void OpenGLSLParserPrivate::autobindIdentifier()
{
  if (m_autobinder)
  {
    std::string target = currToken().m_lexicon;
    if (std::find(m_autobinder->begin(), m_autobinder->end(), target) == m_autobinder->end())
    {
      m_autobinder->push_back(target);
    }
  }
}

void OpenGLSLParserPrivate::autosampleIdentifier()
{
  if (m_autosampler)
  {
    std::string target = currToken().m_lexicon;
    if (std::find(m_autosampler->begin(), m_autosampler->end(), target) == m_autosampler->end())
    {
      m_autosampler->push_back(target);
    }
  }
}

void OpenGLSLParserPrivate::setFilePath(const char *filePath)
{
  QFileInfo file(filePath);
  m_relativeDirectory = file.absoluteDir();
}

void OpenGLSLParserPrivate::setAutoresolver(OpenGLSLParserPrivate::Autoresolver *a)
{
  m_autobinder = a;
}

void OpenGLSLParserPrivate::setAutosampler(OpenGLSLParserPrivate::Autosampler *a)
{
  m_autosampler = a;
}

void OpenGLSLParserPrivate::addIncludePath(const char *path)
{
  m_includePaths.push_back(path);
}

void OpenGLSLParserPrivate::addSharedIncludePath(const char *path)
{
  m_sharedIncludePaths.push_back(path);
}

/////////////

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

void OpenGLSLParser::setAutoresolver(OpenGLSLParser::Autoresolver *a)
{
  P(OpenGLSLParserPrivate);
  p.setAutoresolver(a);
}

void OpenGLSLParser::setAutosampler(OpenGLSLParser::Autosampler *a)
{
  P(OpenGLSLParserPrivate);
  p.setAutosampler(a);
}

void OpenGLSLParser::addIncludePath(const char *path)
{
  P(OpenGLSLParserPrivate);
  p.addIncludePath(path);
}

void OpenGLSLParser::addSharedIncludePath(const char *path)
{
  OpenGLSLParserPrivate::addSharedIncludePath(path);
}

bool OpenGLSLParser::parse()
{
  P(OpenGLSLParserPrivate);
  return p.parse();
}
