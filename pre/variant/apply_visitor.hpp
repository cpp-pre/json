#pragma once

#include <type_traits>
#include <variant>

#include <pre/variant/traits/is_std_variant.hpp>

namespace pre::variant { 

  // std variant

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_std_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(const VisitorType& visitor, const VariantType& variant) -> decltype(std::visit(visitor,variant)) {
    return std::visit(visitor,variant);
  } 

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_std_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(const VisitorType& visitor, VariantType& variant) -> decltype(std::visit(visitor,variant)) {
    return std::visit(visitor,variant);
  } 

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_std_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(VisitorType& visitor, const VariantType& variant) -> decltype(std::visit(visitor,variant)) {
    return std::visit(visitor,variant);
  } 

  template<class VisitorType, class VariantType,
    typename std::enable_if< pre::variant::traits::is_std_variant<VariantType>::value >::type* = nullptr>
  inline auto apply_visitor(VisitorType& visitor, VariantType& variant)  -> decltype(std::visit(visitor,variant)) {
    return std::visit(visitor,variant);
  }


}
