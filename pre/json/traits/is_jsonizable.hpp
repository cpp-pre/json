#ifndef PRE_JSON_TRAITS_IS_JSONIZABLE_HPP
#define PRE_JSON_TRAITS_IS_JSONIZABLE_HPP

#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

#include <type_traits>
#include <string>

#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/tag_of.hpp>
#include <boost/fusion/include/struct.hpp>

#include <pre/variant/traits/is_std_variant.hpp>
#include <pre/json/traits/is_container.hpp>
#include <pre/json/traits/is_associative_container.hpp>
#include <pre/json/traits/is_string.hpp>

namespace pre { namespace json { namespace traits {

  namespace mpl = boost::mpl;
  namespace fusion = boost::fusion; 

  template <typename T, class Enable = void>
  struct is_jsonizable : std::false_type {};


  // Any container of jsonizable
  template <typename T>
  struct is_jsonizable<T, 
    typename std::enable_if<
      mpl::and_<
        typename traits::is_container<T>::type,
        typename is_jsonizable<typename T::value_type>::type
      >::value
    >::type> : std::true_type {};

  // Any associative container with a key of type compatible to string and a jsonizable value
  template <typename T>
  struct is_jsonizable<T, 
    typename std::enable_if<
      mpl::and_<
        typename traits::is_associative_container<T>::type,
        typename std::is_convertible<typename T::key_type, std::string>::type,
        typename is_jsonizable<typename T::mapped_type>::type
      >::value
    >::type> : std::true_type {};

  template <typename T>
  struct is_jsonizable<T,
    typename std::enable_if<
      mpl::or_<
        // Any adapted type
        typename fusion::traits::is_sequence<T>::type,

        // Any boost variant
        typename pre::variant::traits::is_std_variant<T>::type,

        // Integral types
        typename mpl::or_<
          typename std::is_convertible<T, std::string>::type,
          typename std::is_arithmetic<T>::type
        >::type
      >::value
    >::type> : std::true_type {};

}}}

#endif
