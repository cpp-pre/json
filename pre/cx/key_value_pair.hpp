#pragma once

#include <cx_json_parser.h>
#include <cx_json_value.h>

#define pre_json_key(type, name) ::pre::cx::key_value_pair<"" # name, type> name

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
    friend bool operator<(const key_value_pair& x, const key_value_pair& y) {
      return x.value < y.value; 
    }
    template<class U> friend bool operator==(const key_value_pair& lhs, const U& rhs){return lhs.value == rhs.value;}
    template<class U> friend bool operator< (const key_value_pair& lhs, const U& rhs){return lhs.value < rhs.value;}
    template<class U> friend bool operator!=(const key_value_pair& lhs, const U& rhs){return !operator==(lhs,rhs);}
    template<class U> friend bool operator> (const key_value_pair& lhs, const U& rhs){return  operator< (rhs,lhs);}
    template<class U> friend bool operator<=(const key_value_pair& lhs, const U& rhs){return !operator> (lhs,rhs);}
    template<class U> friend bool operator>=(const key_value_pair& lhs, const U& rhs){return !operator< (lhs,rhs);}
  };
}

namespace std {
  template <cx::static_string key_, class T>
  class hash<pre::cx::key_value_pair<key_, T>> {
  public:
    size_t operator()(const pre::cx::key_value_pair<key_, T> &k) const
    {
      return std::hash<std::string>{}(key_.c_str());
    }
  };
}

namespace pre::cx {
  template<class T>
  auto get_key(const T&) {
    return T::key::value.c_str();
  };

  /*template <cx::static_string key_, class T>
  inline bool operator==(key_value_pair<key_, T> const& a, key_value_pair<key_, T> const& b) {
    return a.value == b.value;
  }
  template <cx::static_string key_, class T>
  inline bool operator<(key_value_pair<key_, T> const& a, key_value_pair<key_, T> const& b) {
    return a.value < b.value;
  }*/

}