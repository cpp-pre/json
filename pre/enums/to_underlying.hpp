#ifndef PRE_ENUMS_TO_UNDERLYING_HPP
#define PRE_ENUMS_TO_UNDERLYING_HPP

#include <type_traits>

namespace pre { namespace enums {

  /**
   * \return The enum typed value casted to it's underlying_type.
   */
  template <typename E>
  typename std::underlying_type<E>::type to_underlying(E e) {
      return static_cast<typename std::underlying_type<E>::type>(e);
  }

}}  

#endif
