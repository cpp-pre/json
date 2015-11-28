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

int main() {

  std::string string_to_deserialize = 
    "{\"interests\":[\"sport articles\"], \"money_spent\":50, \"name\":\"Mrs. Fraulein\"}";

  customer my_customer = pre::json::from_json<customer>(string_to_deserialize);

  std::cout << "Customer " << my_customer.name << " spent " <<
    my_customer.money_spent << std::endl;

  return 0;
}
