#ifndef KACCESSOR_H
#define KACCESSOR_H KAccessor

#include <functional>

template <typename Ret, typename Inp>
class KAccessor : public std::unary_function<Ret, Inp>
{
public:
  typedef Ret ReturnType;
  typedef Inp ContainedType;
};

#endif // KACCESSOR_H

