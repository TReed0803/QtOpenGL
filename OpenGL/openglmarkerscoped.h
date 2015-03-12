#ifndef OPENGLMARKERSCOPE_H
#define OPENGLMARKERSCOPE_H OpenGLMarkerScoped

#include <string>
#include <KString>
#include <OpenGLProfiler>

class OpenGLMarkerScoped
{
public:
  OpenGLMarkerScoped(const char *name);
  OpenGLMarkerScoped(const std::string &name);
  OpenGLMarkerScoped(const KString &name);
  ~OpenGLMarkerScoped();
};

inline OpenGLMarkerScoped::OpenGLMarkerScoped(const char *name) { OpenGLProfiler::PushGpuMarker(name); }
inline OpenGLMarkerScoped::OpenGLMarkerScoped(const KString &name) { OpenGLProfiler::PushGpuMarker(qPrintable(name)); }
inline OpenGLMarkerScoped::OpenGLMarkerScoped(const std::string &name) { OpenGLProfiler::PushGpuMarker(name.c_str()); }
inline OpenGLMarkerScoped::~OpenGLMarkerScoped() { OpenGLProfiler::PopGpuMarker(); }

#endif // OPENGLMARKERSCOPE_H

