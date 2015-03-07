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
  const OpenGLUniformBufferObject *m_buffer;
  std::vector<OpenGLShaderProgramPair> m_programs;
};

OpenGLUniformBufferMapping::OpenGLUniformBufferMapping() :
  m_buffer(0)
{
  // Intentionally Empty
}

typedef std::unordered_map<std::string, OpenGLUniformBufferMapping> UniformBufferMap;
static UniformBufferMap m_staticUniformBufferMap;

static OpenGLUniformBufferMapping &resolveMapping(const std::string &name)
{
  return m_staticUniformBufferMap[name];
}

void OpenGLUniformBufferManager::setBindingIndex(const std::string &name, const OpenGLUniformBufferObject &ubo)
{
  OpenGLUniformBufferMapping &mapping = resolveMapping(name);
  if (mapping.m_buffer != &ubo)
  {
    mapping.m_buffer = &ubo;
    for (OpenGLShaderProgramPair &pair : mapping.m_programs)
    {
      pair.m_program->scheduleUniformUpdate(pair.m_uniformLocation, ubo);
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
  if (mapping.m_buffer)
  {
    program.scheduleUniformUpdate(location, *mapping.m_buffer);
  }
}
