#include <iostream>
#include <pre/json/to_json.hpp>
#include <pre/json/mapping.hpp>

struct customer {
  std::string name;
  size_t money_spent;
  std::vector<std::string> interests;
  bool is_private;
};

BOOST_FUSION_ADAPT_STRUCT(customer,
  name,
  money_spent,
  interests,
  is_private)


int main() {

  {
    customer my_customer{
      "Mr. Dupond",
      1000,
      {"sport articles", "food", "tools"},
      true
    };

    std::cout << pre::json::to_json(my_customer) << std::endl;

    // with remapping
    std::cout << "With property 'is_private' mapped to 'private':\n" 
      << pre::json::to_json(
        my_customer, 
        [](auto &jdoc) { 
          pre::json::remap_property(jdoc, "/is_private"_json_pointer, "/private"_json_pointer);
        }) 
      << std::endl;
  }


  return 0;
}
