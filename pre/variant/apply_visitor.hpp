#ifndef PRE_VARIANT_APPLY_VISITOR_HPP
#define PRE_VARIANT_APPLY_VISITOR_HPP

#include <type_traits>

#include <boost/variant.hpp>
#include <variant>

#include <pre/variant/traits/is_boost_variant.hpp>

namespace pre { namespace variant { 

  // Boost.Variant
  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_boost_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(const VisitorType& visitor, const VariantType& variant) -> decltype(variant.apply_visitor(visitor)) {
    return variant.apply_visitor(visitor);
  }

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_boost_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(const VisitorType& visitor, VariantType& variant) -> decltype(variant.apply_visitor(visitor)) {
    return variant.apply_visitor(visitor);
  }

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_boost_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(VisitorType& visitor, const VariantType& variant) -> decltype(variant.apply_visitor(visitor)) {
    return variant.apply_visitor(visitor);
  }

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_boost_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(VisitorType& visitor, VariantType& variant) -> decltype(variant.apply_visitor(visitor)) {
    return variant.apply_visitor(visitor);
  }


}}

#endif
