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
  typedef std::vector<std::string> Autosampler;
  OpenGLSLParser(KAbstractReader *reader, KAbstractWriter *writer);
  virtual ~OpenGLSLParser();

  void initialize();
  void setFilePath(char const *filePath);
  void setAutoresolver(Autoresolver *a);
  void setAutosampler(Autosampler *a);
  void addIncludePath(char const *path);
  static void addSharedIncludePath(char const *path);
  bool parse();

private:
  OpenGLSLParserPrivate *m_private;
};


#endif // OPENGLSLPARSER_H
