#ifndef PRE_VARIANT_GET_TRAIT_HPP
#define PRE_VARIANT_GET_TRAIT_HPP

#include <boost/variant.hpp>
#include <boost/type_traits.hpp>

#include <pre/json/traits/is_boost_variant.hpp>
#include <pre/variant/detail/assignable_from.hpp>
#include <pre/variant/apply_visitor.hpp>

#include <mapbox/traits/is_mapbox_variant.hpp>


namespace pre { namespace variant { 

  namespace detail {
    
    template<class ResultVariant, template<class T> class TraitMetafunction>
    struct get_trait_visitor : public boost::static_visitor<ResultVariant> {

      template< class U, 
        typename boost::disable_if<
          assignable_from<ResultVariant, typename TraitMetafunction<U>::type>
        >::type* = nullptr
      > ResultVariant operator()(const U&) const {
        return ResultVariant{};
      }

      template< class U, 
        typename boost::enable_if<
          assignable_from<ResultVariant, typename TraitMetafunction<U>::type>
        >::type* = nullptr
      > ResultVariant operator()(const U&) const { 
        return typename TraitMetafunction<U>::type{};
      }
    };


    template<class Result, template<class T> class TraitMetafunction>
    struct get_value_trait_visitor : public boost::static_visitor<Result> {

      template< class U >
      Result operator()(const U&) const { 
        return TraitMetafunction<U>::type::value;
      }


    };
  }

  /**
   * \brief Runtime compile time traits access. 
   *        This function gets the active type of a variant and return a runtime
   *        instantiation of the associated type found by
   *        TraitMetafunction<active type>. 
   *
   * \param variant The variant whose active type will be queried for the given
   *                TraitMetafunction
   *
   * \param Result A boost::variant or any type capable to hold the runtime 
   *                     instance of the TraitMetafunction result.
   *
   * \param TraitMetafunction Template metafunction which will be called on the 
   *                          active type of the runtime parameter : variant.
   *
   * \param InspectedVariant A boost::variant to apply the TraitMetafunction on it's
   *                         active field.
   *
   */
  template<class Result, template<class T> class TraitMetafunction, class InspectedVariant,
    typename std::enable_if< 
         pre::json::traits::is_boost_variant<Result>::value 
      || mapbox::traits::is_mapbox_variant<Result>::value 
    >::type* = nullptr
  > 
  inline Result get_trait(const InspectedVariant& variant) {
    return apply_visitor(detail::get_trait_visitor<Result, TraitMetafunction>{}, variant);
  }

  template<class Result, template<class T> class TraitMetafunction, class InspectedVariant,
    typename std::enable_if< 
      !pre::json::traits::is_boost_variant<Result>::value && 
      !mapbox::traits::is_mapbox_variant<Result>::value 
    >::type* = nullptr
  >
  inline Result get_trait(const InspectedVariant& variant) {
    return apply_visitor(detail::get_value_trait_visitor<Result, TraitMetafunction>{}, variant);
  }
  
}}

#endif
