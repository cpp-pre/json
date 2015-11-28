#ifndef PRE_JSON_TRAITS_IS_BOOST_VARIANT_HPP
#define PRE_JSON_TRAITS_IS_BOOST_VARIANT_HPP

#include <boost/variant.hpp>

namespace pre { namespace json { namespace traits {

  /**
   * \return whether the given Container is a standard container
   */
  template <typename Container>
  struct is_boost_variant : std::false_type { };

  template <typename... Ts> struct is_boost_variant<boost::variant<Ts...> > : std::true_type { };

}}}

#endif
