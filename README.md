[![pipeline status](https://git.lecbna.org/cpp-pre/json/badges/master/pipeline.svg)](https://git.lecbna.org/cpp-pre/json/commits/master) *for win10, macOS & linux.*
# pre::from_ or to_ json

[de]Serializes any json object or value to a C++ type with the magic of compile time reflection.

## Examples

See : 
 - [Deserialize JSON to a C++ aggregate](examples/from_json.cpp)
 - [Serialize a C++ aggregate to JSON](examples/to_json.cpp)
 - [Extensive Tests](test/dejsonize_test.cpp)

```cpp
#include <iostream>
#include <pre/json/from_json.hpp> 

// Your C++ type
struct customer {
  std::string name;
  size_t money_spent; 
  std::vector<std::string> interests;
};

// Reflection for the fields you want to load from json
BOOST_FUSION_ADAPT_STRUCT(customer,
  name,
  money_spent,
  interests)

...

std::string string_from_network = 
  "{\"interests\":[\"sport articles\"], \"money_spent\":50, \"name\":\"Mrs. Fraulein\"}";

// Here you get your C++ type
customer my_customer =
  pre::json::from_json<customer>(string_from_network);

std::cout << "Customer " << my_customer.name << " spent " <<
  my_customer.money_spent << std::endl;
```

### Supported types 

  - [Any Integral Types](http://en.cppreference.com/w/cpp/types/is_integral)
  - [std::basic_string, std::string...](http://en.cppreference.com/w/cpp/string/basic_string)
  - [Aggregate/struct adapted with Boost.Fusion](http://www.boost.org/doc/libs/master/libs/fusion/doc/html/fusion/adapted/adapt_struct.html)
  - [Any Container](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) ( i.e. std::vector, std::list...)
  - [Any AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) ( i.e. std::map, std::set, std::multimap...)
  - [std::optional<X>](http://en.cppreference.com/w/cpp/utility/optional)
  - [std::variant<Xs...>](http://en.cppreference.com/w/cpp/utility/variant)
  - [std::chrono::duration](http://en.cppreference.com/w/cpp/chrono/duration)


## License
[Boost Software License](./LICENSE).

Please give copyright notice : 
```
Copyright (c) 2012-2018 Damien Buhl alias daminetreg (damien.buhl@lecbna.org)
```

