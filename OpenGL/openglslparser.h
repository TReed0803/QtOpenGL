#ifndef OPENGLSLPARSER_H
#define OPENGLSLPARSER_H OpenGLSLParser

#include <KAbstractLexer>
#include <vector>

class KAbstractWriter;

class OpenGLSLParserPrivate;
class OpenGLSLParser
{
public:
  typedef std::vector<std::string> Autoresolver;
  OpenGLSLParser(KAbstractReader *reader, KAbstractWriter *writer);
  ~OpenGLSLParser();

  void initialize();
  void setFilePath(char const *filePath);
  void setAutoresolver(Autoresolver *a);
  void addIncludePath(char const *path);
  static void addSharedIncludePath(char const *path);
  bool parse();

private:
  OpenGLSLParserPrivate *m_private;
};


#endif // OPENGLSLPARSER_H
