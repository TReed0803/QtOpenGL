#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <cstdint>
#include <string>
#include <vector>

class AbstractReader;
class QString;

class AbstractObjParserPrivate;
class AbstractObjParser
{
public:

  struct Vec3
  {
    float x;
    float y;
    float z;
  };

  struct Vec4
  {
    float x;
    float y;
    float z;
    float w;
  };

  struct FaceIndex
  {
    uint64_t v;
    uint64_t t;
    uint64_t n;
  };

  typedef std::string String;
  typedef std::vector<FaceIndex> Face;

  AbstractObjParser(AbstractReader *reader);
  bool parse();
protected:
  virtual void onVertex(const Vec4 *v) = 0;
  virtual void onTexture(const Vec3 *t) = 0;
  virtual void onNormal(const Vec3 *n) = 0;
  virtual void onParameter(const Vec3 *p) = 0;
  virtual void onFace(const Face *f) = 0;
  virtual void onGroup(const String *g) = 0;
  virtual void onMaterial(const String *file) = 0;
  virtual void onUseMaterial(const String *file) = 0;
  virtual void onObject(const String *obj) = 0;
  virtual void onSmooth(const String *obj) = 0;
private:
  AbstractObjParserPrivate *m_private;
  bool parseFloat(float &f);
  void parseVertex();
  void parseTexture();
  void parseNormal();
  void parseParameter();
  void parseFace();
  bool parseFaceVertex();
  void parseFaceTexture();
  void parseFaceNormal();
};

#endif // OBJPARSER_H
