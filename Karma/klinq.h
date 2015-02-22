#ifndef KLINQ_H
#define KLINQ_H KLinq

#include <vector>
#include <tuple>
#include <type_traits>
#include <cstdio>

// Caches the reenter variable so it can be modified
// if the container is empty. (Fixes `select from` empty container).
class QueryValidator
{
public:
  QueryValidator(bool &reenter) : m_reenter(reenter) {}
  bool &m_reenter;
};

// Mimic the base iterator type, but keep track of whether the
// parent container is empty so that we can iterate at least 1 time.
// This added minimum fixes the `select from` empty container problem.
template <typename Iterator>
class QueryIteratorBase : public Iterator
{
public:
  typedef typename Iterator::value_type value_type;
  QueryIteratorBase(Iterator const& it, bool empty) : Iterator(it), m_empty(empty) {}
  inline bool checkEq(QueryIteratorBase const &rhs) const { return (static_cast<Iterator const&>(*this) == static_cast<Iterator const&>(rhs)); }
  inline bool checkNe(QueryIteratorBase const &rhs) const { return (static_cast<Iterator const&>(*this) != static_cast<Iterator const&>(rhs)); }
  inline bool operator==(QueryIteratorBase const &rhs) const { return (m_empty) ? false : checkEq(rhs); }
  inline bool operator!=(QueryIteratorBase const &rhs) const { return (m_empty) ? true  : checkNe(rhs); }
  inline value_type &operator*() { return (m_empty) ? *reinterpret_cast<value_type*>(this) : *static_cast<Iterator&>(*this); }
protected:
  bool m_empty;
};

template <typename Iterator>
class QueryIterator : public QueryIteratorBase<Iterator>
{
public:
  typedef typename Iterator::value_type value_type;
  QueryIterator(Iterator const& it, bool empty) : QueryIteratorBase<Iterator>(it, empty) {}
  inline value_type &operator*() { return (this->m_empty) ? *reinterpret_cast<value_type*>(this) : *static_cast<Iterator&>(*this); }
};

template <typename Iterator>
class ConstQueryIterator : public QueryIteratorBase<Iterator>
{
public:
  typedef typename Iterator::value_type value_type;
  ConstQueryIterator(Iterator const& it, bool empty) : QueryIteratorBase<Iterator>(it, empty) {}
  inline value_type const &operator*() { return (this->m_empty) ? *reinterpret_cast<value_type const*>(this) : *static_cast<Iterator&>(*this); }
};

// Mimic the container type, it's imperative that we do not
// add members because we aren't constructing a new QueryContainer.
// We simply cast the original container to look like a QueryContainer
// to inject our QueryIterator types into the range-based for loop.
template <typename Container>
class QueryContainer : public Container
{
public:
  typedef QueryIterator<typename Container::iterator> iterator;
  typedef ConstQueryIterator<typename Container::const_iterator> const_iterator;
  inline bool empty() const { return Container::empty(); }
  inline iterator begin() { return iterator(Container::begin(),empty()); }
  inline iterator end() { return iterator(Container::end(),empty()); }
  inline const_iterator begin() const { return const_iterator(Container::begin(),empty()); }
  inline const_iterator end() const { return const_iterator(Container::end(),empty()); }
  inline const_iterator cbegin() const { return const_iterator(Container::cbegin(),empty()); }
  inline const_iterator cend() const { return const_iterator(Container::cend(),empty()); }
};

// global operator to allow the Container to cast to QueryValidator.
template <typename Container>
QueryContainer<Container> &operator<<(Container &c, QueryValidator const &v)
{
  if (c.empty()) v.m_reenter = true;
  return static_cast<QueryContainer<Container>&>(c);
}

template <typename Container>
const QueryContainer<Container> &operator<<(const Container &c, QueryValidator const &v)
{
  if (c.empty()) v.m_reenter = true;
  return static_cast<const QueryContainer<Container>&>(c);
}

// Note: Multiple from queries are allowed, it acts as a nested for-loop.
// auto query = select from(variable : container)+ where(condition) [join,exclude](value);
#define DECLVEC(...) static std::vector<decltype(std::make_tuple(__VA_ARGS__))> _results; if (reenter) goto returnResults
#define DECLRET( ) continue; returnResults: auto retVal = std::move(_results); _results = decltype(_results)(); return std::move(retVal); }}
#define SELECT [&]() { bool _where; for (bool reenter = false;;reenter = true
#define FROM(ain) ) for (auto & ain << ::QueryValidator(reenter)
#define WHERE(test) ) { _where = (test);
#define JOIN(...) { DECLVEC(__VA_ARGS__); if (_where) _results.emplace_back(__VA_ARGS__); DECLRET(); }
#define EXCLUDE(...) { DECLVEC(__VA_ARGS__); if (!_where) _results.emplace_back(__VA_ARGS__); DECLRET(); }

#endif // KLINQ_H
