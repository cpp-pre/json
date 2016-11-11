#ifndef PRE_VARIANT_DETAIL_ASSIGNABLE_FROM_HPP
#define PRE_VARIANT_DETAIL_ASSIGNABLE_FROM_HPP

#include <boost/mpl/contains.hpp>

#include <pre/json/traits/is_boost_variant.hpp>

#include <mapbox/traits/is_mapbox_variant.hpp>
#include <mapbox/traits/is_type_in_variant.hpp>

namespace pre { namespace variant { namespace detail {

  namespace mpl = ::boost::mpl;

  template <class VariantType, class T, class Enable = void> 
  struct assignable_from;

  template <class VariantType, class T>
  struct assignable_from< VariantType, T, 
    typename std::enable_if< pre::json::traits::is_boost_variant<VariantType>::value >::type
  > : public mpl::contains<typename VariantType::types, T>::type 
  {};

  template <class VariantType, class T>
  struct assignable_from< VariantType, T, 
    typename std::enable_if< mapbox::traits::is_mapbox_variant<VariantType>::value >::type
  > : public mapbox::traits::is_type_in_variant<T, VariantType>::type 
  {};

}}}

#endif
