#include <iostream>

#include <vector>
#include <list>

#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <pre/boost/fusion/adapted_struct_jsonize.hpp>
#include <boost/variant.hpp>

#include <boost/tuple/tuple.hpp>

typedef size_t years;

struct skill {
  std::string skill_name;
  years experience;
};

BOOST_FUSION_ADAPT_STRUCT(skill,
    skill_name,
    experience)

BOOST_FUSION_DEFINE_STRUCT((datamodel), customer,
    (std::string, name)
    (size_t, age)
    (std::vector<int>, friends_id)
    (std::list<skill>, skillset)
    )

int main(int argc, char** argv) {
  datamodel::customer instance {
    "Mr. Dupond",
    43,
    {1,5},
    {skill{"C++", 10}, skill{"GML", 20}, skill{"Linux", 2}}
  };

  auto json = boost::fusion::adapted_struct_jsonize::jsonize(instance);
  
  std::cout << json << std::endl;

  nlohmann::json obj {
    {"skill_name", "GML"},
    {"experience", 10}
  };

  auto skill_parsed = boost::fusion::adapted_struct_dejsonize::dejsonize<skill>(obj);

  std::cout << boost::fusion::adapted_struct_jsonize::jsonize(skill_parsed) << std::endl;
  return 0;
}
