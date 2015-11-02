#ifndef PRE_CV_FORCE_INBOUND_POLICY_HPP
#define PRE_CV_FORCE_INBOUND_POLICY_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/date_time/constrained_value.hpp>

namespace pre { namespace CV { 

  /**
   * \brief A constrained_value policy which set the value to the nearest one
   *        if it's out-of-bound.
   */
  template<class rep_type, const long int min_value, const long int max_value>
  struct force_inbound_policy {
    public:
      typedef rep_type value_type;
      static rep_type min() noexcept { return rep_type(min_value); }
      static rep_type max() noexcept { return rep_type(max_value); }
      static void on_error(rep_type, rep_type& new_value, boost::CV::violation_enum violation) noexcept {
        if (violation == boost::CV::min_violation) {
          new_value = min();
        } else if (violation == boost::CV::max_violation) {
          new_value = max();
        }
      }
    };

}}

#endif
