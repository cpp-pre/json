#ifndef PRE_JSON_DETAIL_SFINAE_ENABLER_HPP
#define PRE_JSON_DETAIL_SFINAE_ENABLER_HPP

#include <type_traits>

#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/tag_of.hpp>
#include <boost/fusion/include/struct.hpp>

#include <pre/variant/traits/is_std_variant.hpp>
#include <pre/json/traits/is_container.hpp>
#include <pre/json/traits/is_associative_container.hpp>
#include <pre/json/traits/is_string.hpp>

#include <chrono>


namespace pre { namespace json { namespace detail {

    template<class T>
    using enable_if_is_chrono_duration_t = typename std::enable_if< 
      std::is_same<std::chrono::duration<typename T::rep, typename T::period>, T>::value 
    >::type;

    template<class T>
    using enable_if_is_directly_serializable_t = typename std::enable_if<
      std::is_fundamental<T>::value || traits::is_string<T>::value
    ,T>::type;

    template<class T>
    using enable_if_is_variant_t = typename std::enable_if<
      pre::variant::traits::is_std_variant<T>::value
    ,T>::type;

    template<class T>
    using enable_if_is_other_sequence_and_not_variant_t = typename std::enable_if<
      boost::fusion::traits::is_sequence<T>::value
      && !std::is_same< 
            typename boost::fusion::traits::tag_of<T>::type, 
            boost::fusion::struct_tag
          >::value
      && !enable_if_is_variant_t<T>::value 
    ,T>::type;

    template<class T>
    using enable_if_is_adapted_struct_t = typename std::enable_if< 
      std::is_same<
        typename boost::fusion::traits::tag_of<T>::type, 
        boost::fusion::struct_tag
      >::value
    ,T>::type;

    template<class T>
    using enable_if_is_struct_non_adapted_t = typename std::enable_if< 
      ! std::is_same<
        typename boost::fusion::traits::tag_of<T>::type, 
        boost::fusion::struct_tag
      >::value && std::is_aggregate_v<T>
    ,T>::type;

    template<class T>
    using enable_if_is_container_t = typename std::enable_if<
      traits::is_container<T>::value
    ,T>::type;

    template<class T>
    using enable_if_is_associative_container_t = typename std::enable_if<
      traits::is_associative_container<T>::value 
    ,T>::type;

    namespace requirements {
      template<class T, std::size_t N>
      concept has_tuple_element =
        requires(T t) {
          typename std::tuple_element_t<N, std::remove_const_t<T>>;
          { get<N>(t) } -> std::convertible_to<const std::tuple_element_t<N, T>&>;
        };

      template<class T>
      concept tuple_like = !std::is_reference_v<T> 
        && requires(T t) { 
          typename std::tuple_size<T>::type; 
          requires std::derived_from<
            std::tuple_size<T>, 
            std::integral_constant<std::size_t, std::tuple_size_v<T>>
          >;
        } && []<std::size_t... N>(std::index_sequence<N...>) { 
          return (has_tuple_element<T, N> && ...); 
        }(std::make_index_sequence<std::tuple_size_v<T>>());

      template<class T>
      concept not_tuple_like = !tuple_like<T>;
    }
}}}

#endif
