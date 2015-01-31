#ifndef MACROS_H
#define MACROS_H

#include <type_traits>

// Append two preprocessor arguments together
#define _APPEND(a,b) a##b
#define APPEND(a,b) _APPEND(a,b)

// Count number of arguments
#define _NARGS(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
#define NARGS(...) APPEND(_NARGS(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1),)

// Stringize Token
#define _STR(x) #x
#define STR(x) _STR(x)

// Get specific value from __VA_ARGS__
#define PGET_0(e0,...) e0
#define PGET_1(e0,e1,...) e1
#define PGET_2(e0,e1,e2,...) e2
#define PGET_3(e0,e1,e2,e3,...) e3
#define PGET_4(e0,e1,e2,e3,e4,...) e4
#define PGET_5(e0,e1,e2,e3,e4,e5,...) e5
#define PGET_6(e0,e1,e2,e3,e4,e5,e6,...) e6
#define PGET_7(e0,e1,e2,e3,e4,e5,e6,e7,...) e7
#define PGET_8(e0,e1,e2,e3,e4,e5,e6,e7,e8,...) e8
#define PGET_9(e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,...) e9
#ifdef WIN32
# define PGET_N(n,...) PGET_I(n,__VA_ARGS__)
# define PGET_I(n,...) APPEND(APPEND(PGET_,n)(__VA_ARGS__),)
#else
# define PGET_N(n,...) APPEND(PGET_,n)(__VA_ARGS__)
#endif

// Declare variables using comma separator
#define PDECL_1(e0,...) e0 v0
#define PDECL_2(e0,e1,...) e0 v0, e1 v1
#define PDECL_3(e0,e1,e2,...) e0 v0, e1 v1, e2 v2
#define PDECL_4(e0,e1,e2,e3,...) e0 v0, e1 v1, e2 v2, e3 v3
#define PDECL_5(e0,e1,e2,e3,e4,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4
#define PDECL_6(e0,e1,e2,e3,e4,e5,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4, e5 v5
#define PDECL_7(e0,e1,e2,e3,e4,e5,e6,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4, e5 v5, e6 v6
#define PDECL_8(e0,e1,e2,e3,e4,e5,e6,e7,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4, e5 v5, e6 v6, e7 v7
#define PDECL_9(e0,e1,e2,e3,e4,e5,e6,e7,e8,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4, e5 v5, e6 v6, e7 v7, e8 v8
#define PDECL_10(e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,...) e0 v0, e1 v1, e2 v2, e3 v3, e4 v4, e5 v5, e6 v6, e7 v7, e8 v8, e9 v9
#ifdef WIN32
# define PDECL_N(n,...) PDECL_I(n,__VA_ARGS__)
# define PDECL_I(n,...) APPEND(APPEND(PDECL_,n)(__VA_ARGS__),)
#else
# define PDECL_N(n,...) APPEND(PDECL_,n)(__VA_ARGS__)
#endif

#define CAST_VOID(t) *(static_cast<std::remove_reference<t>::type*>(0))

// Declare casting for decltype usage
#define PCAST_1(e0,...) CAST_VOID(e0)
#define PCAST_2(e0,e1,...) CAST_VOID(e0), CAST_VOID(e1)
#define PCAST_3(e0,e1,e2,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2)
#define PCAST_4(e0,e1,e2,e3,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3)
#define PCAST_5(e0,e1,e2,e3,e4,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3), CAST_VOID(e4)
#define PCAST_6(e0,e1,e2,e3,e4,e5,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3), CAST_VOID(e4), CAST_VOID(e5)
#define PCAST_7(e0,e1,e2,e3,e4,e5,e6,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3), CAST_VOID(e4), CAST_VOID(e5), CAST_VOID(e6)
#define PCAST_8(e0,e1,e2,e3,e4,e5,e6,e7,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3), CAST_VOID(e4), CAST_VOID(e5), CAST_VOID(e6), CAST_VOID(e7)
#define PCAST_9(e0,e1,e2,e3,e4,e5,e6,e7,e8,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3), CAST_VOID(e4), CAST_VOID(e5), CAST_VOID(e6), CAST_VOID(e7), CAST_VOID(e8)
#define PCAST_10(e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,...) CAST_VOID(e0), CAST_VOID(e1), CAST_VOID(e2), CAST_VOID(e3), CAST_VOID(e4), CAST_VOID(e5), CAST_VOID(e6), CAST_VOID(e7), CAST_VOID(e8), CAST_VOID(e9)
#ifdef WIN32
# define PCAST_N(n,...) PCAST_I(n,__VA_ARGS__)
# define PCAST_I(n,...) APPEND(APPEND(PCAST_,n)(__VA_ARGS__),)
#else
# define PCAST_N(n,...) APPEND(PCAST_,n)(__VA_ARGS__)
#endif

// Pass variables using comma separator
#define PCALL_1(...) v0
#define PCALL_2(...) v0, v1
#define PCALL_3(...) v0, v1, v2
#define PCALL_4(...) v0, v1, v2, v3
#define PCALL_5(...) v0, v1, v2, v3, v4
#define PCALL_6(...) v0, v1, v2, v3, v4, v5
#define PCALL_7(...) v0, v1, v2, v3, v4, v5, v6
#define PCALL_8(...) v0, v1, v2, v3, v4, v5, v6, v7
#define PCALL_9(...) v0, v1, v2, v3, v4, v5, v6, v7, v8
#define PCALL_10(...) v0, v1, v2, v3, v4, v5, v6, v7, v8, v9
#ifdef WIN32
# define PCALL_N(n,...) PCALL_I(n,__VA_ARGS__)
# define PCALL_I(n,...) APPEND(APPEND(PCALL_,n)(__VA_ARGS__),)
#else
# define PCALL_N(n,...) APPEND(PCALL_,n)(__VA_ARGS__)
#endif

#endif // MACROS_H
