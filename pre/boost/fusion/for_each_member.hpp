#ifndef BOOST_FUSION_FOR_EACH_MEMBER_HPP
#define BOOST_FUSION_FOR_EACH_MEMBER_HPP

#include <functional>

#include <boost/fusion/include/adapt_struct.hpp> 

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion {

namespace detail {

  template <typename First, typename Last, typename F>
  inline void
  for_each_member_linear(First const& first, 
      Last const& last, 
      F const& f,
      boost::mpl::true_) {}

  template <typename First, typename Last, typename F>
  inline void
  for_each_member_linear(First const& first, 
      Last const& last,
      F const& f,
      boost::mpl::false_) {

      f(
				extension::struct_member_name<
					typename First::seq_type, First::index::value
				>::call(), 
				*first
			);

      for_each_member_linear(
          next(first), 
          last,
          f,
          result_of::equal_to< typename result_of::next<First>::type, Last>()
      );
  }

  template <typename Sequence, typename F>
  inline void
  for_each_member(Sequence& seq, F const& f) {

    detail::for_each_member_linear(
      fusion::begin(seq), 
      fusion::end(seq), 
      f, 
      result_of::equal_to<
        typename result_of::begin<Sequence>::type, 
        typename result_of::end<Sequence>::type>()
    );
  }

}

  template <typename Sequence, typename F>
  inline void
  for_each_member(Sequence& seq, F f) {
    detail::for_each_member(seq, f);
  }
 
}}

#endif
