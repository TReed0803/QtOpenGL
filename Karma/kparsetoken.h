#ifndef KPARSETOKEN_H
#define KPARSETOKEN_H KParseToken

#include <cstring>
#include <functional>
#include <map>

union KDefaultAttribs
{
  int asInteger;
  float asFloat;
};

struct KParseCompare : public std::binary_function<bool, char const*, char const*>
{
  inline bool operator()(char const* lhs, char const* rhs) const
  {
    return std::strcmp(lhs, rhs) < 0;
  }
};

template <typename IdType>
using KParseMap = std::map<char const*, IdType, KParseCompare>;

template <typename IdType, typename AttribType = KDefaultAttribs>
class KParseToken
{
public:

  typedef IdType token_id;
  typedef AttribType attrib_type;

  token_id m_token;
  attrib_type m_attribute;
  std::string m_lexicon;
  inline KParseToken();
  inline operator token_id() const;
private:
  inline KParseToken(KParseToken const &rhs);
  inline KParseToken(KParseToken &&rhs);
  inline void operator=(KParseToken const &rhs);
  inline void operator=(KParseToken &&rhs);
};

template <typename IdType, typename AttribType>
inline KParseToken<IdType, AttribType>::KParseToken() :
  m_token(static_cast<IdType>(0))
{
  // Intentionally Empty
}

template <typename IdType, typename AttribType>
inline KParseToken<IdType, AttribType>::operator token_id() const
{
  return m_token;
}

#endif // KPARSETOKEN_H
