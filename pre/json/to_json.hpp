#ifndef PRE_JSON_TO_JSON_HPP
#define PRE_JSON_TO_JSON_HPP

#include <pre/json/detail/jsonizer.hpp>

namespace pre { namespace json {

  /**
   * \brief Serialize to a JSON object any C++ object of any type, even your own types. 
   * \param T Any of the Supported Types.
   * \param value A value adapted with [BOOST\_FUSION\_ADAPT\_STRUCT](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/fusion/adapted/adapt_struct.html).
   *        This value can be any aggregate with nested aggregate or any C++ types.
   *
   * Serialize to a JSON object any C++ object of any type, even your own types. 
   *
   * It uses type traits introspection as well as Boost.Fusion reflection information to generate JSON from any types.
   *
   * ### Example 
   *  
   * \code{.cpp}
   *   #include <iostream>
   *   #include <pre/json/to_json.hpp> 
   *  
   *   struct customer {
   *     std::string name;
   *     size_t money_spent; 
   *     std::vector<std::string> interests;
   *   };
   *
   *   BOOST_FUSION_ADAPT_STRUCT(customer,
   *     name,
   *     money_spent,
   *     interests)
   *  
   *  ...
   *
   *  customer my_customer{
   *    "Mr. Dupond",
   *    1000,
   *    {"sport articles", "food", "tools"}
   *  };
   *
   *  std::cout << pre::json::to_json(my_customer) << std::endl;
   * \endcode
   * 
   *   - [Full example](../examples/to_json.cpp)
   *   - [Extended examples](../test/dejsonize_test.cpp)
   *
   * ### Supported types 
   *
   *   - [Any Integral Types](http://en.cppreference.com/w/cpp/types/is_integral)
   *   - [std::basic_string, std::string...](http://en.cppreference.com/w/cpp/string/basic_string)
   *   - [Aggregate/struct adapted with Boost.Fusion](http://www.boost.org/doc/libs/master/libs/fusion/doc/html/fusion/adapted/adapt_struct.html)
   *   - [Any Container](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) ( i.e. std::vector, std::list...)
   *   - [Any AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) ( i.e. std::map, std::set, std::multimap...)
   *   - [std::optional<X>](http://en.cppreference.com/w/cpp/utility/optional)
   *   - [std::variant<Xs...>](http://en.cppreference.com/w/cpp/utility/variant)
   *   - [std::chrono::duration](http://en.cppreference.com/w/cpp/chrono/duration)
   *
   *
   * \return An [nlohmann::json](https://github.com/nlohmann/json/) object directly streamable to std::cout or convertible to string.
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
