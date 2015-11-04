#ifndef PRE_JSON_JSONIZE_HPP
#define PRE_JSON_JSONIZE_HPP

#include <pre/json/detail/jsonizer.hpp>

namespace pre { namespace json {

  template<class T>
  nlohmann::json jsonize(const T& value) {
    nlohmann::json json_object;
    detail::jsonizer jsonizer(json_object);
    jsonizer(value);
    return json_object;
  }

}}

#endif
