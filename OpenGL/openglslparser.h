#ifndef OPENGLSLPARSER_H
#define OPENGLSLPARSER_H OpenGLSLParser

#include <KAbstractLexer>

class KAbstractWriter;

class OpenGLSLParserPrivate;
class OpenGLSLParser
{
public:
  OpenGLSLParser(KAbstractReader *reader, KAbstractWriter *writer);
  ~OpenGLSLParser();

  void initialize();
  void setFilePath(char const *filePath);
  void addIncludePath(char const *path);
  bool parse();

private:
  OpenGLSLParserPrivate *m_private;
};


#endif // OPENGLSLPARSER_H
