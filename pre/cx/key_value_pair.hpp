#pragma once

#include <cx_json_parser.h>
#include <cx_json_value.h>

namespace pre::cx {
  using ::cx::static_string;
  template <cx::static_string str>
  struct tstring {
    static constexpr cx::static_string value = str;
  };

  template <cx::static_string key_, class T>
  struct key_value_pair { 
    using key = tstring<key_>;
    T value;
  };

  template<class T>
  auto get_key(const T&) {
    return T::key::value.c_str();
  };

}