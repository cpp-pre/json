#ifndef PRE_VARIANT_FOR_EACH_TYPE_HPP
#define PRE_VARIANT_FOR_EACH_TYPE_HPP

#include <boost/variant.hpp>
#include <mapbox/variant.hpp>

namespace pre { namespace variant {

  template< template<class...> class Variant, class F, class... Types>
  inline void for_each_type( Variant<Types...>& variant, F&& f) {
    (void)std::initializer_list<int>{(std::ref(f)(std::forward<Types>(Types{})),0)...};
  }

  template< template<class...> class Variant, class F, class... Types>
  inline void for_each_type( const Variant<Types...>& variant, F&& f) {
    (void)std::initializer_list<int>{(std::ref(f)(std::forward<Types>(Types{})),0)...};
  }

}}

#endif
