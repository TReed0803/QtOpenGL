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
  virtual void onVertex(const float vertex[4]) = 0;
  virtual void onTexture(const float texture[3]) = 0;
  virtual void onNormal(const float normal[3]) = 0;
  virtual void onParameter(const float parameter[3]) = 0;
  virtual void onFace(const index_type indices[3], size_type count) = 0;
  virtual void onGroup(const char *group) = 0;
  virtual void onMaterial(const char *file) = 0;
  virtual void onUseMaterial(const char *file) = 0;
  virtual void onObject(const char *obj) = 0;
  virtual void onSmooth(const char *obj) = 0;
private:
  KAbstractObjParserPrivate *m_private;
  friend class KAbstractObjParserPrivate;
};

#endif // KABSTRACTOBJPARSER_H
