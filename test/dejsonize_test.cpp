#define BOOST_TEST_MODULE dejsonize_test
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <vector>
#include <list>

#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/variant.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/exception_ptr.hpp>

#include <pre/json/traits/is_jsonizable.hpp>
#include <pre/json/to_json.hpp>
#include <pre/json/from_json.hpp>

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

BOOST_AUTO_TEST_CASE (traits) {

  struct non_adapted_struct {
    int dummy;
    int dummy2;
  };

  static_assert(pre::json::traits::is_jsonizable<datamodel::customer>::type::value, "datamodel::customer should be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable<int>::type::value, "int should be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable<float>::type::value, "float should be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable<double>::type::value, "float should be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable<std::string>::type::value, "std::string should be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable< std::map<std::string, int> >::type::value, "std::map<std::string,int> should be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable< std::vector<std::string> >::type::value, "should NOT be jsonizable");
  static_assert(pre::json::traits::is_jsonizable< std::vector<int> >::type::value, "should NOT be jsonizable");

  static_assert(pre::json::traits::is_jsonizable<std::map<int, int>>::type::value == false , "std::map<int,int> should NOT be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable<non_adapted_struct>::type::value == false , "non_adapted_struct should NOT be jsonizable.");
  static_assert(pre::json::traits::is_jsonizable< std::vector<non_adapted_struct> >::type::value == false, "should NOT be jsonizable");

}

BOOST_AUTO_TEST_CASE (composedtype) {


  // Testing a customer structure
  datamodel::customer customer {
    "Mr. Dupond",
    43,
    {1,5},
    {datamodel::skill{"C++", 10}, datamodel::skill{"GML", 20}, datamodel::skill{"Linux", 2}}
  };

  auto json_customer = pre::json::to_json(customer);
  std::cout << json_customer.dump(2) << std::endl;

  auto customer_parsed = pre::json::from_json<datamodel::customer>(json_customer);
  BOOST_REQUIRE(customer_parsed == customer);

  auto json_customer_after_reserialize = pre::json::to_json(customer);
  std::cout << json_customer_after_reserialize.dump(2) << std::endl;
  BOOST_REQUIRE(json_customer == json_customer_after_reserialize);

  // testing an adapted skill alone from plain json
  nlohmann::json obj {
    {"skill_name", "GML"},
    {"experience", 10}
  };

  auto skill_parsed = pre::json::from_json<datamodel::skill>(obj);

  BOOST_REQUIRE(skill_parsed.skill_name == "GML");
  BOOST_REQUIRE(skill_parsed.experience == 10);
  std::cout << pre::json::to_json(skill_parsed).dump(2) << std::endl;
}

/**
 * Test nested objects
 */
BOOST_AUTO_TEST_CASE (nested) {
  datamodel::customer customer {
    "Mr. Dupond",
    43,
    {1,5},
    {datamodel::skill{"C++", 10}, datamodel::skill{"GML", 20}, datamodel::skill{"Linux", 2}}
  };

  datamodel::sales_assitant assistant{"Mr. Gold", 130000, customer};
  auto json_sales_assistant = pre::json::to_json(assistant);
  std::cout << json_sales_assistant.dump(2) << std::endl;

  auto deser_assistant = pre::json::from_json<datamodel::sales_assitant>(json_sales_assistant);

  auto json_sales_assistant_reserialized = pre::json::to_json(deser_assistant);
  std::cout << json_sales_assistant_reserialized.dump(2) << std::endl;

  BOOST_REQUIRE(json_sales_assistant_reserialized == json_sales_assistant);
}

BOOST_AUTO_TEST_CASE (working_plain_json) {
  std::string json = R"(
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

  )";

  auto deser_assistant = pre::json::from_json<datamodel::sales_assitant>(json);

  auto json_sales_assistant_reserialized = pre::json::to_json(deser_assistant);
  std::cout << json_sales_assistant_reserialized.dump(2) << std::endl;
}

BOOST_AUTO_TEST_CASE (incorrect_plain_json) {
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

    auto deser_assistant = pre::json::from_json<datamodel::sales_assitant>(json);
    // Must fail
    BOOST_REQUIRE(false);

  } catch (...) {
    std::cout << "EXCEPTION which will be outputed to library user : " << std::endl;
    std::cout << boost::current_exception_diagnostic_information(true) << std::endl;
    BOOST_REQUIRE(true);
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

    auto deser_assistant = pre::json::from_json<datamodel::sales_assitant>(json);
    // Must fail
    BOOST_REQUIRE(false);

  } catch (...) {
    std::cout << "EXCEPTION which will be outputed to library user : " << std::endl;
    std::cout << boost::current_exception_diagnostic_information(true) << std::endl;
    BOOST_REQUIRE(true);
  }
}


