#include "opengldebugdraw.h"

#include <vector>
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLShaderProgram>
#include <OpenGLVertexArrayObject>
#include <KCamera3D>
#include <KRectF>
#include <KTransform3D>
#include <KDebugVertex>
#include <KMath>
#include <OpenGLTexture>
#include <OpenGLDynamicBuffer>

/*******************************************************************************
 * OpenGLAbstractDebugGroup
 ******************************************************************************/
class OpenGLAbstractDebugGroup
{
public:
  typedef std::vector<KDebugVertex> KDebugVertexContainer;

  virtual void create(unsigned idx) = 0;
  virtual void bind() = 0;
  virtual void write(KDebugVertex *&dest) const = 0;
  virtual void draw(GLsizei offset) = 0;
  virtual void release() = 0;
  virtual void teardown() = 0;
  virtual void clear() = 0;
  virtual GLsizei size() const = 0;
};

/*******************************************************************************
 * OpenGLDebugGroup
 ******************************************************************************/
class OpenGLDebugGroup : public OpenGLAbstractDebugGroup
{
public:
  typedef std::vector<KDebugVertex> KDebugVertexContainer;

  OpenGLDebugGroup(unsigned &idx, GLenum drawMode, char const *vertex, char const *fragment);
  virtual void create(unsigned idx);
  virtual void bind();
  virtual void write(KDebugVertex *&dest) const;
  virtual void draw(GLsizei offset);
  virtual void release();
  virtual void teardown();
  virtual void clear();
  virtual GLsizei size() const;
  KDebugVertexContainer &vertices();

protected:
  GLenum m_drawMode;
  unsigned *m_idx;
  char const *m_vertexFile;
  char const *m_fragmentFile;
  OpenGLShaderProgram *m_program;
  KDebugVertexContainer m_vertices;
};

OpenGLDebugGroup::OpenGLDebugGroup(unsigned &idx, GLenum drawMode, char const *vertex, char const *fragment) :
  m_drawMode(drawMode), m_idx(&idx), m_vertexFile(vertex), m_fragmentFile(fragment), m_program(0)
{
  // Intentionally Empty
}

void OpenGLDebugGroup::create(unsigned idx)
{
  *m_idx = idx;
  m_program = new OpenGLShaderProgram;
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, m_vertexFile);
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, m_fragmentFile);
  m_program->link();
}

void OpenGLDebugGroup::bind()
{
  m_program->bind();
}

void OpenGLDebugGroup::write(KDebugVertex *&dest) const
{
  KDebugVertexContainer::const_iterator begin = m_vertices.cbegin();
  KDebugVertexContainer::const_iterator end = m_vertices.cend();
  while (begin != end)
  {
    *dest = *begin;
    ++begin;
    ++dest;
  }
}

void OpenGLDebugGroup::draw(GLsizei offset)
{
  GL::glDrawArrays(m_drawMode, offset, size());
}

void OpenGLDebugGroup::release()
{
  m_program->release();
}

void OpenGLDebugGroup::teardown()
{
  delete m_program;
}

void OpenGLDebugGroup::clear()
{
  m_vertices.clear();
}

GLsizei OpenGLDebugGroup::size() const
{
  return static_cast<GLsizei>(m_vertices.size());
}

OpenGLDebugGroup::KDebugVertexContainer &OpenGLDebugGroup::vertices()
{
  return m_vertices;
}

/*******************************************************************************
 * OpenGLDebugTextureGroup
 ******************************************************************************/

class OpenGLDebugTextureGroup : public OpenGLDebugGroup
{
public:
  typedef std::vector<OpenGLTexture*> OpenGLTextureContainer;
  OpenGLDebugTextureGroup(unsigned &idx, GLenum drawMode, char const *vertex, char const *fragment);
  virtual void create(unsigned idx);
  virtual void draw(GLsizei offset);
  virtual void clear();
  OpenGLTextureContainer &textures();
private:
  OpenGLTextureContainer m_textures;
};

OpenGLDebugTextureGroup::OpenGLDebugTextureGroup(unsigned &idx, GLenum drawMode, const char *vertex, const char *fragment) :
  OpenGLDebugGroup(idx, drawMode, vertex, fragment)
{
  // Intentionally Empty
}

