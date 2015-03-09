#include "opengluniformbuffermanager.h"

#include <algorithm>
#include <unordered_map>
#include <OpenGLUniformBufferObject>
#include <OpenGLShaderProgram>

///////////////////////////////////////////////////////////////////////////
/// Helper Classes / Structs
///////////////////////////////////////////////////////////////////////////

template <typename T>
struct OpenGLShaderProgramPair
{
  typedef T ValueType;
  OpenGLShaderProgramPair(OpenGLShaderProgram *program, unsigned location);
  OpenGLShaderProgram *program;
  unsigned location;
};

template <typename T>
OpenGLShaderProgramPair<T>::OpenGLShaderProgramPair(OpenGLShaderProgram *p, unsigned l) :
  program(p), location(l)
{
  // Intentionally Empty
}

template <typename T>
struct OpenGLUniformMapping
{
  typedef T ValueType;
  typedef OpenGLShaderProgramPair<ValueType> PairType;
  ValueType value;
  std::vector<PairType> programs;
};

template <typename Mapper>
class OpenGLUniformMap : public std::unordered_map<std::string, Mapper>
{
  // Intentionally Empty
};

template <typename T>
static OpenGLUniformMapping<T> &resolveMapping(const std::string &name);

template <typename T>
static void scheduleUpdate(OpenGLShaderProgram *program, unsigned location, const T &value);

template <typename T>
static unsigned resolveLocation(OpenGLShaderProgram *program, const std::string &name);

template <typename T>
static bool validLocation(unsigned location);

template <size_t id, typename T>
struct OpenGLValue
{
  T internal;
  OpenGLValue() {}
  OpenGLValue(const T &v) : internal(v) {}
  bool operator==(const T &v) const { return internal == v; }
  bool operator!=(const T &v) const { return internal != v; }
  void operator=(const T &v) { internal = v; }
};

///////////////////////////////////////////////////////////////////////////
/// Uniform Buffers
///////////////////////////////////////////////////////////////////////////

typedef OpenGLValue<0, unsigned> OpenGLUniformBufferIndex;
typedef OpenGLUniformMapping<OpenGLUniformBufferIndex> OpenGLUniformBufferMapping;
typedef OpenGLUniformMap<OpenGLUniformBufferMapping> OpenGLUniformBufferMap;
static OpenGLUniformBufferMap sg_uniformBufferMap;

template <>
static OpenGLUniformMapping<OpenGLUniformBufferIndex> &resolveMapping<OpenGLUniformBufferIndex>(const std::string &name)
{
  return sg_uniformBufferMap[name];
}

template <>
static void scheduleUpdate<OpenGLUniformBufferIndex>(OpenGLShaderProgram *program, unsigned location, const OpenGLUniformBufferIndex &value)
{
  program->scheduleUniformBlockUpdate(location, value.internal);
}

template <>
static unsigned resolveLocation<OpenGLUniformBufferIndex>(OpenGLShaderProgram *program, const std::string &name)
{
  return program->uniformBlockLocation(name.c_str());
}

template <>
static bool validLocation<OpenGLUniformBufferIndex>(unsigned location)
{
  return location != OpenGLUniformBufferObject::InvalidLocation;
}

///////////////////////////////////////////////////////////////////////////
/// Texture Samplers
///////////////////////////////////////////////////////////////////////////

typedef OpenGLValue<1, unsigned> OpenGLTextureSampler;
typedef OpenGLUniformMapping<OpenGLTextureSampler> OpenGLTextureSamplerMapping;
typedef OpenGLUniformMap<OpenGLTextureSamplerMapping> OpenGLTextureSamplerMap;
static OpenGLTextureSamplerMap sg_textureSamplerMap;

template <>
static OpenGLUniformMapping<OpenGLTextureSampler> &resolveMapping<OpenGLTextureSampler>(const std::string &name)
{
  return sg_textureSamplerMap[name];
}

template <>
static void scheduleUpdate<OpenGLTextureSampler>(OpenGLShaderProgram *program, unsigned location, const OpenGLTextureSampler &value)
{
  program->scheduleUniformUpdate(location, value.internal);
}

template <>
static unsigned resolveLocation<OpenGLTextureSampler>(OpenGLShaderProgram *program, const std::string &name)
{
  return program->uniformLocation(name.c_str());
}

template <>
static bool validLocation<OpenGLTextureSampler>(unsigned location)
{
  return location != -1;
}

///////////////////////////////////////////////////////////////////////////
/// OpenGLUniformManager
///////////////////////////////////////////////////////////////////////////

template <typename T>
static void set(const std::string &name, unsigned value)
{
  auto &mapping = resolveMapping<T>(name);
  if (mapping.value != value)
  {
    mapping.value = value;
    for (auto &pair : mapping.programs)
    {
      scheduleUpdate<T>(pair.program, pair.location, value);
    }
  }
}

template <typename T>
static void registerCallback(const std::string &name, OpenGLShaderProgram &program)
{
  auto &mapping = resolveMapping<T>(name);
  for (auto &pair : mapping.programs)
  {
    if (pair.program == &program) return;
  }
  unsigned location = resolveLocation<T>(&program, name);
  if (validLocation<T>(location))
  {
    mapping.programs.emplace_back(&program, location);
    scheduleUpdate<T>(&program, location, mapping.value);
  }
}

void OpenGLUniformManager::setTextureSampler(const std::string &name, unsigned textureId)
{
  set<OpenGLTextureSampler>(name, textureId);
}

void OpenGLUniformManager::setUniformBufferIndex(const std::string &name, unsigned index)
{
  set<OpenGLUniformBufferIndex>(name, index);
}

void OpenGLUniformManager::registerTextureSamplerCallbacks(const std::string &name, OpenGLShaderProgram &program)
{
  registerCallback<OpenGLTextureSampler>(name, program);
}

void OpenGLUniformManager::registerUniformBufferCallbacks(const std::string &name, OpenGLShaderProgram &program)
{
  registerCallback<OpenGLUniformBufferIndex>(name, program);
}
