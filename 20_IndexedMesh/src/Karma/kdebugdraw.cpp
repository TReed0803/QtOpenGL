#include "kdebugdraw.h"

#include <vector>
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLShaderProgram>
#include <OpenGLVertexArrayObject>
#include <KCamera3D>
#include <KRectF>
#include <KTransform3D>
#include <KVertex>

/*******************************************************************************
 * KDebugDrawPrivate
 ******************************************************************************/
static std::vector<KVertex> sg_lines;
static std::vector<KVertex> sg_rectangles;

static size_t sg_bufferSize = 0;
static OpenGLFunctions f;
static OpenGLBuffer *sg_debugBuffer = Q_NULLPTR;
static OpenGLVertexArrayObject *sg_vertexArrayObject = Q_NULLPTR;
static OpenGLShaderProgram *sg_program = Q_NULLPTR;

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
#include <QOpenGLFunctions>
/*******************************************************************************
 * KDebugDraw
 ******************************************************************************/
void KDebugDraw::initialize()
{
  f.initializeOpenGLFunctions();

  // Create shaders
  sg_program = new OpenGLShaderProgram();
  sg_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/debug.vert");
  sg_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/debug.frag");
  sg_program->link();

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

void KDebugDraw::draw()
{
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
    sg_debugBuffer->bind();
    sg_debugBuffer->write(0, sg_lines.data(), sizeof(KVertex) * sg_lines.size());
    sg_debugBuffer->write(sizeof(KVertex) * sg_lines.size(), sg_rectangles.data(), sizeof(KVertex) * sg_rectangles.size());
    sg_debugBuffer->release();
  }

  // Draw Data
  sg_vertexArrayObject->bind();
  {
    sg_program->bind();
    f.glDrawArrays(GL_LINES, 0, sg_lines.size());
    f.glDrawArrays(GL_TRIANGLES, sg_lines.size(), sg_rectangles.size());
    sg_program->release();
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
  delete sg_program;
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
