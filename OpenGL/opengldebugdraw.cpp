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

/*******************************************************************************
 * OpenGLDebugDrawPrivate
 ******************************************************************************/
static std::vector<KVertex> sg_lines;
static std::vector<KVertex> sg_rectangles;

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
  size_t verts = sg_lines.size() + sg_rectangles.size();
  return verts * sizeof(KVertex);
}

/*******************************************************************************
 * OpenGLDebugDraw
 ******************************************************************************/
void OpenGLDebugDraw::initialize(OpenGLUniformBufferObject &ubo)
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
  sg_programWorld->bind();
  sg_programWorld->uniformBlockBinding("GlobalMatrices", ubo);
  sg_programWorld->release();

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
    sg_debugBuffer->release();
  }

  // Draw Data
  sg_vertexArrayObject->bind();
  {
    glDisable(GL_DEPTH_TEST);

    // Draw World Debug Information
    sg_programWorld->bind();
    f.glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(sg_lines.size()));
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
void OpenGLDebugDraw::World::drawLine(const KVector3D &from, const KVector3D &to, const KColor &color)
{
  // Create vertices
  sg_lines.push_back(KVertex(from, color));
  sg_lines.push_back(KVertex(to, color));
}
