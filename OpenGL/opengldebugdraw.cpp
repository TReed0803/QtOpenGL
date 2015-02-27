#include "opengldebugdraw.h"

#include <vector>
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLShaderProgram>
#include <OpenGLVertexArrayObject>
#include <KCamera3D>
#include <KRectF>
#include <KTransform3D>
#include <KVertex>
#include <KMath>

/*******************************************************************************
 * OpenGLDebugDrawPrivate
 ******************************************************************************/
static std::vector<KVertex> sg_lines;
static std::vector<KVertex> sg_rectangles;
static std::vector<KVertex> sg_points;

static size_t sg_bufferSize = 0;
static OpenGLFunctions f;
static OpenGLBuffer *sg_debugBuffer = Q_NULLPTR;
static OpenGLVertexArrayObject *sg_vertexArrayObject = Q_NULLPTR;
static OpenGLShaderProgram *sg_programScreen = Q_NULLPTR;
static OpenGLShaderProgram *sg_programWorld = Q_NULLPTR;

static KRectF normalize(const KRectF &rect)
{
  float x = rect.x() * 2.0f - 1.0f;
  float y = rect.y() * -2.0f + 1.0f;
  float w = rect.width() * 2.0f;
  float h = rect.height() * 2.0f;
  return KRectF(x, y, w, h);
}

static size_t requiredSize()
{
  size_t verts = sg_lines.size() + sg_rectangles.size() + sg_points.size();
  return verts * sizeof(KVertex);
}

/*******************************************************************************
 * OpenGLDebugDraw
 ******************************************************************************/
void OpenGLDebugDraw::initialize()
{
  f.initializeOpenGLFunctions();

  // Create shaders
  sg_programScreen = new OpenGLShaderProgram();
  sg_programScreen->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/debug/screen.vert");
  sg_programScreen->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/debug/screen.frag");
  sg_programScreen->link();
  sg_programWorld = new OpenGLShaderProgram();
  sg_programWorld->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/debug/world.vert");
  sg_programWorld->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/debug/world.frag");
  sg_programWorld->link();

  // Create Vertex Array Object
  sg_vertexArrayObject = new OpenGLVertexArrayObject();
  sg_vertexArrayObject->create();
  sg_vertexArrayObject->bind();

  sg_debugBuffer = new OpenGLBuffer();
  sg_debugBuffer->create();
  sg_debugBuffer->bind();
  sg_debugBuffer->setUsagePattern(OpenGLBuffer::DynamicDraw);
  f.glEnableVertexAttribArray(0);
  f.glEnableVertexAttribArray(1);
  f.glVertexAttribPointer(0, KVertex::PositionTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::positionOffset());
  f.glVertexAttribPointer(1, KVertex::ColorTupleSize, GL_FLOAT, GL_FALSE, KVertex::stride(), (void*)KVertex::colorOffset());

  // Release (unbind) all
  sg_vertexArrayObject->release();
}

void OpenGLDebugDraw::draw()
{
  // Early-out if there is nothing to draw
  size_t required = requiredSize();
  if (required == 0) return;

  // Prepare OpenGL Buffer data
  if (sg_bufferSize < required)
  {
    sg_debugBuffer->bind();
    sg_debugBuffer->allocate(static_cast<int>(required));
    sg_debugBuffer->release();
    sg_bufferSize = required;
  }

  // Send data to GPU
  {
    sg_debugBuffer->bind();
    sg_debugBuffer->write(0, sg_lines.data(), static_cast<int>(sizeof(KVertex) * sg_lines.size()));
    sg_debugBuffer->write(static_cast<int>(sizeof(KVertex) * sg_lines.size()), sg_rectangles.data(), static_cast<int>(sizeof(KVertex) * sg_rectangles.size()));
    sg_debugBuffer->write(static_cast<int>(sizeof(KVertex) * (sg_lines.size() + sg_rectangles.size())), sg_points.data(), static_cast<int>(sizeof(KVertex) * sg_points.size()));
    sg_debugBuffer->release();
  }

  // Draw Data
  sg_vertexArrayObject->bind();
  {
    glDisable(GL_DEPTH_TEST);

    // Draw World Debug Information
    sg_programWorld->bind();
    f.glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(sg_lines.size()));
    f.glDrawArrays(GL_POINTS, static_cast<GLsizei>(sg_lines.size() + sg_rectangles.size()), static_cast<GLsizei>(sg_points.size()));
    sg_programWorld->release();

    // Draw Screen Debug Information
    sg_programScreen->bind();
    f.glDrawArrays(GL_TRIANGLES, static_cast<GLsizei>(sg_lines.size()), static_cast<int>(sg_rectangles.size()));
    sg_programScreen->release();

    glEnable(GL_DEPTH_TEST);
  }
  sg_vertexArrayObject->release();

  // Clear data
  sg_lines.clear();
  sg_rectangles.clear();
  sg_points.clear();
}

void OpenGLDebugDraw::teardown()
{
  delete sg_debugBuffer;
  delete sg_vertexArrayObject;
  delete sg_programScreen;
  delete sg_programWorld;
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
  sg_rectangles.push_back(KVertex( KVector3D(x1, y1, 0.0f), color ));
  sg_rectangles.push_back(KVertex( KVector3D(x1, y2, 0.0f), color ));
  sg_rectangles.push_back(KVertex( KVector3D(x2, y1, 0.0f), color ));
  sg_rectangles.push_back(KVertex( KVector3D(x2, y1, 0.0f), color ));
  sg_rectangles.push_back(KVertex( KVector3D(x1, y2, 0.0f), color ));
  sg_rectangles.push_back(KVertex( KVector3D(x2, y2, 0.0f), color ));
}

/*******************************************************************************
 * OpenGLDebugDraw::World
 ******************************************************************************/
void OpenGLDebugDraw::World::drawPoint(const KVector3D &point, const KColor &color)
{
  sg_points.push_back(KVertex(point, color));
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
    sg_lines.push_back(KVertex(center + point, color));
    t = x;
    x = cosine * x - sine * z;
    z = sine * t + cosine * z;
    point = z_axis * z * upRadius + x_axis * x * rightRadius;
    sg_lines.push_back(KVertex(center + point, color));
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
    sg_lines.push_back(KVertex(center + point, color));
    t = x;
    x = cosine * x - sine * z;
    z = sine * t + cosine * z;
    point = z_axis * z + x_axis * x;
    sg_lines.push_back(KVertex(center + point, color));
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
  sg_lines.push_back(KVertex(from, color));
  sg_lines.push_back(KVertex(to, color));
}
