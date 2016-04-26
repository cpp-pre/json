#ifndef PRE_VARIANT_ASSIGN_IF_COMPATIBLE_HPP
#define PRE_VARIANT_ASSIGN_IF_COMPATIBLE_HPP

#include <type_traits> 
#include <boost/variant.hpp>

#include <pre/json/traits/is_boost_variant.hpp>
#include <pre/variant/detail/assignable_from.hpp>

namespace pre { namespace variant { 

  namespace detail {
    template<class ResultVariant>
    struct assign_if_compatible_visitor : public boost::static_visitor<ResultVariant> {

      template< class U, 
        typename std::enable_if<
          !assignable_from<ResultVariant, U>::value
        >::type* = nullptr
      > ResultVariant operator()(const U& u) const {
        //std::cout << "Cannot assign incompatible : " << typeid(U).name() << std::endl;
        return ResultVariant{}; //XXX: how to inform about this ? Variant is always non-empty, we cannot tel it failed. Optional ?
      }

      template< class U, 
        typename std::enable_if<
          assignable_from<ResultVariant, U>::value
        >::type* = nullptr
      > ResultVariant operator()(const U& u) const { 
        //std::cout << "assigning compatible : " << typeid(U).name() << std::endl;
        return ResultVariant{u};
      }
    };

  }

  /**
   * \brief This function gets the active type of a variant and return another
   *        variant type instance filled with the active type if they are 
   *        compatible.
   *
   * \param variant The variant from which the active type will be taken 
   *
   * \param Result Another boost::variant or any type capable to hold the runtime 
   *                     instance of the active type.
   *
   * \param InspectedVariant A boost::variant to apply the TraitMetafunction on it's
   *                         active field.
   *
   */
  template<class Result, class InspectedVariant,
    typename std::enable_if< 
      pre::json::traits::is_boost_variant<InspectedVariant>::value
      && pre::json::traits::is_boost_variant<Result>::value 
    >::type* = nullptr
  > 
  inline Result assign_if_compatible(const InspectedVariant& variant) {
    return boost::apply_visitor(detail::assign_if_compatible_visitor<Result>{}, variant);
  }

}}

#endif
