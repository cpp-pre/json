/**
 * Copyright 2014, Joel FALCOU <joel.falcou@lri.fr>
 * Copyright 2016, Daniel Friedrich <daniel@duerrenbuehl.de> 
 * Copyright 2016, Damien BUHL <damien.buhl@lecbna.org>
 *
 */
#ifndef PRE_TYPE_TRAITS_DETAIL_FUNCTION_TRAITS_IMPL_HPP
#define PRE_TYPE_TRAITS_DETAIL_FUNCTION_TRAITS_IMPL_HPP

#include <type_traits>
#include <functional>
#include <tuple>

namespace pre { namespace type_traits { namespace detail {

    template<typename F>
    using disable_if_is_function_t = 
      typename std::enable_if< not std::is_function< typename std::remove_pointer<F>::type >::value >::type;

    template<typename F>
    using enable_if_is_function_t = 
      typename std::enable_if< std::is_function< typename std::remove_pointer<F>::type >::value >::type;

    template<typename... FunctionSignature>
    struct function_traits_impl;

    template<typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(Args...)> {
      enum {
          arity = sizeof...(Args)
      };

      using result_type = ReturnType;

      using function_type = std::function<ReturnType(Args...)>;

    private:
      template<size_t i>
      struct arg_ {
          typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
      };

    public:
      template<size_t i>
      using arg = typename arg_<i>::type;
    };


    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const> {
      enum {
          arity = sizeof...(Args)
      };

      using result_type = ReturnType;

      using function_type = std::function<ReturnType(Args...)>;

    private:
      template<size_t i>
      struct arg_ {
          typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
      };

    public:
      template<size_t i>
      using arg = typename arg_<i>::type;
    };

}}}

#endif