void OpenGLDebugTextureGroup::create(unsigned idx)
{
  OpenGLDebugGroup::create(idx);
  m_program->bind();
  m_program->setUniformValue("textureData", 0);
  m_program->release();
}

void OpenGLDebugTextureGroup::draw(GLsizei offset)
{
  GL::glActiveTexture(GL_TEXTURE0);
  for (OpenGLTexture *texture : m_textures)
  {
    texture->bind();
    GL::glDrawArrays(m_drawMode, offset, 4);
    offset += 4;
  }
}

void OpenGLDebugTextureGroup::clear()
{
  m_textures.clear();
  OpenGLDebugGroup::clear();
}

OpenGLDebugTextureGroup::OpenGLTextureContainer &OpenGLDebugTextureGroup::textures()
{
  return m_textures;
}

/*******************************************************************************
 * OpenGLDebugGroup
 ******************************************************************************/
class OpenGLDebugGroups
{
public:
  OpenGLDebugGroups(const std::initializer_list<OpenGLAbstractDebugGroup*> &groups);
  void create();
  void write(KDebugVertex *dest);
  void draw();
  void destroy();
  void clear();
  GLsizei size() const;
  OpenGLAbstractDebugGroup &operator[](unsigned idx);
private:
  std::vector<OpenGLAbstractDebugGroup*> m_groups;
};

OpenGLDebugGroups::OpenGLDebugGroups(const std::initializer_list<OpenGLAbstractDebugGroup*> &groups)
{
  for (OpenGLAbstractDebugGroup *group : groups)
  {
    m_groups.push_back(group);
  }
}

void OpenGLDebugGroups::create()
{
  unsigned idx = 0;
  for (OpenGLAbstractDebugGroup *group : m_groups)
  {
    group->create(idx);
    ++idx;
  }
}

void OpenGLDebugGroups::write(KDebugVertex *dest)
{
  for (OpenGLAbstractDebugGroup *group : m_groups)
  {
    group->write(dest);
  }
}

void OpenGLDebugGroups::draw()
{
  GLsizei offset = 0;
  for (OpenGLAbstractDebugGroup *group : m_groups)
  {
    group->bind();
    group->draw(offset);
    group->release();
    offset += group->size();
  }
}

void OpenGLDebugGroups::destroy()
{
  for (OpenGLAbstractDebugGroup *group : m_groups)
  {
    group->teardown();
  }
}

void OpenGLDebugGroups::clear()
{
  for (OpenGLAbstractDebugGroup *group : m_groups)
  {
    group->clear();
  }
}

GLsizei OpenGLDebugGroups::size() const
{
  GLsizei rSize = 0;
  for (const OpenGLAbstractDebugGroup *group : m_groups)
  {
    rSize += group->size();
  }
  return rSize;
}

OpenGLAbstractDebugGroup &OpenGLDebugGroups::operator[](unsigned idx)
{
  return *m_groups[idx];
}

/*******************************************************************************
 * OpenGLDebugDrawPrivate
 ******************************************************************************/
static unsigned sg_idxLines, sg_idxPoints, sg_idxRectangles, sg_idxTextures;
static OpenGLDebugGroups sg_debugGroups =
{
  new OpenGLDebugGroup(sg_idxLines,           GL_LINES,         ":/resources/shaders/debug/world.vert",   ":/resources/shaders/debug/world.frag"  ),
  new OpenGLDebugGroup(sg_idxPoints,          GL_POINTS,        ":/resources/shaders/debug/world.vert",   ":/resources/shaders/debug/world.frag"  ),
  new OpenGLDebugGroup(sg_idxRectangles,      GL_TRIANGLES,     ":/resources/shaders/debug/screen.vert",  ":/resources/shaders/debug/screen.frag" ),
  new OpenGLDebugTextureGroup(sg_idxTextures, GL_TRIANGLE_FAN,  ":/resources/shaders/debug/texture.vert", ":/resources/shaders/debug/texture.frag")
};
#define G(name) sg_debugGroups[sg_idx##name]
#define V(name) static_cast<OpenGLDebugGroup&>(G(name)).vertices()
#define T(name) static_cast<OpenGLDebugTextureGroup&>(G(name)).textures()
OpenGLDynamicBuffer<KDebugVertex> sg_vertexBuffer;
OpenGLVertexArrayObject *sg_vertexArrayObject;

