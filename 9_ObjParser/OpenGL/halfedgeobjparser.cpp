#include "halfedgeobjparser.h"
#include "halfedgemesh.h"
#include <QVector3D>

HalfEdgeObjParser::HalfEdgeObjParser(HalfEdgeMesh *mesh, AbstractReader *reader) :
  AbstractObjParser(reader), m_mesh(mesh)
{
  // Intentionally Empty
}

void HalfEdgeObjParser::onVertex(const Vec4 *v)
{
  m_mesh->addVertex(*v);
}

void HalfEdgeObjParser::onTexture(const Vec3 *t)
{
  // Unsupported
  (void)t;
}

void HalfEdgeObjParser::onNormal(const Vec3 *n)
{
  // Unsupported
  (void)n;
}

void HalfEdgeObjParser::onParameter(const Vec3 *p)
{
  // Unsupported
  (void)p;
}

void HalfEdgeObjParser::onFace(const Face *f)
{
  m_mesh->addFace(*f);
}

void HalfEdgeObjParser::onGroup(const String *g)
{
  // Unsupported
  (void)g;
}

void HalfEdgeObjParser::onMaterial(const String *file)
{
  // Unsupported
  (void)file;
}

void HalfEdgeObjParser::onUseMaterial(const String *mat)
{
  // Unsupported
  (void)mat;
}

void HalfEdgeObjParser::onObject(const String *obj)
{
  // Unsupported
  (void)obj;
}

void HalfEdgeObjParser::onSmooth(const String *s)
{
  // Unsupported
  (void)s;
}
