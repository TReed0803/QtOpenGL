#ifndef OPENGLCOMMON_H
#define OPENGLCOMMON_H

#include "macros.h"
#include <OpenGLError>

// Report error statement
#define GL_REPORT_STMT(type,func,...) { OpenGLError ev(STR(type), STR(func), OpenGLError::type, OpenGLError::func); OpenGLError::sendEvent(&ev); }
#ifdef WIN32
# define GL_REPORT(type,func,...) APPEND(GL_REPORT_STMT(type,func,__VA_ARGS__),)
#else
# define GL_REPORT(type,func,...) GL_REPORT_STMT(type,func,__VA_ARGS__)
#endif

// Accept declarations
#define GL_DECL_1(fn) inline bool fn()
#define GL_DECL_2(fn,t0,...) inline bool fn(PDECL_1(t0))
#define GL_DECL_3(fn,t0,t1,...) inline bool fn(PDECL_2(t0,t1))
#define GL_DECL_4(fn,t0,t1,t2,...) inline bool fn(PDECL_3(t0,t1,t2))
#define GL_DECL_5(fn,t0,t1,t2,t3,...) inline bool fn(PDECL_4(t0,t1,t2,t3))
#define GL_DECL_6(fn,t0,t1,t2,t3,t4,...) inline bool fn(PDECL_5(t0,t1,t2,t3,t4))
#define GL_DECL_7(fn,t0,t1,t2,t3,t4,t5,...) inline bool fn(PDECL_6(t0,t1,t2,t3,t4,t5))
#define GL_DECL_8(fn,t0,t1,t2,t3,t4,t5,t6,...) inline bool fn(PDECL_7(t0,t1,t2,t3,t4,t5,t6))
#define GL_DECL_9(fn,t0,t1,t2,t3,t4,t5,t6,t7,...) inline bool fn(PDECL_8(t0,t1,t2,t3,t4,t5,t6,t7))
#define GL_DECL_10(fn,t0,t1,t2,t3,t4,t5,t6,t7,t8,...) inline bool fn(PDECL_9(t0,t1,t2,t3,t4,t5,t6,t7,t8))
#define GL_DECL_11(fn,t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,...) inline bool fn(PDECL_10(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9))

// Accept callings
#define GL_CALL_1(caller,fn) caller::fn()
#define GL_CALL_2(caller,fn,...) caller::fn(PCALL_1())
#define GL_CALL_3(caller,fn,...) caller::fn(PCALL_2())
#define GL_CALL_4(caller,fn,...) caller::fn(PCALL_3())
#define GL_CALL_5(caller,fn,...) caller::fn(PCALL_4())
#define GL_CALL_6(caller,fn,...) caller::fn(PCALL_5())
#define GL_CALL_7(caller,fn,...) caller::fn(PCALL_6())
#define GL_CALL_8(caller,fn,...) caller::fn(PCALL_7())
#define GL_CALL_9(caller,fn,...) caller::fn(PCALL_8())
#define GL_CALL_10(caller,fn,...) caller::fn(PCALL_9())
#define GL_CALL_11(caller,fn,...) caller::fn(PCALL_10())

// Platform-specific entry points
#ifdef WIN32
# define GL_DECL(...) GL_DECL_I(NARGS(__VA_ARGS__),__VA_ARGS__)
# define GL_DECL_I(n,...) APPEND(APPEND(GL_DECL_,n)(__VA_ARGS__),)
# define GL_CALL(caller,...) GL_CALL_I(NARGS(__VA_ARGS__),caller,__VA_ARGS__)
# define GL_CALL_I(n,caller,...) APPEND(APPEND(GL_CALL_,n)(caller,__VA_ARGS__),)
#else
# define GL_DECL(...) APPEND(GL_DECL_,NARGS(__VA_ARGS__)(__VA_ARGS__))
# define GL_CALL(caller,...) APPEND(GL_CALL_,NARGS(__VA_ARGS__)(caller,__VA_ARGS__))
#endif

// Entry points
#ifdef    GL_DEBUG
# define GL_CHECK(caller,...) GL_DECL(__VA_ARGS__) { if(!GL_CALL(caller,__VA_ARGS__)) { GL_REPORT(caller,__VA_ARGS__); return false; } return true; }
#else
# define GL_CHECK(caller,...)
#endif // GL_DEBUG

#endif // OPENGLCOMMON_H

