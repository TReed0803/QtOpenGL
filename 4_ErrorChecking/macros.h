#ifndef MACROS_H
#define MACROS_H

// Delay the preprocessor by one step
#define _DELAY(d) d
#define DELAY(d) d

// Append two preprocessor arguments together
#define _APPEND(a,b) a##b
#define APPEND(a,b) _APPEND(a,b)

// Count number of arguments
#define _NARGS(_1,_2,_3,_4,_5,N,...) N
#define NARGS(...) APPEND(_,NARGS)(__VA_ARGS__,5,4,3,2,1)

#endif // MACROS_H
