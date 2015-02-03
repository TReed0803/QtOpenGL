#ifndef KABSTRACTOBJPARSER_H
#define KABSTRACTOBJPARSER_H

#include <array>
#include <cstdint>

class KAbstractReader;

class KAbstractObjParserPrivate;
class KAbstractObjParser
{
public:
  typedef uint64_t index_type;
  typedef uint64_t size_type;
  typedef std::array<index_type, 3> index_array;
  KAbstractObjParser(KAbstractReader *reader);
  bool parse();
protected:
  virtual void onVertex(float vertex[4]) = 0;
  virtual void onTexture(float texture[3]) = 0;
  virtual void onNormal(float normal[3]) = 0;
  virtual void onParameter(float parameter[3]) = 0;
  virtual void onFace(index_array indices[], size_type count) = 0;
  virtual void onGroup(char *group) = 0;
  virtual void onMaterial(char *file) = 0;
  virtual void onUseMaterial(char *file) = 0;
  virtual void onObject(char *obj) = 0;
  virtual void onSmooth(char *obj) = 0;
private:
  KAbstractObjParserPrivate *m_private;
  friend class KAbstractObjParserPrivate;
};

#endif // KABSTRACTOBJPARSER_H
