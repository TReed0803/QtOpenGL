#ifndef KTYPETRAITS_H
#define KTYPETRAITS_H KTypeTraits

#include <type_traits>
#include <Qt>

namespace Karma
{

/*!
 * The template `has_destructor<T>` exports a
 * boolean constant `value that is true iff `T` has
 * a public destructor.
 *
 * N.B. A compile error will occur if T has non-public destructor.
 * \see http://stackoverflow.com/questions/10711952/how-to-detect-existence-of-a-class-using-sfinae
 */
template< typename T>
struct has_destructor
{
    /* Has destructor :) */
    template <typename A>
    static std::true_type test(decltype(std::declval<A>().~A()) *) {
        return std::true_type();
    }

    /* Has no destructor :( */
    template<typename A>
    static std::false_type test(...) {
        return std::false_type();
    }

    /* This will be either `std::true_type` or `std::false_type` */
    typedef decltype(test<T>(0)) type;

    static const bool value = type::value; /* Which is it? */
};

template<typename T>
struct is_defined : has_destructor<T> {};

template <typename T>
inline typename std::enable_if<Karma::is_defined<T>::value>::type safeDelete(T *data)
{
  delete data;
}

template <typename T>
inline typename std::enable_if<!Karma::is_defined<T>::value>::type safeDelete(T *data)
{
  (void)data;
  qFatal("Attempted to delete a variable which was not defined!");
}

}

#endif // KTYPETRAITS_H

