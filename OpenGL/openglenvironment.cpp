#include "openglenvironment.h"

#include <KBufferedBinaryFileReader>
#include <KMacros>
#include <OpenGLTexture>
#include <OpenGLHdrTexture>

class OpenGLEnvrionmentPrivate
{
public:
  OpenGLEnvrionmentPrivate();
  ~OpenGLEnvrionmentPrivate();
  bool m_dirty;
  OpenGLTexture m_directIllumination;
  OpenGLTexture m_indirectIllumination;
  OpenGLToneMappingFunction *m_toneMapping;
};

OpenGLEnvrionmentPrivate::OpenGLEnvrionmentPrivate() :
  m_dirty(false), m_toneMapping(0)
{
  // Intentionally Empty
}

OpenGLEnvrionmentPrivate::~OpenGLEnvrionmentPrivate()
{
  delete m_toneMapping;
}

OpenGLEnvironment::OpenGLEnvironment() :
  m_private(new OpenGLEnvrionmentPrivate)
{
  // Intentionally Empty
}

OpenGLEnvironment::~OpenGLEnvironment()
{
  delete m_private;
}

void OpenGLEnvironment::create()
{
  P(OpenGLEnvrionmentPrivate);
}

void OpenGLEnvironment::setDirect(const char *filePath)
{
  P(OpenGLEnvrionmentPrivate);
  KBufferedBinaryFileReader reader(filePath, 1024);
  OpenGLHdrTextureLoader loader(&reader, &p.m_directIllumination);
  loader.parse(p.m_toneMapping);
}

void OpenGLEnvironment::setIndirect(const char *filePath)
{
  P(OpenGLEnvrionmentPrivate);
  KBufferedBinaryFileReader reader(filePath, 1024);
  OpenGLHdrTextureLoader loader(&reader, &p.m_indirectIllumination);
  loader.parse(p.m_toneMapping);
}

void OpenGLEnvironment::setToneMappingFunction(OpenGLToneMappingFunction *fnc)
{
  P(OpenGLEnvrionmentPrivate);
  p.m_toneMapping = fnc;
}

OpenGLTexture &OpenGLEnvironment::direct()
{
  P(OpenGLEnvrionmentPrivate);
  return p.m_directIllumination;
}

OpenGLTexture &OpenGLEnvironment::indirect()
{
  P(OpenGLEnvrionmentPrivate);
  return p.m_indirectIllumination;
}

const KSize &OpenGLEnvironment::directSize() const
{
  P(const OpenGLEnvrionmentPrivate);
  return p.m_directIllumination.size();
}
