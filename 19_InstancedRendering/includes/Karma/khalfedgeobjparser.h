#ifndef KHALFEDGEOBJPARSER_H
#define KHALFEDGEOBJPARSER_H

#include <KAbstractObjParser>

class KHalfEdgeMesh;

class KHalfEdgeObjParser : public KAbstractObjParser
{
public:
  KHalfEdgeObjParser(KHalfEdgeMesh *mesh, KAbstractReader *reader);
protected:
  virtual void onVertex(float vertex[4]);
  virtual void onTexture(float texture[3]);
  virtual void onNormal(float normal[3]);
  virtual void onParameter(float parameter[3]);
  virtual void onFace(index_array indices[], size_type count);
  void triangulateFace(index_array indices[], size_type count);
  virtual void onGroup(char *group);
  virtual void onMaterial(char *file);
  virtual void onUseMaterial(char *mat);
  virtual void onObject(char *obj);
  virtual void onSmooth(char *smooth);
private:
  KHalfEdgeMesh *m_mesh;
};

#endif // KHALFEDGEOBJPARSER_H
