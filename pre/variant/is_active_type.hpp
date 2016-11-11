#ifndef PRE_VARIANT_IS_ACTIVE_TYPE_HPP
#define PRE_VARIANT_IS_ACTIVE_TYPE_HPP

#include <boost/variant.hpp>
#include <mapbox/variant.hpp>

namespace pre { namespace variant { 
 
  /**
   * \brief is_active_type is meant to be used on a boost::variant.
   * \param variant the Variant to check which type is active.
   * \param T the type that is checked for being active.
   */
  template<class T, class... Types>
  inline bool is_active_type(const boost::variant<Types...>& variant) {
    return ::boost::get<T>(std::addressof(variant)) != nullptr;
  }

  /**
   * \brief is_active_type is meant to be used on a boost::variant.
   * \param variant the Variant to check which type is active.
   * \param T the type that is checked for being active.
   */
  template<class T, class... Types>
  inline bool is_active_type(const mapbox::util::variant<Types...>& variant) {
    return variant.template is<T>();
  }

}}

#endif