static KRectF normalize(const KRectF &rect)
{
  float x = rect.x() * 2.0f - 1.0f;
  float y = rect.y() * -2.0f + 1.0f;
  float w = rect.width() * 2.0f;
  float h = rect.height() * 2.0f;
  return KRectF(x, y, w, h);
}

/*******************************************************************************
 * OpenGLDebugDraw
 ******************************************************************************/
void OpenGLDebugDraw::initialize()
{
  // Create debug groups
  sg_debugGroups.create();

  // Create Vertex Array Object
  sg_vertexArrayObject = new OpenGLVertexArrayObject();
  sg_vertexArrayObject->create();
  sg_vertexArrayObject->bind();

  // Bind relevant information
  sg_vertexBuffer.create();
  sg_vertexBuffer.bind();
  sg_vertexBuffer.setUsagePattern(OpenGLBuffer::DynamicDraw);
  GL::glEnableVertexAttribArray(0);
  GL::glEnableVertexAttribArray(1);
  GL::glVertexAttribPointer(0, KDebugVertex::PositionTupleSize, GL_FLOAT, GL_FALSE, KDebugVertex::stride(), (void*)KDebugVertex::positionOffset());
  GL::glVertexAttribPointer(1, KDebugVertex::ColorTupleSize, GL_FLOAT, GL_FALSE, KDebugVertex::stride(), (void*)KDebugVertex::colorOffset());

  // Release (unbind) all
  sg_vertexArrayObject->release();
}

void OpenGLDebugDraw::draw()
{
  sg_vertexBuffer.bind();

  // Send data to GPU
  {
    OpenGLBuffer::RangeAccessFlags flags =
      OpenGLBuffer::RangeUnsynchronized   |
      OpenGLBuffer::RangeInvalidateBuffer |
      OpenGLBuffer::RangeWrite;
    GLsizei size = sg_debugGroups.size();

    sg_vertexBuffer.reserve(size);
    KDebugVertex *dest = sg_vertexBuffer.mapRange(0, size, flags);
    sg_debugGroups.write(dest);
    sg_vertexBuffer.unmap();
  }

  // Draw Data
  sg_vertexArrayObject->bind();
  {
    glDisable(GL_DEPTH_TEST);
    sg_debugGroups.draw();
    glEnable(GL_DEPTH_TEST);
  }
  sg_vertexArrayObject->release();

  // Clear data
  sg_debugGroups.clear();
}

void OpenGLDebugDraw::teardown()
{
  sg_vertexBuffer.destroy();
  sg_debugGroups.destroy();
  delete sg_vertexArrayObject;
}

/*******************************************************************************
 * OpenGLDebugDraw::Screen
 ******************************************************************************/
void OpenGLDebugDraw::Screen::drawRect(const KRectF &rect, const KColor &color)
{
  // Move coordinates to OpenGL NDC
  KRectF nRect = normalize(rect);

  // Create key vertex positions
  float x1 = nRect.x();
  float y1 = nRect.y();
  float x2 = nRect.x() + nRect.width();
  float y2 = nRect.y() - nRect.height();

  // Create vertices
  V(Rectangles).push_back(KDebugVertex( KVector3D(x1, y1, 0.0f), color ));
  V(Rectangles).push_back(KDebugVertex( KVector3D(x1, y2, 0.0f), color ));
  V(Rectangles).push_back(KDebugVertex( KVector3D(x2, y1, 0.0f), color ));
  V(Rectangles).push_back(KDebugVertex( KVector3D(x2, y1, 0.0f), color ));
  V(Rectangles).push_back(KDebugVertex( KVector3D(x1, y2, 0.0f), color ));
  V(Rectangles).push_back(KDebugVertex( KVector3D(x2, y2, 0.0f), color ));
}