BOOST_AUTO_TEST_CASE (containers_direct) {
  std::vector<datamodel::skill> skills {
    datamodel::skill{"C++", 0},
    datamodel::skill{"Java", 0},
    datamodel::skill{".Net", 0},
    datamodel::skill{"Objective-C", 0},
    datamodel::skill{"HTML", 0},
    datamodel::skill{"SQL", 0},
    datamodel::skill{"Project-Management", 0}
  };

  auto skills_json = pre::json::to_json(skills);
  std::cout << skills_json.dump(2) << std::endl;

  auto skills_deserialized = pre::json::from_json<std::vector<datamodel::skill>>(skills_json); 
  BOOST_REQUIRE(skills == skills_deserialized);
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

BOOST_AUTO_TEST_CASE (boost_variant) {

  std::vector<datamodel::employee> employees {
    {"King", datamodel::cashier{"hardware", 1} },
    {"Blake", datamodel::security{true, "Krav Maga"} },
    {"Martin", datamodel::cleaner{"5th floor", "Toys, Petshop, Drugs, Food" } },
    {"Ward", datamodel::cashier{"Food", 2} }
  };

  auto employees_json = pre::json::to_json(employees);
  std::cout << employees_json.dump(2) << std::endl;

  auto employees_deserialized = pre::json::from_json<std::vector<datamodel::employee>>(employees_json); 

  auto employees_reserialized = pre::json::to_json(employees_deserialized);
  std::cout << employees_reserialized.dump(2) << std::endl;

  BOOST_REQUIRE(employees == employees_deserialized);
}

namespace datamodel {

  struct struct_with_a_map {
    std::map<std::string, int> with_a_map;
    std::map<std::string, skill> some_other_map;
  };
  
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::struct_with_a_map, with_a_map, some_other_map)

BOOST_AUTO_TEST_CASE(maps) {

  datamodel::struct_with_a_map some{
    {
      {"hellO",12},
      {"help", 15}
    },
    {
      {"test", datamodel::skill{"C++", 10}}
    }
  };

  auto some_json = pre::json::to_json(some);
  std::cout << some_json.dump(2) << std::endl; 

  auto some_deserialized = pre::json::from_json<datamodel::struct_with_a_map>(some_json);
  
  BOOST_REQUIRE(some_deserialized == some); 
}


namespace datamodel {

  enum class my_value_type : uint8_t {
    TYPE1,
    TYPE2
  };

  enum some_other {
    ONE,
    TWO,
    THREE
  };

  struct some_value {
    my_value_type type;
    int value;
    some_other enums_without_underlying;
  };

}

BOOST_FUSION_ADAPT_STRUCT(datamodel::some_value,
  type,
  value,
  enums_without_underlying)

BOOST_AUTO_TEST_CASE (enums) {

  datamodel::some_value val{datamodel::my_value_type::TYPE2, 12, datamodel::THREE};

  auto val_json = pre::json::to_json(val);
  std::cout << val_json.dump(2) << std::endl;

  auto val_deserialized = pre::json::from_json<datamodel::some_value>(val_json); 

  auto val_reserialized = pre::json::to_json(val_deserialized);
  std::cout << val_reserialized.dump(2) << std::endl;

  BOOST_REQUIRE(val == val_deserialized);
}

BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_chrono) {

  boost::chrono::milliseconds val{100};

  auto val_json = pre::json::to_json(val);
  std::cout << val_json.dump(2) << std::endl;

  auto val_deserialized = pre::json::from_json<boost::chrono::milliseconds>(val_json); 

  auto val_reserialized = pre::json::to_json(val_deserialized);
  std::cout << val_reserialized.dump(2) << std::endl;

  BOOST_REQUIRE(val == val_deserialized);
}

namespace datamodel {
  struct some_time {
    std::string label;
    boost::chrono::milliseconds millis;
    std::chrono::minutes mins;
  };

}

BOOST_FUSION_ADAPT_STRUCT(datamodel::some_time,
  label,
  millis,
  mins)

