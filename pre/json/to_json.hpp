#ifndef PRE_JSON_TO_JSON_HPP
#define PRE_JSON_TO_JSON_HPP

#include <pre/json/detail/jsonizer.hpp>

namespace pre { namespace json {

  /* Facility to transform in a JSON object any C++ object of any type, even your own types. 
   * 
   * @value A value adapted with [BOOST_FUSION_ADAPT_STRUCT](http://www.boost.org/doc/libs/master/libs/fusion/doc/html/fusion/adapted/adapt_struct.html)
   */
  template<class T>
  nlohmann::json to_json(const T& value) {
    nlohmann::json json_object;
    detail::jsonizer jsonizer(json_object);
    jsonizer(value);
    return json_object;
  }

}}

#endif
