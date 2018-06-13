#pragma once

namespace pre::variant {

  template< template<class...> class Variant, class F, class... Types>
  inline void for_each_type( Variant<Types...>& variant, F&& f) {
    (void)std::initializer_list<int>{(std::ref(f)(std::forward<Types>(Types{})),0)...};
  }

  template< template<class...> class Variant, class F, class... Types>
  inline void for_each_type( const Variant<Types...>& variant, F&& f) {
    (void)std::initializer_list<int>{(std::ref(f)(std::forward<Types>(Types{})),0)...};
  }

}
