#ifndef OPENGLCOMMON_H
#define OPENGLCOMMON_H

#include "../macros.h"
#include <OpenGLError>
#include <OpenGLMarkerScoped>

// Report error statement
#define GL_REPORT_STMT(type,func,...) { OpenGLError ev(STR(type), STR(func), OpenGLError::type, OpenGLError::func); OpenGLError::sendEvent(&ev); }
#ifdef WIN32
# define GL_REPORT(type,func,...) APPEND(GL_REPORT_STMT(type,func,__VA_ARGS__),)
#else
# define GL_REPORT(type,func,...) GL_REPORT_STMT(type,func,__VA_ARGS__)
#endif

// Accept declarations
#define GL_DECL_1(caller,fn) inline decltype(((caller*)0)->fn()) fn()
#define GL_DECL_2(caller,fn,t0,...) inline decltype(((caller*)0)->fn(PCAST_1(t0))) fn(PDECL_1(t0))
#define GL_DECL_3(caller,fn,t0,t1,...) inline decltype(((caller*)0)->fn(PCAST_2(t0,t1))) fn(PDECL_2(t0,t1))
#define GL_DECL_4(caller,fn,t0,t1,t2,...) inline decltype(((caller*)0)->fn(PCAST_3(t0,t1,t2))) fn(PDECL_3(t0,t1,t2))
#define GL_DECL_5(caller,fn,t0,t1,t2,t3,...) inline decltype(((caller*)0)->fn(PCAST_4(t0,t1,t2,t3))) fn(PDECL_4(t0,t1,t2,t3))
#define GL_DECL_6(caller,fn,t0,t1,t2,t3,t4,...) inline decltype(((caller*)0)->fn(PCAST_5(t0,t1,t2,t3,t4))) fn(PDECL_5(t0,t1,t2,t3,t4))
#define GL_DECL_7(caller,fn,t0,t1,t2,t3,t4,t5,...) inline decltype(((caller*)0)->fn(PCAST_6(t0,t1,t2,t3,t4,t5))) fn(PDECL_6(t0,t1,t2,t3,t4,t5))
#define GL_DECL_8(caller,fn,t0,t1,t2,t3,t4,t5,t6,...) inline decltype(((caller*)0)->fn(PCAST_7(t0,t1,t2,t3,t4,t5,t6))) fn(PDECL_7(t0,t1,t2,t3,t4,t5,t6))
#define GL_DECL_9(caller,fn,t0,t1,t2,t3,t4,t5,t6,t7,...) inline decltype(((caller*)0)->fn(PCAST_8(t0,t1,t1,t2,t3,t4,t5,t6,t7))) fn(PDECL_8(t0,t1,t2,t3,t4,t5,t6,t7))
#define GL_DECL_10(caller,fn,t0,t1,t2,t3,t4,t5,t6,t7,t8,...) inline decltype(((caller*)0)->fn(PCAST_9(t0,t1,t2,t3,t4,t5,t6,t7,t8))) fn(PDECL_9(t0,t1,t2,t3,t4,t5,t6,t7,t8))
#define GL_DECL_11(caller,fn,t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,...) inline decltype(((caller*)0)->fn(PCAST_10(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9))) fn(PDECL_10(t0,t1,t2,t3,t4,t5,t6,t7,t8,t9))

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
# define GL_DECL(caller,...) GL_DECL_I(NARGS(__VA_ARGS__),caller,__VA_ARGS__)
# define GL_DECL_I(n,caller,...) APPEND(APPEND(GL_DECL_,n)(caller,__VA_ARGS__),)
# define GL_CALL(caller,...) GL_CALL_I(NARGS(__VA_ARGS__),caller,__VA_ARGS__)
# define GL_CALL_I(n,caller,...) APPEND(APPEND(GL_CALL_,n)(caller,__VA_ARGS__),)
#else
# define GL_DECL(caller,...) APPEND(GL_DECL_,NARGS(__VA_ARGS__)(caller,__VA_ARGS__))
# define GL_CALL(caller,...) APPEND(GL_CALL_,NARGS(__VA_ARGS__)(caller,__VA_ARGS__))
#endif

// Entry points
#ifdef    GL_DEBUG
# define GL_CHECK(caller,...) GL_DECL(caller,__VA_ARGS__) { if(!GL_CALL(caller,__VA_ARGS__)) { GL_REPORT(caller,__VA_ARGS__); return false; } return true; }
# define GL_CHECK_CONST(caller,...) GL_DECL(caller,__VA_ARGS__) const { if(!GL_CALL(caller,__VA_ARGS__)) { GL_REPORT(caller,__VA_ARGS__); return false; } return true; }
# define GL_PROFILE(caller,...) GL_DECL(caller,__VA_ARGS__) { OpenGLMarkerScoped prf(STR(PGET_N(0,__VA_ARGS__))); return GL_CALL(caller,__VA_ARGS__); }
# define GL_PROFILE_CONST(caller,...) GL_DECL(caller,__VA_ARGS__) const { OpenGLMarkerScoped prf(STR(PGET_N(0,__VA_ARGS__))); return GL_CALL(caller,__VA_ARGS__); }
#else
# define GL_CHECK(caller,...)
# define GL_PROFILE(caller,...)
# define GL_PROFILE_CONST(caller,...)
#endif // GL_DEBUG

#endif // OPENGLCOMMON_H