void OpenGLDebugDraw::Screen::drawTexture(const KRectF &rect, OpenGLTexture &texture)
{
  // Move coordinates to OpenGL NDC
  KRectF nRect = normalize(rect);

  // Create key vertex positions
  float x1 = nRect.x();
  float y1 = nRect.y();
  float x2 = nRect.x() + nRect.width();
  float y2 = nRect.y() - nRect.height();

  // Create vertices
  V(Textures).push_back(KDebugVertex( KVector3D(x1, y1, 0.0f), KVector2D(0.0f, 1.0f) ));
  V(Textures).push_back(KDebugVertex( KVector3D(x1, y2, 0.0f), KVector2D(0.0f, 0.0f) ));
  V(Textures).push_back(KDebugVertex( KVector3D(x2, y2, 0.0f), KVector2D(1.0f, 0.0f) ));
  V(Textures).push_back(KDebugVertex( KVector3D(x2, y1, 0.0f), KVector2D(1.0f, 1.0f) ));
  T(Textures).push_back(&texture);
}

/*******************************************************************************
 * OpenGLDebugDraw::World
 ******************************************************************************/
void OpenGLDebugDraw::World::drawPoint(const KVector3D &point, const KColor &color)
{
  V(Points).push_back(KDebugVertex(point, color));
}

void OpenGLDebugDraw::World::drawOval(const KVector3D &center, const KVector3D &normal, const KVector3D &up, float upRadius, float rightRadius, const KColor &color)
{
  OpenGLDebugDraw::World::drawOval(center, normal, up, upRadius, rightRadius, 25, color);
}

void OpenGLDebugDraw::World::drawOval(const KVector3D &center, const KVector3D &normal, const KVector3D &up, float upRadius, float rightRadius, int segments, const KColor &color)
{
  if (segments < 4) segments = 4;

  // Find orientation of circle based on normal
  KVector3D x_axis = KVector3D::crossProduct(up, normal).normalized();
  KVector3D z_axis = KVector3D::crossProduct(normal, x_axis).normalized();

  // Precompute delta values for rotation.
  float theta = 2.0f * 3.1415926f / float(segments);
  float cosine = std::cos(theta);
  float sine = std::sin(theta);

  // Step values
  float t;
  float x = 1.0f;
  float z = 0.0f;

  // Create circle
  KVector3D point = x_axis * x * rightRadius;
  for (int i = 0; i < segments; ++i)
  {
    V(Lines).push_back(KDebugVertex(center + point, color));
    t = x;
    x = cosine * x - sine * z;
    z = sine * t + cosine * z;
    point = z_axis * z * upRadius + x_axis * x * rightRadius;
    V(Lines).push_back(KDebugVertex(center + point, color));
  }
}

void OpenGLDebugDraw::World::drawObb(const KVector3D &center, const KMatrix3x3 &eigen, const KVector3D &halfLengths, const KColor &color)
{
  KVector3D axes[3];
  Karma::decomposeMatrixeByColumnVectors(eigen, axes);
  axes[0] *= halfLengths.x();
  axes[1] *= halfLengths.y();
  axes[2] *= halfLengths.z();
  KVector3D frontA  = center + axes[0] + axes[1] + axes[2];
  KVector3D frontB  = center + axes[0] + axes[1] - axes[2];
  KVector3D frontC  = center + axes[0] - axes[1] - axes[2];
  KVector3D frontD  = center + axes[0] - axes[1] + axes[2];
  KVector3D backA   = center - axes[0] + axes[1] + axes[2];
  KVector3D backD   = center - axes[0] - axes[1] + axes[2];
  KVector3D backB   = center - axes[0] + axes[1] - axes[2];
  KVector3D backC   = center - axes[0] - axes[1] - axes[2];
  drawLine(frontA, frontB, color);
  drawLine(frontB, frontC, color);
  drawLine(frontC, frontD, color);
  drawLine(frontD, frontA, color);
  drawLine(backA, backB, color);
  drawLine(backB, backC, color);
  drawLine(backC, backD, color);
  drawLine(backD, backA, color);
  drawLine(frontA, backA, color);
  drawLine(frontB, backB, color);
  drawLine(frontC, backC, color);
  drawLine(frontD, backD, color);
}

void OpenGLDebugDraw::World::drawCircle(const KVector3D &center, const KVector3D &normal, float radius, const KColor &color)
{
  OpenGLDebugDraw::World::drawCircle(center, normal, radius, 25, color);
}

