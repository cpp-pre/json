#include <iostream>
#include <pre/json/from_json.hpp> 
#include <pre/json/mapping.hpp>
#include <nlohmann/json.hpp>

 
struct customer {
  std::string name;
  size_t money_spent; 
  std::vector<std::string> interests;
  std::optional<bool> is_private;  // this property shall be mapped to the "private" key in the json
  std::vector<int> numbers;
};

BOOST_FUSION_ADAPT_STRUCT(customer,
  name,
  money_spent,
  interests,
  is_private,
  numbers)

int main() {


  std::string string_to_deserialize = 
    "{\"interests\":[\"sport articles\"], \"money_spent\":50, \"name\":\"Mrs. Fraulein\", \"private\": true, \"numbers\": [1,2,3]}";

  // classic deserialization
  {
    customer my_customer = pre::json::from_json<customer>(string_to_deserialize);

    std::cout << "Customer " << my_customer.name << " spent " <<
      my_customer.money_spent << std::endl;
  }

  // with mapping deserialization
  {
    customer my_customer = pre::json::from_json<customer>(string_to_deserialize, [](nlohmann::json& jdoc) {
      jdoc["is_private"] = jdoc["private"];
      jdoc.erase("private");
    });

    std::cout << "Customer " << my_customer.name << " spent " <<
      my_customer.money_spent << " is private:" << std::to_string(my_customer.is_private.value()) << std::endl;
  }

  // with mapping helpers
  {
    customer my_customer = pre::json::from_json<customer>(string_to_deserialize, [](nlohmann::json& jdoc) {
      pre::json::remap_property(jdoc, "private", "is_private");
    });

    std::cout << "Customer " << my_customer.name << " spent " <<
      my_customer.money_spent << " is private:" << std::to_string(my_customer.is_private.value()) << std::endl;
  }

  // with mapping and json_pointer helpers
  {
    std::string string_to_deserialize = 
    "{\"interests\":[\"sport articles\"], \"money_spent\":50, \"name\":\"Mrs. Fraulein\", \"private\": true, \"numbers\": [1,2,3]}";

    customer my_customer = pre::json::from_json<customer>(string_to_deserialize, [](nlohmann::json& jdoc) {
      pre::json::remap_property(jdoc, "/private"_json_pointer, "/is_private"_json_pointer);
      pre::json::remap_property(jdoc, "/numbers/0"_json_pointer, "/numbersNew/2"_json_pointer);
      pre::json::remap_property(jdoc, "/money_spent"_json_pointer, "/numbers/0"_json_pointer);
      jdoc["money_spent"] = 0;
      std::cout << jdoc.dump(2) << std::endl; 
    });
  } 

  return 0;
}
