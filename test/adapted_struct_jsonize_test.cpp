#define BOOST_TEST_MODULE adapted_struct_jsonize_test
#include <boost/test/included/unit_test.hpp>

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

#include <boost/variant.hpp>



namespace datamodel {

  using boost::fusion::operator==;
  using boost::fusion::operator!=;

  typedef size_t years;

  struct skill {
    std::string skill_name;
    years experience;
  };
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::skill,
    skill_name,
    experience)


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

BOOST_FUSION_ADAPT_STRUCT(datamodel::sales_assitant,
  nickname,
  salary,
  main_customer)



BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_test_composedtype) {

  // Testing a customer structure
  datamodel::customer customer {
    "Mr. Dupond",
    43,
    {1,5},
    {datamodel::skill{"C++", 10}, datamodel::skill{"GML", 20}, datamodel::skill{"Linux", 2}}
  };

  auto json_customer = boost::fusion::adapted_struct_jsonize::jsonize(customer);
  std::cout << json_customer << std::endl;

  auto customer_parsed = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::customer>(json_customer);
  BOOST_ASSERT(customer_parsed == customer);

  auto json_customer_after_reserialize = boost::fusion::adapted_struct_jsonize::jsonize(customer);
  std::cout << json_customer_after_reserialize << std::endl;
  BOOST_ASSERT(json_customer == json_customer_after_reserialize);

  // testing an adapted skill alone from plain json
  nlohmann::json obj {
    {"skill_name", "GML"},
    {"experience", 10}
  };

  auto skill_parsed = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::skill>(obj);

  BOOST_ASSERT(skill_parsed.skill_name == "GML");
  BOOST_ASSERT(skill_parsed.experience == 10);
  std::cout << boost::fusion::adapted_struct_jsonize::jsonize(skill_parsed) << std::endl;
}

/**
 * Test nested objects
 */
BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_test_nested) {
  datamodel::customer customer {
    "Mr. Dupond",
    43,
    {1,5},
    {datamodel::skill{"C++", 10}, datamodel::skill{"GML", 20}, datamodel::skill{"Linux", 2}}
  };

  datamodel::sales_assitant assistant{"Mr. Gold", 130000, customer};
  auto json_sales_assistant = boost::fusion::adapted_struct_jsonize::jsonize(assistant);
  std::cout << json_sales_assistant << std::endl;

  auto deser_assistant = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::sales_assitant>(json_sales_assistant);

  auto json_sales_assistant_reserialized = boost::fusion::adapted_struct_jsonize::jsonize(deser_assistant);
  std::cout << json_sales_assistant_reserialized << std::endl;

  BOOST_ASSERT(json_sales_assistant_reserialized == json_sales_assistant);
}

BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_test_working_plain_json) {
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

BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_test_incorrect_plain_json) {
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
}


BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_test_containers_direct) {
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

namespace datamodel {

  struct cleaner {
    std::string floor;
    std::string room;
  };

}

BOOST_FUSION_ADAPT_STRUCT(datamodel::cleaner,
  floor,
  room)

namespace datamodel {

  struct cashier {
    std::string section;
    int checkout_number;
  };

}

BOOST_FUSION_ADAPT_STRUCT(datamodel::cashier,
  section,
  checkout_number)

namespace datamodel {

  struct security {
    bool has_a_weapon;
    std::string fighting_tactic;
  };

  typedef boost::variant<cleaner, cashier, security> possible_responsibilities;
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::security,
  has_a_weapon,
  fighting_tactic)



namespace datamodel {
  struct employee {
    std::string name;
    possible_responsibilities responsibility;
  };
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::employee,
  name,
  responsibility)

BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_test_vairants) {

  std::vector<datamodel::employee> employees {
    {"King", datamodel::cashier{"hardware", 1} },
    {"Blake", datamodel::security{true, "Krav Maga"} },
    {"Martin", datamodel::cleaner{"5th floor", "Toys, Petshop, Drugs, Food" } },
    {"Ward", datamodel::cashier{"Food", 2} }
  };

  auto employees_json = boost::fusion::adapted_struct_jsonize::jsonize(employees);
  std::cout << employees_json << std::endl;

  auto employees_deserialized = boost::fusion::adapted_struct_dejsonize::dejsonize<std::vector<datamodel::employee>>(employees_json); 

  auto employees_reserialized = boost::fusion::adapted_struct_jsonize::jsonize(employees_deserialized);
  std::cout << employees_reserialized << std::endl;

  BOOST_ASSERT(employees == employees_deserialized);
}

namespace datamodel {

  struct struct_with_a_map {
    std::map<std::string, int> with_a_map;
    std::map<std::string, skill> some_other_map;
  };
  
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::struct_with_a_map, with_a_map, some_other_map)

BOOST_AUTO_TEST_CASE(adapted_struct_jsonize_test_maps) {

  datamodel::struct_with_a_map some{
    {
      {"hellO",12},
      {"help", 15}
    },
    {
      {"test", datamodel::skill{"C++", 10}}
    }
  };

  auto some_json = boost::fusion::adapted_struct_jsonize::jsonize(some);
  std::cout << some_json << std::endl; 

  auto some_deserialized = boost::fusion::adapted_struct_dejsonize::dejsonize<datamodel::struct_with_a_map>(some_json);
  
  BOOST_ASSERT(some_deserialized == some); 
}
