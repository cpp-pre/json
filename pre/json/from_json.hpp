#ifndef PRE_JSON_FROM_JSON_HPP
#define PRE_JSON_FROM_JSON_HPP

#include <pre/json/detail/dejsonizer.hpp>

namespace pre { namespace json {

  /**
   * \brief Deserialize any json object or value in a C++ type.
   * \param T The result type wanted from the deserialization. that will be filled from the given json_object.
   * \param serialized_json json to deserialize
   *
   * Deserialize any json object or value in a C++ type.
   *
   * The function throws an std::exception if a mandatory member 
   * isn't found, but doesn't in the case of std::optional.
   *
   * The type has to be supported, or adapted with [BOOST\_FUSION\_ADAPT\_STRUCT](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/fusion/adapted/adapt_struct.html).
   *
   * See below for supported types.
   * 
   * ### Example
   *  
   * \code{.cpp}
   *  #include <iostream>
   *  #include <pre/json/from_json.hpp> 
   *  
   *  struct customer {
   *    std::string name;
   *    size_t money_spent; 
   *    std::vector<std::string> interests;
   *  };
   *
   *  BOOST_FUSION_ADAPT_STRUCT(customer,
   *    name,
   *    money_spent,
   *    interests)
   *  
   *  ...
   *
   *  std::string string_to_deserialize = 
   *    "{\"interests\":[\"sport articles\"], \"money_spent\":50, \"name\":\"Mrs. Fraulein\"}";
   *
   *  customer my_customer = pre::json::from_json<customer>(string_to_deserialize);
   *
   *  std::cout << "Customer " << my_customer.name << " spent " <<
   *    my_customer.money_spent << std::endl;
   *
   * \endcode
   *
   *   - [Full example](../examples/from_json.cpp)
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
   * \return the type wanted if it succeed find all member and value in the json.
   */
  template<class T>
  T from_json(const std::string& serialized_json) {
    const nlohmann::json json_object = nlohmann::json::parse(serialized_json);
    T object;
    detail::dejsonizer dejsonizer(json_object);
    dejsonizer(object);
    return object;
  }

  /**
   * \brief Same as pre::json::from_json(const std::string&) but directly with a JSON.
   */
  template<class T>
  T from_json(const nlohmann::json& json_object) {
    T object;
    detail::dejsonizer dejsonizer(json_object);
    dejsonizer(object);
    return object;
  }
}}

#endif
