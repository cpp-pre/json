#pragma once

#include <variant>

namespace pre::variant::traits {

  /**
   * \return whether the given Container is a standard container
   */
  template <typename Container>
  struct is_std_variant : std::false_type { };

  template <typename... Ts> struct is_std_variant<std::variant<Ts...> > : std::true_type { };

}
