#ifndef BOOST_FUSION_FOR_EACH_MEMBER_HPP
#define BOOST_FUSION_FOR_EACH_MEMBER_HPP

#include <type_traits>
#include <functional>

#include <boost/format.hpp>
#include <boost/fusion/include/adapt_struct.hpp> 

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/include/tag_of.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion {

namespace detail {

  template <typename First, typename Last, typename F, typename Sequence>
  inline void
  for_each_member_linear(First const& first, 
      Last const& last, 
      F const& f,
      Sequence&,
      boost::mpl::true_) {}

  template <typename First, typename Last, typename F, typename Sequence,
  typename std::enable_if< 
    std::is_same<
      typename boost::fusion::traits::tag_of<Sequence>::type,
      boost::fusion::struct_tag
    >::value
  >::type* = nullptr>
  inline void
  for_each_member_linear(First & first, 
      Last const& last,
      F const& f,
      Sequence& seq,
      boost::mpl::false_) {

      f(
				extension::struct_member_name<
					typename std::remove_cv< typename First::seq_type>::type , First::index::value
				>::call(), 
				*first
			);

      for_each_member_linear(
          next(first), 
          last,
          f,
          seq,
          result_of::equal_to< typename result_of::next<First>::type, Last>()
      );
  }


 template <typename First, typename Last, typename F, typename Sequence,
  typename std::enable_if< 
    std::is_same<
      typename boost::fusion::traits::tag_of<Sequence>::type,
      boost::fusion::boost_tuple_tag
    >::value
  >::type* = nullptr>
  inline void
  for_each_member_linear(First & first, 
      Last const& last,
      F const& f,
      Sequence& seq,
      boost::mpl::false_) {

      auto index = result_of::distance<
                     typename result_of::begin<Sequence>::type, 
                     First
                   >::type::value; 
      std::string name =  typeid(decltype(*first)).name();

      f(
				str(format("%1% (%2%)") % index % name ).data(), 
				*first
			);

      for_each_member_linear(
          next(first), 
          last,
          f,
          seq,
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
      seq,
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