void OpenGLDebugDraw::World::drawCircle(const KVector3D &center, const KVector3D &normal, float radius, int segments, const KColor &color)
{
  if (segments < 4) segments = 4;

  // Find orientation of circle based on normal
  KVector3D binormal;
  if (normal.y() > 0.0f)
    binormal = KVector3D(1.0f, 0.0f, 0.0f);
  else
    binormal = KVector3D(0.0f, 1.0f, 0.0f);
  KVector3D x_axis = KVector3D::crossProduct(normal, binormal).normalized();
  KVector3D z_axis = KVector3D::crossProduct(normal, x_axis).normalized();

  // Precompute delta values for rotation.
  float theta = 2.0f * 3.1415926f / float(segments);
  float cosine = std::cos(theta);
  float sine = std::sin(theta);

  // Step values
  float t;
  float x = radius;
  float z = 0.0f;

  // Create circle
  KVector3D point = x_axis * x;
  for (int i = 0; i < segments; ++i)
  {
    V(Lines).push_back(KDebugVertex(center + point, color));
    t = x;
    x = cosine * x - sine * z;
    z = sine * t + cosine * z;
    point = z_axis * z + x_axis * x;
    V(Lines).push_back(KDebugVertex(center + point, color));
  }
}

void OpenGLDebugDraw::World::drawSphere(const KVector3D &center, float radius, const KColor &color)
{
  drawSphere(center, radius, 12, 8, color);
}

void OpenGLDebugDraw::World::drawSphere(const KVector3D &center, float radius, int segments, int rings, const KColor &color)
{
  drawSphere(center, radius, segments, rings, int(std::min(2.0f * radius * rings, 128.0f)), color);
}

void OpenGLDebugDraw::World::drawSphere(const KVector3D &center, float radius, int segments, int rings, int subdivisions, const KColor &color)
{
  static const KVector3D upAxis(0.0f, 1.0f, 0.0f);
  float angle;

  // Bounds checking
  if (segments <= 1) segments = 1;
  if (rings <= 1) rings = 1;
  float segmentAngle = 3.1415926f / float(segments);
  float ringAngle = 3.1415926f / float(rings + 1);

  // Longitude (segments)
  angle = 0.0f;
  for (int i = 0; i < segments; ++i)
  {
    OpenGLDebugDraw::World::drawCircle(center, KVector3D(std::cos(angle), 0.0f, std::sin(angle)), radius, subdivisions, color);
    angle += segmentAngle;
  }

  // Latitude (rings)
  angle = ringAngle;
  for (int i = 0; i < rings; ++i)
  {
    drawCircle(center + radius * std::cos(angle) * upAxis, upAxis, radius * std::sin(angle), subdivisions, color);
    angle += ringAngle;
  }
}

void OpenGLDebugDraw::World::drawAabb(const KVector3D &frontA, const KVector3D &backC, const KColor &color)
{
  KVector3D frontB  = KVector3D( backC.x(), frontA.y(), frontA.z());
  KVector3D frontC  = KVector3D( backC.x(),  backC.y(), frontA.z());
  KVector3D frontD  = KVector3D(frontA.x(),  backC.y(), frontA.z());
  KVector3D backD   = KVector3D(frontA.x(),  backC.y(),  backC.z());
  KVector3D backA   = KVector3D(frontA.x(), frontA.y(),  backC.z());
  KVector3D backB   = KVector3D( backC.x(), frontA.y(),  backC.z());
  drawLine(frontA, frontB, color);
  drawLine(frontB, frontC, color);
  drawLine(frontC, frontD, color);
  drawLine(frontD, frontA, color);
  drawLine(backA, backB, color);
  drawLine(backB, backC, color);
  drawLine(backC, backD, color);
  drawLine(backD, backA, color);
  drawLine(frontA, backA, color);
  drawLine(frontB, backB, color);
  drawLine(frontC, backC, color);
  drawLine(frontD, backD, color);
}

void OpenGLDebugDraw::World::drawLine(const KVector3D &from, const KVector3D &to, const KColor &color)
{
  // Create vertices
  V(Lines).push_back(KDebugVertex(from, color));
  V(Lines).push_back(KDebugVertex(to, color));
}
