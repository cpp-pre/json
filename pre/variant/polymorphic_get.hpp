#ifndef PRE_VARIANT_POLYMORPHIC_GET_HPP
#define PRE_VARIANT_POLYMORPHIC_GET_HPP

#include <boost/variant/polymorphic_get.hpp>
#include <mapbox/variant.hpp>

namespace pre { namespace variant {
  
  using ::boost::polymorphic_get;

  template <class T, class... Types>
  inline const T& polymorphic_get(const mapbox::util::variant<Types...>& variant);

  template <class T, class... Types>
  inline T& polymorphic_get(mapbox::util::variant<Types...>& variant);

  namespace detail {
    template<class ParentType>
    struct polymorphic_getter {

      template<class U, typename std::enable_if< std::is_base_of<ParentType, U>::value >::type* = nullptr>
      ParentType& operator()(U& value) {
        return value; 
      }

      template<class U, typename std::enable_if< ! std::is_base_of<ParentType, U>::value >::type* = nullptr>
      ParentType& operator()(U& value) {
        throw mapbox::util::bad_variant_access("in polymorphic_get<T>()");
      }
    };
  }

  template <class T, class... Types>
  inline const T& polymorphic_get(const mapbox::util::variant<Types...>& variant) {
    return mapbox::util::apply_visitor(detail::polymorphic_getter<T>{}, variant);
  }

  template <class T, class... Types>
  inline T& polymorphic_get(mapbox::util::variant<Types...>& variant) {
    return mapbox::util::apply_visitor(detail::polymorphic_getter<T>{}, variant);
  }

}}

#endif
