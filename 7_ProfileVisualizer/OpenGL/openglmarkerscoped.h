#ifndef OPENGLMARKERSCOPE_H
#define OPENGLMARKERSCOPE_H

#include "profiler.h"

class OpenGLMarkerScoped
{
public:
  OpenGLMarkerScoped(const char *name);
  ~OpenGLMarkerScoped();
};

inline OpenGLMarkerScoped::OpenGLMarkerScoped(const char *name) { PROFILER_PUSH_GPU_MARKER(name); }
inline OpenGLMarkerScoped::~OpenGLMarkerScoped() { PROFILER_POP_GPU_MARKER(); }

#endif // OPENGLMARKERSCOPE_H