BOOST_AUTO_TEST_CASE (adapted_struct_jsonize_chrono_composed) {

  datamodel::some_time val{"my time", boost::chrono::milliseconds(100), std::chrono::minutes(43)};

  auto val_json = pre::json::to_json(val);
  std::cout << val_json.dump(2) << std::endl;

  auto val_deserialized = pre::json::from_json<datamodel::some_time>(val_json); 

  auto val_reserialized = pre::json::to_json(val_deserialized);
  std::cout << val_reserialized.dump(2) << std::endl;

  BOOST_REQUIRE(val == val_deserialized);
}

namespace datamodel {
  struct with_optional_member {
    boost::optional<double> field_optional; 
    double other; 
  };
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::with_optional_member,
  field_optional,
  other)

BOOST_AUTO_TEST_CASE (optional) {

  boost::optional<std::string> val{"some text"};

  auto val_json = pre::json::to_json(val);
  std::cout << val_json.dump(2) << std::endl;

  auto val_deserialized = pre::json::from_json<boost::optional<std::string>>(val_json); 

  auto val_reserialized = pre::json::to_json(val_deserialized);
  std::cout << val_reserialized.dump(2) << std::endl;

  BOOST_REQUIRE(val == val_deserialized);
}

BOOST_AUTO_TEST_CASE (optional_member) {

  {
    datamodel::with_optional_member val;
    val.field_optional = 12.32;
    val.other = 13.21;

    auto val_json = pre::json::to_json(val);
    std::cout << val_json.dump(2) << std::endl;

    auto val_deserialized = pre::json::from_json<datamodel::with_optional_member>(val_json); 

    auto val_reserialized = pre::json::to_json(val_deserialized);
    std::cout << val_reserialized.dump(2) << std::endl;

    BOOST_REQUIRE(val == val_deserialized);
  }

  {
    datamodel::with_optional_member val;
    val.other = 43.21;

    auto val_json = pre::json::to_json(val);
    std::cout << val_json.dump(2) << std::endl;

    auto val_deserialized = pre::json::from_json<datamodel::with_optional_member>(val_json); 

    auto val_reserialized = pre::json::to_json(val_deserialized);
    std::cout << val_reserialized.dump(2) << std::endl;

    BOOST_REQUIRE(val == val_deserialized);
  }
}


namespace datamodel {
  typedef mapbox::util::variant<cleaner, cashier, security> possible_responsibilities_mapbox;

  struct with_mapbox_variant{
    possible_responsibilities_mapbox responsibility;
    boost::optional<possible_responsibilities_mapbox> additional_responsibility;
  };
}

BOOST_FUSION_ADAPT_STRUCT(datamodel::with_mapbox_variant,
  responsibility,
  additional_responsibility)


BOOST_AUTO_TEST_CASE (mapbox_variant_simple) {

  {
    using datamodel::with_mapbox_variant;
    using datamodel::possible_responsibilities_mapbox;
    using datamodel::cashier;
    using datamodel::cleaner;
    using datamodel::security;

    with_mapbox_variant val{cashier{"hardware", 1} } ;

    auto val_json = pre::json::to_json(val);
    std::cout << val_json.dump(2) << std::endl;

    auto val_deserialized = pre::json::from_json<decltype(val)>(val_json); 

    auto val_reserialized = pre::json::to_json(val_deserialized);
    std::cout << val_reserialized.dump(2) << std::endl;

    BOOST_REQUIRE(val == val_deserialized);
  }


}


BOOST_AUTO_TEST_CASE (mapbox_variant) {

  {
    using datamodel::with_mapbox_variant;
    using datamodel::possible_responsibilities_mapbox;
    using datamodel::cashier;
    using datamodel::cleaner;
    using datamodel::security;

    std::vector<with_mapbox_variant> val {
      with_mapbox_variant{cashier{"hardware", 1} },
      with_mapbox_variant{security{true, "Krav Maga"}, possible_responsibilities_mapbox{cleaner{"Entrance", "Doors" }}},
      with_mapbox_variant{cleaner{"5th floor", "Toys, Petshop, Drugs, Food" } },
      with_mapbox_variant{cashier{"Food", 2} }
    };

    auto val_json = pre::json::to_json(val);
    std::cout << val_json.dump(2) << std::endl;

    auto val_deserialized = pre::json::from_json<decltype(val)>(val_json); 

    auto val_reserialized = pre::json::to_json(val_deserialized);
    std::cout << val_reserialized.dump(2) << std::endl;

    BOOST_REQUIRE(val == val_deserialized);
  }


}
