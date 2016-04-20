#ifndef PRE_VARIANT_DETAIL_ASSIGNABLE_FROM_HPP
#define PRE_VARIANT_DETAIL_ASSIGNABLE_FROM_HPP

#include <boost/mpl/contains.hpp>

namespace pre { namespace variant { namespace detail {

  namespace mpl = boost::mpl;

  template <class SelectedTypeVariant, class T>
  using assignable_from  = 
    typename mpl::contains<typename SelectedTypeVariant::types, T>::type;


}}}

#endif
