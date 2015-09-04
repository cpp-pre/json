#ifndef BOOST_FUSION_DETAIL_STD_CONTAINERS_OSTREAM_OPERATORS
#define BOOST_FUSION_DETAIL_STD_CONTAINERS_OSTREAM_OPERATORS 

#include <iterator>
#include <ostream>
#include <type_traits>
#include <pre/boost/fusion/traits/is_container.hpp>

namespace boost { namespace fusion { namespace std_containers_ostream_operators {

  template<class T, 
    typename std::enable_if<
      boost::fusion::traits::is_container<T>::value
    >::type* = nullptr>
  inline std::ostream& operator<<(std::ostream& os, const T& value) {
    auto end = std::begin(value);
    std::advance(end, value.size() - 1);

    os << "[";
    for (typename T::const_iterator iter=std::begin(value); iter!=std::end(value); ++iter) {
      os << *iter << ( (iter != end) ? ", " : "");
    }
    os << "]";

    return os;
  }


}
  

}}


#endif
