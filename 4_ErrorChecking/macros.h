#ifndef MACROS_H
#define MACROS_H

// Delay the preprocessor by one step
#define CYCLE()

// Append two preprocessor arguments together
#define _APPEND(a,b) a##b
#define APPEND(a,b) _APPEND(a,b)

// Count number of arguments
#define _NARGS(_1,_2,_3,_4,_5,N,...) N
#define NARGS(...) _NARGS CYCLE() (__VA_ARGS__,5,4,3,2,1)

// Stringize Token
#define _STR(x) #x
#define STR(x) _STR(x)

// Get specific value from __VA_ARGS__
#define PGET_0(...)
#define PGET_1(r,...) r
#define PGET_2(a,...) PGET_1 CYCLE() (__VA_ARGS__)
#define PGET_3(a,...) PGET_2 CYCLE() (__VA_ARGS__)
#define PGET_4(a,...) PGET_3 CYCLE() (__VA_ARGS__)
#define PGET_5(a,...) PGET_4 CYCLE() (__VA_ARGS__)
#define PGET_N(n,...) APPEND(PGET_,n) CYCLE() (__VA_ARGS__)

// Declare variables using separator `sep`
#define PDECL_0(...)
#define PDECL_1(...)                                PGET_1 CYCLE() (__VA_ARGS__) v1
#define PDECL_2(...) PDECL_1 CYCLE() (__VA_ARGS__), PGET_2 CYCLE() (__VA_ARGS__) v2
#define PDECL_3(...) PDECL_2 CYCLE() (__VA_ARGS__), PGET_3 CYCLE() (__VA_ARGS__) v2
#define PDECL_4(...) PDECL_3 CYCLE() (__VA_ARGS__), PGET_4 CYCLE() (__VA_ARGS__) v2
#define PDECL_5(...) PDECL_4 CYCLE() (__VA_ARGS__), PGET_5 CYCLE() (__VA_ARGS__) v2
#define PDECL_N(n,...) APPEND(PDECL_,n) CYCLE() (__VA_ARGS__)
#define PDECL(...) PDECL_N(NARGS(__VA_ARGS__),__VA_ARGS__)

// Pass variables using separator `sep`
#define PCALL_0()
#define PCALL_1() v1
#define PCALL_2() PCALL_1(), v2
#define PCALL_3() PCALL_2(), v3
#define PCALL_4() PCALL_3(), v4
#define PCALL_5() PCALL_4(), v5
#define PCALL_N(n,...) APPEND(PCALL_,n) CYCLE() ()
#define PCALL(...) PCALL_N(NARGS(__VA_ARGS__),__VA_ARGS__)

#endif // MACROS_H
