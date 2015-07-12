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
  FCookTorrance,
  FSphericalGaussian,
  FresnelCount
};

static std::string FToCStr(int f)
{
  switch (f)
  {
  CASE(FNone);
  CASE(FSchlick);
  CASE(FCookTorrance);
  CASE(FSphericalGaussian);
  }
  return "N/A";
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
  GSmith,
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
  CASE(GSmith);
  }
  return "N/A";
}

enum DistributionFactor
{
  DPhong,
  DBeckmann,
  DGgx,
  DistributionCount
};

static std::string DToCStr(int d)
{
  switch (d)
  {
  CASE(DPhong);
  CASE(DBeckmann);
  CASE(DGgx);
  }
  return "N/A";
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
  static int &SFactor();

protected:
  OpenGLMesh m_mesh;
  OpenGLUniformBufferObject m_blurData;
  OpenGLTexture m_shadowTexture, m_blurTexture, m_shadowDepth;
  OpenGLFramebufferObject m_shadowMappingFbo;
  OpenGLShaderProgram *m_regularLight;
  OpenGLShaderProgram *m_shadowCastingLight;
  OpenGLShaderProgram *m_shadowMappingLight;
  OpenGLShaderProgram *m_blurProgram;
  int m_uFresnel, m_uGeometry, m_uDistribution, m_uDistributionSample;
};

#endif // OPENGLABSTRACTLIGHTGROUP_H
