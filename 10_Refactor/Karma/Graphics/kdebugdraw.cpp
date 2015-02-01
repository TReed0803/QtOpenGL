#include "kdebugdraw.h"

#include <vector>
#include <OpenGLShaderProgram>
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLVertexArrayObject>
#include <KVertex>
#include <KRectF>
#include <KCamera3D>
#include <KTransform3D>

/*******************************************************************************
 * KDebugDrawPrivate
 ******************************************************************************/
static std::vector<KVertex> sg_lines;
static std::vector<KVertex> sg_rectangles;

static size_t sg_bufferSize = 0;
static int su_worldToCamera = 0;
static int su_cameraToView = 0;
static OpenGLBuffer *sg_debugBuffer = Q_NULLPTR;
static OpenGLVertexArrayObject *sg_vertexArrayObject = Q_NULLPTR;
static OpenGLShaderProgram *sg_screenProgram = Q_NULLPTR;
static OpenGLShaderProgram *sg_worldProgram = Q_NULLPTR;

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
  size_t verts = sg_lines.size() + sg_rectangles.size();
  return verts * sizeof(KVertex);
}

/*******************************************************************************
 * KDebugDraw
 ******************************************************************************/
void KDebugDraw::initialize()
{
  QOpenGLContext *ctx = QOpenGLContext::currentContext();
  OpenGLFunctions f(ctx);

  // Create shaders
  sg_screenProgram = new OpenGLShaderProgram();
  sg_screenProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/debugScreen.vert");
  sg_screenProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/debugScreen.frag");
  sg_screenProgram->link();
  sg_worldProgram = new OpenGLShaderProgram();
  sg_worldProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/debugWorld.vert");
  sg_worldProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/debugWorld.frag");
  sg_worldProgram->link();

  // Cache uniform locations
  su_worldToCamera = sg_worldProgram->uniformLocation("worldToCamera");
  su_cameraToView = sg_worldProgram->uniformLocation("cameraToView");

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
  sg_worldProgram->release();
}

void KDebugDraw::draw(KCamera3D &camera, KMatrix4x4 const &view)
{
  QOpenGLContext *ctx = QOpenGLContext::currentContext();
  OpenGLFunctions f(ctx);

  // Early-out if there is nothing to draw
  size_t required = requiredSize();
  if (required == 0) return;

  // Prepare OpenGL Buffer data
  if (sg_bufferSize < required)
  {
    sg_debugBuffer->bind();
    sg_debugBuffer->allocate(required);
    sg_debugBuffer->release();
    sg_bufferSize = required;
  }

  // Send data to GPU
  {
    size_t idx = 0;
    sg_debugBuffer->bind();
    KVertex *data = static_cast<KVertex*>(sg_debugBuffer->map(OpenGLBuffer::WriteOnly));
    for (auto &v : sg_lines)
    {
      data[idx++] = v;
    }
    for (auto &v : sg_rectangles)
    {
      data[idx++] = v;
    }
    sg_debugBuffer->unmap();
    sg_debugBuffer->release();
  }

  // Draw Data
  sg_vertexArrayObject->bind();
  {
    // Screen
    sg_screenProgram->bind();
    f.glDrawArrays(GL_LINES, 0, sg_lines.size());
    sg_screenProgram->release();
    // World
    sg_worldProgram->bind();
    sg_worldProgram->setUniformValue(su_worldToCamera, camera.toMatrix());
    sg_worldProgram->setUniformValue(su_cameraToView, view);
    f.glDrawArrays(GL_TRIANGLES, sg_lines.size(), sg_rectangles.size());
    sg_worldProgram->release();

  }
  sg_vertexArrayObject->release();

  // Clear data
  sg_lines.clear();
  sg_rectangles.clear();
}

void KDebugDraw::teardown()
{
  delete sg_debugBuffer;
  delete sg_vertexArrayObject;
  delete sg_screenProgram;
  delete sg_worldProgram;
}

/*******************************************************************************
 * KDebugDraw::Screen
 ******************************************************************************/
void KDebugDraw::Screen::drawRect(const KRectF &rect, const KColor &color)
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
 * KDebugDraw::World
 ******************************************************************************/
void KDebugDraw::World::drawLine(const KVector3D &from, const KVector3D &to, const KColor &color)
{
  // Create vertices
  sg_lines.push_back(KVertex(from, color));
  sg_lines.push_back(KVertex(to, color));
}
