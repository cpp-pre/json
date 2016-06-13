#ifndef PRE_RETRY_FOR_HPP
#define PRE_RETRY_FOR_HPP

#include <cassert>
#include <functional>
#include <chrono>
#include <thread>

namespace pre {


  inline namespace retry_v1 {

    /**
     * \brief Sometimes you need to retry some operations, as it's normal that they may fail.
     *        Typically an IO error is not always a real error, as the media may 
     *        be known to be instable (e.g. airwaves communication, speaking over 
     *        serial line when remote device is busy...)
     *
     *        Typically if there are many retries tried out without any pause in between, the
     *        operation may still fail, as the main reason for something not to reply is to be
     *        busy. Therefore this function allows you to specify blackout time as well as a
     *        complete retry time.
     *
     *        **Preconditions**: blackout_step must be a multiple of total_time.
     *
     * \param total_time Total time allowed for retrying.
     * \param time_step Wait time between retries.
     * \param retried_func Callback retried returning true on success, false on error.
     *
     * \return Whether the callback succeeded or not.
     */
    template< class Rep, class Period >
    inline bool retry_for(
        const std::chrono::duration<Rep, Period>& total_time,
        const std::chrono::duration<Rep, Period>& time_step,
        std::function<bool (void)> retried_func) {
      assert(time_step.count()!=0);
      assert(total_time.count() >= time_step.count());

      size_t iterations = total_time.count() / time_step.count();
      for(size_t p=0; p <iterations; p++) {
        if(retried_func()) { return true; }
        std::this_thread::sleep_for(time_step);
      }
      return false;
    }

    /**
     * \brief Equivalent to :
     *        sleep_for(initial_blackout); pre::retry_for(total_time, time_step, retried_func);
     *  
     *        Please see [pre::retry_for](@ref pre::retry_for(const std::chrono::duration<Rep, Period>&, 
     *          const std::chrono::duration<Rep, Period>& , std::function<bool (void)>)).
     */
    template< class Rep, class Period >
    inline bool retry_for(
        const std::chrono::duration<Rep, Period>& total_time,
        const std::chrono::duration<Rep, Period>& time_step,
        const std::chrono::duration<Rep, Period>& initial_blackout,
        std::function<bool (void)> retried_func) {
      std::this_thread::sleep_for(initial_blackout);
      return retry_for(total_time, time_step, retried_func);
    }

  }

}

#endif
