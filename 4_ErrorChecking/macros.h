#ifndef MACROS_H
#define MACROS_H

// Append two preprocessor arguments together
#define _APPEND(a,b) a##b
#define APPEND(a,b) _APPEND(a,b)

// Count number of arguments
#define _NARGS(_1,_2,_3,_4,_5,N,...) N
#define NARGS(...) APPEND(_NARGS(__VA_ARGS__,5,4,3,2,1),)

// Stringize Token
#define _STR(x) #x
#define STR(x) _STR(x)

// Get specific value from __VA_ARGS__
#define PGET_0(e0,...) e0
#define PGET_1(e0,e1,...) e1
#define PGET_2(e0,e1,e2,...) e2
#define PGET_3(e0,e1,e2,e3,...) e3
#define PGET_4(e0,e1,e2,e3,e4,...) e4
#ifdef MSVC_VER
# define PGET_N(n,...) PGET_I(n,__VA_ARGS__)
# define PGET_I(n,...) APPEND(APPEND(PGET_,n)(__VA_ARGS__),)
#else
# define PGET_N(n,...) APPEND(PGET_,n)(__VA_ARGS__)
#endif

// Declare variables using comma separator
#define PDECL_0(...)
#define PDECL_1(e0,...) e0 v0
#define PDECL_2(e0,e1,...) e0 v0, e1 v1
#define PDECL_3(e0,e1,e2,...) e0 v0, e1 v1, e2 v2
#define PDECL_4(e0,e1,e2,e3,...) e0 v0, e1 v1, e2 v2, e3 v3
#define PDECL_5(e0,e1,e2,e3,e4,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4
#ifdef MSVC_VER
# define PDECL_N(n,...) PDECL_I(n,__VA_ARGS__)
# define PDECL_I(n,...) APPEND(APPEND(PDECL_,n)(__VA_ARGS__),)
#else
# define PDECL_N(n,...) APPEND(PDECL_,n)(__VA_ARGS__)
#endif
#define PDECL(...) PDECL_N(NARGS(__VA_ARGS__),__VA_ARGS__)

// Pass variables using comma separator
#define PCALL_0(...)
#define PCALL_1(...) v0
#define PCALL_2(...) v0, v1
#define PCALL_3(...) v0, v1, v2
#define PCALL_4(...) v0, v1, v2, v3
#define PCALL_5(...) v0, v1, v2, v3, v4
#ifdef MSVC_VER
# define PCALL_N(n,...) PDECL_I(n,__VA_ARGS__)
# define PCALL_I(n,...) APPEND(APPEND(PCALL_,n)(__VA_ARGS__),)
#else
# define PCALL_N(n,...) APPEND(PCALL_,n)(__VA_ARGS__)
#endif
#define PCALL(...) PCALL_N(NARGS(__VA_ARGS__),__VA_ARGS__)

#endif // MACROS_H
