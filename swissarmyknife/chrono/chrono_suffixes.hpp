#ifndef SWISSARMYKNIFE_CHRONO_CHRONOSUFFIXES_HPP
#define SWISSARMYKNIFE_CHRONO_CHRONOSUFFIXES_HPP

#include <swissarmyknife/detail/config.hpp>

#include <boost/chrono.hpp>

namespace swissarmyknife { namespace chrono {

  constexpr boost::chrono::milliseconds operator "" _ms(
      unsigned long long ms) {
    return boost::chrono::milliseconds(ms);
  }

  constexpr boost::chrono::seconds operator "" _sec(
      unsigned long long sec) {
    return boost::chrono::seconds(sec);
  }

  constexpr boost::chrono::milliseconds operator "" _min(
      unsigned long long min) {
    return boost::chrono::minutes(min);
  }

}}

#endif
