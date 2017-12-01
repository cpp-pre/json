#define BOOST_TEST_MODULE for_each_type
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <pre/variant/for_each_type.hpp>

/* Goal is to test compilation with temporary visitor and with non copyable ones. */

using boost_test_variant = boost::variant<int, double, std::string>;
using mapbox_test_variant = mapbox::util::variant<int, double, std::string>;

struct uncopyable_visitor : public boost::static_visitor<> {
  uncopyable_visitor() = default;
  ~uncopyable_visitor() = default;
  uncopyable_visitor(const uncopyable_visitor&) = delete; 

  template<class T>
  void operator()(const T&) { std::cout << typeid(T).name() << std::endl; }
};

BOOST_AUTO_TEST_CASE (mapbox_test) {

    uncopyable_visitor each{};
    pre::variant::for_each_type(mapbox_test_variant{}, each);
    pre::variant::for_each_type(mapbox_test_variant{}, uncopyable_visitor{});
}

BOOST_AUTO_TEST_CASE (boost_test) {

    uncopyable_visitor each{};
    pre::variant::for_each_type(boost_test_variant{}, each);
    pre::variant::for_each_type(boost_test_variant{}, uncopyable_visitor{});

}
