/**
 * \brief Provides comparison operators for Boost.Ref reference_wrapper.
 */
#ifndef SWISSARMYKNIFE_BOOST_REF_COMPARISON_HPP
#define SWISSARMYKNIFE_BOOST_REF_COMPARISON_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <boost/ref.hpp>

namespace boost {

  template<typename T>
  inline bool operator==(const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs) {
    return lhs.get() == rhs.get();
  }

  template<typename T>
  inline bool operator< (const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs) {
    return lhs.get() < rhs.get();
  }

  template<typename T>
  inline bool operator!=(const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs){
    return lhs.get() != rhs.get();
  }

  template<typename T>
  inline bool operator> (const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs){
    return lhs.get() > rhs.get();
  } 

  template<typename T>
  inline bool operator<=(const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs){
    return lhs.get() <= rhs.get();
  } 

  template<typename T>
  inline bool operator>=(const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs){
    return lhs.get() >= rhs.get();
  }

}

#endif
