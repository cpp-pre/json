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


int main() {

  customer my_customer{
    "Mr. Dupond",
    1000,
    {"sport articles", "food", "tools"}
  };

  std::cout << pre::json::to_json(my_customer) << std::endl;
  return 0;
}
