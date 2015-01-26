#ifndef OPENGLCOMMON_H
#define OPENGLCOMMON_H

#include "macros.h"
#include <OpenGLError>

#define _GL_REPORT(type,func,...) { OpenGLError ev(STR(type), STR(func), OpenGLError::ErrorOn_##func); OpenGLError::sendEvent(&ev); }
#define GL_REPORT(type,...) APPEND(_,GL_REPORT) CYCLE() (type,__VA_ARGS__)

// Accept declarations
#define GL_DECL_1(fn) inline bool fn()
#define GL_DECL_2(fn,...) inline bool fn(PDECL(__VA_ARGS__))
#define GL_DECL_3(fn,...) GL_DECL_2(fn,__VA_ARGS__)
#define GL_DECL_4(fn,...) GL_DECL_2(fn,__VA_ARGS__)
#define GL_DECL_5(fn,...) GL_DECL_2(fn,__VA_ARGS__)

// Accept callings
#define GL_CALL_1(caller,fn) caller::fn()
#define GL_CALL_2(caller,fn,...) caller::fn(PCALL(__VA_ARGS__))
#define GL_CALL_3(caller,fn,...) GL_CALL_2(caller,fn,__VA_ARGS__)
#define GL_CALL_4(caller,fn,...) GL_CALL_2(caller,fn,__VA_ARGS__)
#define GL_CALL_5(caller,fn,...) GL_CALL_2(caller,fn,__VA_ARGS__)

// Both
#define GL_DECL(...) APPEND CYCLE() (GL_DECL_,NARGS(__VA_ARGS__)) CYCLE() (__VA_ARGS__)
#define GL_CALL(caller,...) APPEND CYCLE() (GL_CALL_,NARGS(__VA_ARGS__)) CYCLE() (caller,__VA_ARGS__)

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

