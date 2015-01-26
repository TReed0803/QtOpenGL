#ifdef    GL_DEBUG

sg_debug = true;

#define glProfile(func,...) PROFILER_PUSH_GPU_MARKER(#func); func(__VA_ARGS__); PROFILER_POP_GPU_MARKER()

// GL Functions
#define glClear(...) glProfile(glClear, __VA_ARGS__);
#define glDrawArrays(...) glProfile(glDrawArrays, __VA_ARGS__);

#endif // GL_DEBUG
