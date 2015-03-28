#ifndef OPENGLABSTRACTLIGHTGROUP_H
#define OPENGLABSTRACTLIGHTGROUP_H OpenGLAbstractLightGroup

class KHalfEdgeMesh;
class KMatrix4x4;
class OpenGLShaderProgram;
class OpenGLViewport;
class OpenGLScene;

#include <OpenGLMesh>
#include <OpenGLTexture>
#include <OpenGLFramebufferObject>
#include <OpenGLUniformBufferObject>

#define CASE(c) case c: return #c

enum FresnelFactor
{
  FNone,
  FSchlick,
  FSchlick2,
  FCookTorrance,
  FresnelCount
};

static std::string FToCStr(int f)
{
  switch (f)
  {
  CASE(FNone);
  CASE(FSchlick);
  CASE(FSchlick2);
  CASE(FCookTorrance);
  }
}

enum GeometryFactor
{
  GImplicit,
  GNeumann,
  GCookTorrance,
  GKelemen,
  GSmithBeckmann,
  GSmithGgx,
  GSmithSchlickBeckmann,
  GeometryCount
};

static std::string GToCStr(int g)
{
  switch (g)
  {
  CASE(GImplicit);
  CASE(GNeumann);
  CASE(GCookTorrance);
  CASE(GKelemen);
  CASE(GSmithBeckmann);
  CASE(GSmithGgx);
  CASE(GSmithSchlickBeckmann);
  }
}

enum DistributionFactor
{
  DPhong,
  DBlinnPhong,
  DBeckmann,
  DGgx,
  DistributionCount
};

static std::string DToCStr(int d)
{
  switch (d)
  {
  CASE(DPhong);
  CASE(DBlinnPhong);
  CASE(DBeckmann);
  CASE(DGgx);
  }
}

#undef CASE

class OpenGLAbstractLightGroup
{
public:
  typedef unsigned char Byte;

  // Construction Routines
  bool create();

  // Properties
  void setMesh(const OpenGLMesh &mesh);
  void setMesh(const KHalfEdgeMesh &mesh);
  void setMesh(const char *filepath);

  // Virtual
  virtual void prepMesh(OpenGLMesh &mesh) = 0;
  virtual void commit(const OpenGLViewport &view) = 0;
  virtual void draw() = 0;
  virtual void drawShadowed(OpenGLScene &scene) = 0;

  static int &FFactor();
  static int &GFactor();
  static int &DFactor();

protected:
  OpenGLMesh m_mesh;
  OpenGLUniformBufferObject m_blurData;
  OpenGLTexture m_shadowTexture, m_blurTexture, m_shadowDepth;
  OpenGLFramebufferObject m_shadowMappingFbo;
  OpenGLShaderProgram *m_regularLight[FresnelCount][GeometryCount][DistributionCount];
  OpenGLShaderProgram *m_shadowCastingLight;
  OpenGLShaderProgram *m_shadowMappingLight;
  OpenGLShaderProgram *m_blurProgram;
};

#endif // OPENGLABSTRACTLIGHTGROUP_H
