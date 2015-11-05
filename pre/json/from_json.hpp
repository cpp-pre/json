#ifndef PRE_JSON_FROM_JSON_HPP
#define PRE_JSON_FROM_JSON_HPP

#include <pre/json/detail/dejsonizer.hpp>

namespace pre { namespace json {

    template<class T>
    T from_json(const nlohmann::json& json_object) {
      T object;
      detail::dejsonizer dejsonizer(json_object);
      dejsonizer(object);
      return object;
    }

}}

#endif
