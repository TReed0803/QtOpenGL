#ifndef HALFEDGEOBJPARSER_H
#define HALFEDGEOBJPARSER_H

#include "abstractobjparser.h"

class HalfEdgeMesh;

class HalfEdgeObjParser : public AbstractObjParser
{
public:
  HalfEdgeObjParser(HalfEdgeMesh *mesh, AbstractReader *reader);
protected:
  virtual void onVertex(const Vec4 *v);
  virtual void onTexture(const Vec3 *t);
  virtual void onNormal(const Vec3 *n);
  virtual void onParameter(const Vec3 *p);
  virtual void onFace(const Face *f);
  virtual void onGroup(const String *g);
  virtual void onMaterial(const String *file);
  virtual void onUseMaterial(const String *mat);
  virtual void onObject(const String *obj);
  virtual void onSmooth(const String *s);
private:
  HalfEdgeMesh *m_mesh;
};

#endif // HALFEDGEOBJPARSER_H
