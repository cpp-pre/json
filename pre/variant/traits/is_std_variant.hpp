#ifndef PRE_VARIANT_TRAITS_IS_STD_VARIANT_HPP
#define PRE_VARIANT_TRAITS_IS_STD_VARIANT_HPP

#include <variant>

namespace pre { namespace variant { namespace traits {

  /**
   * \return whether the given Container is a standard container
   */
  template <typename Container>
  struct is_std_variant : std::false_type { };

  template <typename... Ts> struct is_std_variant<std::variant<Ts...> > : std::true_type { };

}}}

#endif
