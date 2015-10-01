#include <iostream>

#include <vector>
#include <list>

#include <boost/fusion/include/equal_to.hpp>

#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <pre/boost/fusion/adapted_struct_jsonize.hpp>
#include <boost/variant.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/exception_ptr.hpp>


typedef size_t years;

namespace datamodel {

  using boost::fusion::operator==;
  using boost::fusion::operator!=;

  struct skill {
    std::string skill_name;
    years experience;
  };
}

BOOST_FUSION_DEFINE_STRUCT((datamodel), customer,
    (std::string, name)
    (size_t, age)
    (std::vector<int>, friends_id)
    (std::list<datamodel::skill>, skillset)
    )

namespace datamodel {

  struct sales_assitant {

    sales_assitant() {}

    sales_assitant(
      std::string nickname,
      size_t salary,
      customer main_customer) 
      : nickname(nickname),
         salary(salary),
         main_customer(main_customer) {
     }

    std::string nickname;
    size_t salary;
    customer main_customer; 
  };
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::skill,
    skill_name,
    experience)

BOOST_FUSION_ADAPT_STRUCT(datamodel::sales_assitant,
  nickname,
  salary,
  main_customer)


int main(int argc, char** argv) {
  datamodel::customer instance {
    "Mr. Dupond",
    43,
    {1,5},
    {datamodel::skill{"C++", 10}, datamodel::skill{"GML", 20}, datamodel::skill{"Linux", 2}}
  };

  auto json_customer = boost::fusion::adapted_struct_jsonize::jsonize(instance);
  
  std::cout << json_customer << std::endl;

  nlohmann::json obj {
    {"skill_name", "GML"},
    {"experience", 10}
  };

  auto skill_parsed = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::skill>(obj);

  std::cout << boost::fusion::adapted_struct_jsonize::jsonize(skill_parsed) << std::endl;

  
  auto customer_parsed = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::customer>(json_customer);

  auto json_customer_after_reserialize = boost::fusion::adapted_struct_jsonize::jsonize(instance);
  
  std::cout << json_customer_after_reserialize << std::endl;

  BOOST_ASSERT(json_customer == json_customer_after_reserialize);


  // Test nested objects
  {
    datamodel::sales_assitant assistant{"Mr. Gold", 130000, instance};
    auto json_sales_assistant = boost::fusion::adapted_struct_jsonize::jsonize(assistant);
    std::cout << json_sales_assistant << std::endl;

    auto deser_assistant = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::sales_assitant>(json_sales_assistant);

    auto json_sales_assistant_reserialized = boost::fusion::adapted_struct_jsonize::jsonize(deser_assistant);
    std::cout << json_sales_assistant_reserialized << std::endl;
    BOOST_ASSERT(json_sales_assistant_reserialized == json_sales_assistant);
  }

  // Test working objects
  {
    auto json = R"(
      {
          "main_customer": {
              "age": 43,
              "friends_id": [
                  1,
                  5
              ],
              "name": "Mr. Dupond",
              "skillset": [
                  {
                      "experience": 10,
                      "skill_name": "C++"
                  },
                  {
                      "experience": 20,
                      "skill_name": "GML"
                  },
                  {
                      "experience": 2,
                      "skill_name": "Linux"
                  }
              ]
          },
          "nickname": "Mr. Gold",
          "salary": 130000
      }

    )"_json;

    auto deser_assistant = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::sales_assitant>(json);

    auto json_sales_assistant_reserialized = boost::fusion::adapted_struct_jsonize::jsonize(deser_assistant);
    std::cout << json_sales_assistant_reserialized << std::endl;
  }

  // Test buggy keys
  try {
    auto json = R"(
      {
          "main_customer": {
              "BUGGYKEYage": 43,
              "friends_id": [
                  1,
                  5
              ],
              "name": "Mr. Dupond",
              "skillset": [
                  {
                      "experience": 10,
                      "skill_name": "C++"
                  },
                  {
                      "experience": 20,
                      "skill_name": "GML"
                  },
                  {
                      "experience": 2,
                      "skill_name": "Linux"
                  }
              ]
          },
          "nickname": "Mr. Gold",
          "OTHERBUGGYKEYsalary": 130000
      }

    )"_json;

    auto deser_assistant = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::sales_assitant>(json);
    // Must fail
    BOOST_ASSERT(false);

  } catch (...) {
    std::cout << "EXCEPTION which will be outputed to library user : " << std::endl;
    std::cout << boost::current_exception_diagnostic_information(true) << std::endl;
    BOOST_ASSERT(true);
  }

  // Test buggy value types
  try {
    auto json = R"(
      {
          "main_customer": {
              "age": "He is 43 years old, does it fit in an integer ?",
              "friends_id": [
                  1,
                  5
              ],
              "name": "Mr. Dupond",
              "skillset": [
                  {
                      "experience": 10,
                      "skill_name": "C++"
                  },
                  {
                      "experience": 20,
                      "skill_name": "GML"
                  },
                  {
                      "experience": 2,
                      "skill_name": "Linux"
                  }
              ]
          },
          "nickname": "Mr. Gold",
          "salary": 130000
      }

    )"_json;

    auto deser_assistant = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::sales_assitant>(json);
    // Must fail
    BOOST_ASSERT(false);

  } catch (...) {
    std::cout << "EXCEPTION which will be outputed to library user : " << std::endl;
    std::cout << boost::current_exception_diagnostic_information(true) << std::endl;
    BOOST_ASSERT(true);
  }

  {
    std::vector<datamodel::skill> skills {
      datamodel::skill{"C++", 0},
      datamodel::skill{"Java", 0},
      datamodel::skill{".Net", 0},
      datamodel::skill{"Objective-C", 0},
      datamodel::skill{"HTML", 0},
      datamodel::skill{"SQL", 0},
      datamodel::skill{"Project-Management", 0}
    };

    auto skills_json = boost::fusion::adapted_struct_jsonize::jsonize(skills);
    std::cout << skills_json << std::endl;

    auto skills_deserialized = boost::fusion::adapted_struct_dejsonize::dejsonize<std::vector<datamodel::skill>>(skills_json); 
    BOOST_ASSERT(skills == skills_deserialized);
  }


  return 0;
}
