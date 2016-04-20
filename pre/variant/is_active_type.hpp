#ifndef PRE_VARIANT_IS_ACTIVE_TYPE
#define PRE_VARIANT_IS_ACTIVE_TYPE

#include <boost/variant.hpp>

namespace pre { namespace variant { 
 
  /**
   * \brief is_active_type is meant to be used on a boost::variant.
   * \param variant the Variant to check which type is active.
   * \param T the type that is checked for being active.
   */
  template<class T, class Variant>
  inline bool is_active_type(const Variant& variant) {
    return boost::get<T>(std::addressof(variant)) != nullptr;
  }

}}

#endif
