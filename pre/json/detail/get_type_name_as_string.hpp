#ifndef PRE_JSON_DETAIL_GET_TYPE_NAME_AS_STRING_HPP
#define PRE_JSON_DETAIL_GET_TYPE_NAME_AS_STRING_HPP

#include <boost/type_index.hpp>
#include <boost/algorithm/string.hpp>

namespace pre { namespace json { namespace detail {

  template<class T>
  inline std::string get_type_name_as_string() {
  
    using namespace boost::algorithm;
    auto pretty_name = boost::typeindex::type_id<T>().pretty_name();
    if (starts_with(pretty_name, "struct ")) {
      return replace_first_copy(pretty_name, "struct ", "");
    } else if (starts_with(pretty_name, "class ")) {
      return replace_first_copy(pretty_name, "class ", "");
    } else {
      return pretty_name;
    }
  }
}}}

#endif
