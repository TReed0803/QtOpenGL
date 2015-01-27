#ifndef OPENGLCOMMON_H
#define OPENGLCOMMON_H

#include "macros.h"
#include <OpenGLError>

#ifdef WIN32
# define GL_REPORT(type,...) APPEND(GL_REPORT_I(type,__VA_ARGS__),)
# define GL_REPORT_I(type,func,...) { OpenGLError ev(STR(type), STR(func), OpenGLError::ErrorOn_##func); OpenGLError::sendEvent(&ev); }
#else
# define GL_REPORT(type,func,...) { OpenGLError ev(STR(type), STR(func), OpenGLError::ErrorOn_##func); OpenGLError::sendEvent(&ev); }
#endif

// Accept declarations
#define GL_DECL_1(fn) inline bool fn()
#define GL_DECL_2(fn,t0,...) inline bool fn(PDECL_1(t0))
#define GL_DECL_3(fn,t0,t1,...) inline bool fn(PDECL_2(t0,t1))
#define GL_DECL_4(fn,t0,t1,t2,...) inline bool fn(PDECL_3(t0,t1,t2))
#define GL_DECL_5(fn,t0,t1,t2,t3,...) inline bool fn(PDECL_4(t0,t1,t2,t3))

// Accept callings
#define GL_CALL_1(caller,fn) caller::fn()
#define GL_CALL_2(caller,fn,...) caller::fn(PCALL_1(__VA_ARGS__))
#define GL_CALL_3(caller,fn,...) caller::fn(PCALL_2(__VA_ARGS__))
#define GL_CALL_4(caller,fn,...) caller::fn(PCALL_3(__VA_ARGS__))
#define GL_CALL_5(caller,fn,...) caller::fn(PCALL_4(__VA_ARGS__))

// Both
#ifdef WIN32
# define GL_DECL(...) GL_DECL_I(NARGS(__VA_ARGS__),__VA_ARGS__)
# define GL_DECL_I(n,...) APPEND(APPEND(GL_DECL_,n)(__VA_ARGS__),)
# define GL_CALL(caller,...) GL_CALL_I(NARGS(__VA_ARGS__),caller,__VA_ARGS__)
# define GL_CALL_I(n,caller,...) APPEND(APPEND(GL_CALL_,n)(caller,__VA_ARGS__),)
#else
# define GL_DECL(...) APPEND(GL_DECL_,NARGS(__VA_ARGS__)(__VA_ARGS__))
# define GL_CALL(caller,...) APPEND(GL_CALL_,NARGS(__VA_ARGS__)(caller,__VA_ARGS__))
#endif

// Definition
#define GL_CHECK(caller,...) GL_DECL(__VA_ARGS__) { if(!GL_CALL(caller,__VA_ARGS__)) { GL_REPORT(caller,__VA_ARGS__); return false; } return true; }

// Typedef
#define GL_TYPEDEF_0(name) typedef APPEND(Q,name) APPEND(_,APPEND(name,0))
#define GL_TYPEDEF_1(name) typedef APPEND(name,Checked) APPEND(_,APPEND(name,1))
#define GL_TYPEDEF(name,n) APPEND(GL_TYPEDEF_,n)(name)

#ifdef GL_DEBUG
#define GL_FINALIZE(cclass,subclass) class cclass : public APPEND(cclass,subclass) { }
#define GL_QFINALIZE(cclass,subclass) class cclass : public APPEND(cclass,subclass) { public: explicit cclass(QObject*p=0) : APPEND(cclass,subclass)(p) {} }
#else
#define GL_FINALIZE(cclass,subclass) class cclass : public APPEND(Q,cclass) {}
#define GL_QFINALIZE(cclass,subclass) class cclass : public APPEND(Q,cclass) { public: explicit cclass(QObject*p=0) : APPEND(Q,cclass)(p) {} }
#endif

#endif // OPENGLCOMMON_H

