#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include "qvertex.h"

// Front Verticies
#define QVERTEX_FTR QVertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define QVERTEX_FTL QVertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define QVERTEX_FBL QVertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define QVERTEX_FBR QVertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define QVERTEX_BTR QVertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define QVERTEX_BTL QVertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define QVERTEX_BBL QVertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define QVERTEX_BBR QVertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const QVertex sg_vertexes[] = {
  // Face 1 (Front)
    QVERTEX_FTR, QVERTEX_FTL, QVERTEX_FBL,
    QVERTEX_FBL, QVERTEX_FBR, QVERTEX_FTR,
  // Face 2 (Back)
    QVERTEX_BBR, QVERTEX_BTL, QVERTEX_BTR,
    QVERTEX_BTL, QVERTEX_BBR, QVERTEX_BBL,
  // Face 3 (Top)
    QVERTEX_FTR, QVERTEX_BTR, QVERTEX_BTL,
    QVERTEX_BTL, QVERTEX_FTL, QVERTEX_FTR,
  // Face 4 (Bottom)
    QVERTEX_FBR, QVERTEX_FBL, QVERTEX_BBL,
    QVERTEX_BBL, QVERTEX_BBR, QVERTEX_FBR,
  // Face 5 (Left)
    QVERTEX_FBL, QVERTEX_FTL, QVERTEX_BTL,
    QVERTEX_FBL, QVERTEX_BTL, QVERTEX_BBL,
  // Face 6 (Right)
    QVERTEX_FTR, QVERTEX_FBR, QVERTEX_BBR,
    QVERTEX_BBR, QVERTEX_BTR, QVERTEX_FTR
};

#undef QVERTEX_BBR
#undef QVERTEX_BBL
#undef QVERTEX_BTL
#undef QVERTEX_BTR

#undef QVERTEX_FBR
#undef QVERTEX_FBL
#undef QVERTEX_FTL
#undef QVERTEX_FTR

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

Window::Window()
{
  m_transform.translate(0.0f, 0.0f, -5.0f);
}

void Window::initializeGL()
{
  // Initialize OpenGL Backend
  initializeOpenGLFunctions();
  connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  printVersionInformation();

  // Set global information
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Application-specific initialization
  {
    // Create Shader (Do not release until VAO is created)
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    m_program->link();
    m_program->bind();

    // Cache Uniform Locations
    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    u_worldToView = m_program->uniformLocation("worldToView");

    // Create Buffer (Do not release until VAO is created)
    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

    // Create Vertex Array Object
    m_object.create();
    m_object.bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, QVertex::positionOffset(), QVertex::PositionTupleSize, QVertex::stride());
    m_program->setAttributeBuffer(1, GL_FLOAT, QVertex::colorOffset(), QVertex::ColorTupleSize, QVertex::stride());

    // Release (unbind) all
    m_object.release();
    m_vertex.release();
    m_program->release();
  }
}

void Window::resizeGL(int width, int height)
{
  m_projection.setToIdentity();
  m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

void Window::paintGL()
{
  // Clear
  glClear(GL_COLOR_BUFFER_BIT);

  // Render using our shader
  m_program->bind();
  m_program->setUniformValue(u_worldToView, m_projection);
  {
    m_object.bind();
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
    m_object.release();
  }
  m_program->release();
}

void Window::teardownGL()
{
  // Actually destroy our OpenGL information
  m_object.destroy();
  m_vertex.destroy();
  delete m_program;
}

void Window::update()
{
  // Update instance information
  m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

  // Schedule a redraw
  QOpenGLWindow::update();
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
