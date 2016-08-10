#ifndef PRE_RANGE_ERASE_IF_HPP
#define PRE_RANGE_ERASE_IF_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <boost/range/config.hpp>
#include <boost/range/concepts.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/assert.hpp>

namespace pre {


  namespace range {

    template< class Container, class Pred >
    inline Container& erase_if( Container& on, Pred pred )
    {
        typedef BOOST_DEDUCED_TYPENAME Container::const_iterator iterator_t;

        iterator_t it_end = ::boost::end(on);
        for (iterator_t it = ::boost::begin(on); it != it_end;) {
          if (pred(*it)) { it = on.erase(it); } else { ++it; }
        }

        return on;
    }

  } // namespace range

  using range::erase_if;
} // namespace pre 

#endif
