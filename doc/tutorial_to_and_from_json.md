Tutorial to or from json
========================
With pre::json you can serialize or deserialize any C++ Type to JSON, even composite and aggregate types.

## Serialize anything
You can serialize anything to json.

See : 

  - [Full example](../examples/to_json.cpp)
  - [Extended examples](../test/dejsonize_test.cpp)

```cpp
  #include <iostream>
  #include <pre/json/to_json.hpp> 
 
  struct customer {
    std::string name;
    size_t money_spent; 
    std::vector<std::string> interests;
  };

  BOOST_FUSION_ADAPT_STRUCT(customer,
    name,
    money_spent,
    interests)
 
 ...

 customer my_customer{
   "Mr. Dupond",
   1000,
   {"sport articles", "food", "tools"}
 };

 std::cout << pre::json::to_json(my_customer) << std::endl;
```

## Deserialize anything
You can deserialize to any type from json.

See : 

  - [Full example](../examples/from_json.cpp)
  - [Extended examples](../test/dejsonize_test.cpp)

```cpp
 #include <iostream>
 #include <pre/json/from_json.hpp> 
 
 struct customer {
   std::string name;
   size_t money_spent; 
   std::vector<std::string> interests;
 };
                                                                                           
 BOOST_FUSION_ADAPT_STRUCT(customer,
   name,
   money_spent,
   interests)
 
 ...
                                                                                           
 std::string string_to_deserialize = 
   "{\"interests\":[\"sport articles\"], \"money_spent\":50, \"name\":\"Mrs. Fraulein\"}";
                                                                                           
 customer my_customer = pre::json::from_json<customer>(string_to_deserialize);
                                                                                           
 std::cout << "Customer " << my_customer.name << " spent " <<
   my_customer.money_spent << std::endl;
                                                                                           
```
