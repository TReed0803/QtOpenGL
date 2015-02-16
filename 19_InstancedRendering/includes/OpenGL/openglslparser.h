#ifndef OPENGLSLPARSER_H
#define OPENGLSLPARSER_H

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
  bool parse();

private:
  OpenGLSLParserPrivate *m_private;
};


#endif // OPENGLSLPARSER_H
