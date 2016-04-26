#ifndef PRE_VARIANT_OPTIMIZED_BOOST_GET_FOR_HPP
#define PRE_VARIANT_OPTIMIZED_BOOST_GET_FOR_HPP

#include <boost/variant.hpp>
#include <pre/variant/is_active_type.hpp>

#include <boost/type_traits.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/distance.hpp>

#define PRE_VARIANT_OPTIMIZED_BOOST_GET_FOR(VARIANT_TYPE)                 \
                                                                          \
                                                                          \
namespace boost {                                                         \
template <typename U > \
  inline typename boost::add_pointer<const U>::type get(                  \
      const VARIANT_TYPE* operand) {                                      \
                                                                          \
    typedef typename boost::add_pointer<const U>::type U_ptr;             \
                                                                          \
    typedef typename boost::mpl::begin<VARIANT_TYPE::types>::type begin;  \
    typedef typename boost::mpl::find<VARIANT_TYPE::types,U>::type iter;  \
                                                                          \
    if (operand->which() == boost::mpl::distance<begin, iter>::value) {   \
      return reinterpret_cast<U_ptr>(operand->storage_.address());        \
    } else {                                                              \
      return nullptr;                                                     \
    }                                                                     \
                                                                          \
  }                                                                       \
                                                                          \
  template <typename U >                                                  \
  inline typename boost::add_reference<const U>::type get(                \
      const VARIANT_TYPE& operand) {                                      \
                                                                          \
    auto* value = get<U>(std::addressof(operand));                        \
    if (value != nullptr) {                                               \
      return *value;                                                      \
    } else {                                                              \
      boost::throw_exception(boost::bad_get());                           \
    }                                                                     \
                                                                          \
  }                                                                       \
                                                                          \
template <typename U > \
  inline typename boost::add_pointer<U>::type get(                        \
      VARIANT_TYPE* operand) {                                            \
                                                                          \
    typedef typename boost::add_pointer<U>::type U_ptr;                   \
                                                                          \
    typedef typename boost::mpl::begin<VARIANT_TYPE::types>::type begin;  \
    typedef typename boost::mpl::find<VARIANT_TYPE::types,U>::type iter;  \
                                                                          \
    if (operand->which() == boost::mpl::distance<begin, iter>::value) {   \
      return reinterpret_cast<U_ptr>(operand->storage_.address());        \
    } else {                                                              \
      return nullptr;                                                     \
    }                                                                     \
                                                                          \
  }                                                                       \
                                                                          \
  template <typename U >                                                  \
  inline typename boost::add_reference<U>::type get(                      \
      VARIANT_TYPE& operand) {                                            \
                                                                          \
    auto* value = get<U>(std::addressof(operand));                        \
    if (value != nullptr) {                                               \
      return *value;                                                      \
    } else {                                                              \
      boost::throw_exception(boost::bad_get());                           \
    }                                                                     \
                                                                          \
  }                                                                       \
                                                                          \
}                                                                         \
                                                                          \
namespace pre { namespace variant {                                       \
                                                                          \
  template<class U>                                        \
  inline bool is_active_type(const VARIANT_TYPE& variant) {               \
                                                                          \
    typedef typename boost::mpl::begin<VARIANT_TYPE::types>::type begin;  \
    typedef typename boost::mpl::find<VARIANT_TYPE::types,U>::type iter;  \
                                                                          \
    return                                                                \
       (variant.which() == boost::mpl::distance<begin, iter>::value);     \
  }                                                                       \
                                                                          \
}}                                                                        \



#endif
