#include "opengluniformbuffermanager.h"

#include <algorithm>
#include <unordered_map>
#include <OpenGLUniformBufferObject>
#include <OpenGLShaderProgram>

struct OpenGLShaderProgramPair
{
  OpenGLShaderProgramPair(unsigned location, OpenGLShaderProgram *program);
  unsigned m_uniformLocation;
  OpenGLShaderProgram *m_program;
};

OpenGLShaderProgramPair::OpenGLShaderProgramPair(unsigned location, OpenGLShaderProgram *program) :
  m_uniformLocation(location), m_program(program)
{
  // Intentionally Empty
}

class OpenGLUniformBufferMapping
{
public:
  OpenGLUniformBufferMapping();
  unsigned m_bufferIndex;
  std::vector<OpenGLShaderProgramPair> m_programs;
};

OpenGLUniformBufferMapping::OpenGLUniformBufferMapping() :
  m_bufferIndex(0)
{
  // Intentionally Empty
}

typedef std::unordered_map<std::string, OpenGLUniformBufferMapping> UniformBufferMap;
static UniformBufferMap m_staticUniformBufferMap;

static OpenGLUniformBufferMapping &resolveMapping(const std::string &name)
{
  return m_staticUniformBufferMap[name];
}

void OpenGLUniformBufferManager::setBindingIndex(const std::string &name, unsigned index)
{
  OpenGLUniformBufferMapping &mapping = resolveMapping(name);
  if (mapping.m_bufferIndex != index)
  {
    mapping.m_bufferIndex = index;
    for (OpenGLShaderProgramPair &pair : mapping.m_programs)
    {
      pair.m_program->scheduleUniformUpdate(pair.m_uniformLocation, index);
    }
  }
}

void OpenGLUniformBufferManager::setBindingProgram(const std::string &name, unsigned location, OpenGLShaderProgram &program)
{
  OpenGLUniformBufferMapping &mapping = resolveMapping(name);
  for (OpenGLShaderProgramPair &pair : mapping.m_programs)
  {
    if (pair.m_program == &program) return;
  }
  mapping.m_programs.emplace_back(location, &program);

  // Only schedule if a uniform is bound to the manager
  if (mapping.m_bufferIndex)
  {
    program.scheduleUniformUpdate(location, mapping.m_bufferIndex);
  }
}
