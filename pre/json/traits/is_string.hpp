#ifndef PRE_JSON_TRAITS_IS_STRING_HPP
#define PRE_JSON_TRAITS_IS_STRING_HPP

#include <string>

namespace pre { namespace json { namespace traits {

  template <typename T>
  struct is_string : std::false_type {};

  template <typename charT, typename traits, typename Alloc>
  struct is_string<std::basic_string<charT, traits, Alloc> > : std::true_type {};

}}}

#endif
