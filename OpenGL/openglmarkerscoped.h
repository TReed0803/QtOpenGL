#ifndef OPENGLMARKERSCOPE_H
#define OPENGLMARKERSCOPE_H OpenGLMarkerScoped

#include <OpenGLProfiler>

class OpenGLMarkerScoped
{
public:
  OpenGLMarkerScoped(const char *name);
  ~OpenGLMarkerScoped();
};

inline OpenGLMarkerScoped::OpenGLMarkerScoped(const char *name) { OpenGLProfiler::PushGpuMarker(name); }
inline OpenGLMarkerScoped::~OpenGLMarkerScoped() { OpenGLProfiler::PopGpuMarker(); }

#endif // OPENGLMARKERSCOPE_H

