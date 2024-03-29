#define BOOST_TEST_MODULE for_each_type
#include <boost/test/included/unit_test.hpp>

#include "wasm_boost_test.hpp"

#include <iostream>
#include <variant>
#include <pre/variant/for_each_type.hpp>

/* Goal is to test compilation with temporary visitor and with non copyable ones. */

using std_test_variant = std::variant<int, double, std::string>;

struct uncopyable_visitor {
  uncopyable_visitor() = default;
  ~uncopyable_visitor() = default;
  uncopyable_visitor(const uncopyable_visitor&) = delete; 

  template<class T>
  void operator()(const T&) { std::cout << typeid(T).name() << std::endl; }
};

BOOST_AUTO_TEST_CASE (std_test) {

    uncopyable_visitor each{};
    pre::variant::for_each_type(std_test_variant{}, each);
    pre::variant::for_each_type(std_test_variant{}, uncopyable_visitor{});
}
