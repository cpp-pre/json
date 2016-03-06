/**
 * Copyright 2014, Joel FALCOU <joel.falcou@lri.fr>
 *
 * \brief Provides access to lambda arity, return type, argumets type and access as std::function type:
 *
 *
 * #include <iostream>
 * #include <typeinfo>
 * #include <pre/traits/function_traits.hpp>
 *
 * int main() {
 *
 *     auto my_lambda = [](bool arg_0, int arg_1, double arg_2, std::string arg_3) {
 *                             return int{0} ;
 *                         };
 *
 *     typedef pre::function_traits<decltype(my_lambda)> my_lambda_types;
 *
 *     std::cout << "my_lambda number of arguments " << my_lambda_types::arity                        << std::endl;
 *     std::cout << "my_lambda return type is "      << typeid(my_lambda_types::result_type).name()   << std::endl;
 *     std::cout << "my_lambda argument 0 type is "  << typeid(my_lambda_types::arg<0>).name()        << std::endl;
 *     std::cout << "my_lambda argument 1 type is "  << typeid(my_lambda_types::arg<1>).name()        << std::endl;
 *     std::cout << "my_lambda argument 2 type is "  << typeid(my_lambda_types::arg<2>).name()        << std::endl;
 *     std::cout << "my_lambda argument 3 type is "  << typeid(my_lambda_types::arg<3>).name()        << std::endl;
 *     std::cout << "my_lambda function type is "    << typeid(my_lambda_types::function_type).name() << std::endl;
 *
 *     auto std_function = pre::to_std_function(my_lambda);
 *
 *     std::cout << "my_lambda called as std::function "  << std_function(true, int{42}, double{3.14}, std::string{"Hello World"}) << std::endl;
 *
 *     return 0;
 * }
 */
#ifndef PRE_TRAITS_FUNCTION_TRAITS_HPP
#define PRE_TRAITS_FUNCTION_TRAITS_HPP

 
#include <functional>
#include <tuple>


namespace pre {
    
    
  template<typename F>
  struct function_traits : public function_traits<decltype(&F::operator())> {
  };

  template<typename ClassType, typename ReturnType, typename... Args>
  struct function_traits<ReturnType(ClassType::*)(Args...) const> {
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


  template <class T>
  auto to_std_function (T t) -> typename function_traits<T>::function_type  {
    return typename function_traits<T>::function_type(t);
  }
  

} //pre


#endif /*PRE_TRAITS_FUNCTION_TRAITS_HPP*/