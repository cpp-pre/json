#define BOOST_TEST_MODULE retry_for_test 
#include <boost/test/included/unit_test.hpp>

#include <pre/retry_for.hpp>
#include <pre/chrono/chrono_suffixes.hpp>

BOOST_AUTO_TEST_CASE (retry_for_testing) {
  using namespace pre::chrono::std;

  size_t times = 0;
  auto im_happy_only_after_5times = [&times]() {
    ++times;
    std::cout << times << ": I'm " << ((times >= 5) ? "happy" : "unhappy") << std::endl; 
    return (times >= 5);
  };


  {
    times = 0;
    auto start = std::chrono::steady_clock::now();
    pre::retry_for(5_sec, 1_sec, im_happy_only_after_5times);

    BOOST_REQUIRE((std::chrono::steady_clock::now() - start) > 4_sec);
  }

  {
    times = 0;
    auto start = std::chrono::steady_clock::now();
    pre::retry_for(5_ms, 1_ms, 200_ms, im_happy_only_after_5times);

    BOOST_REQUIRE((std::chrono::steady_clock::now() - start) > 200_ms);
  }
}
