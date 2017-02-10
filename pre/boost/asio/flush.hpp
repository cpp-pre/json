#ifndef PRE_BOOST_ASIO_FLUSH_HPP
#define PRE_BOOST_ASIO_FLUSH_HPP

#include <boost/asio/serial_port.hpp>
#include <pre/boost/asio/mockup_serial_port_service.hpp>

namespace boost { namespace asio {

#if defined(BOOST_ASIO_WINDOWS)
  const size_t TCIFLUSH = 0;
  const size_t TCOFLUSH = 1;
  const size_t TCIOFLUSH = 2;
#endif

  /// @brief Different ways a serial port may be flushed.
  enum flush_type
  {
    flush_receive = TCIFLUSH,
    flush_send = TCOFLUSH,
    flush_both = TCIOFLUSH
  };

  /// @brief Flush a serial port's buffers.
  ///
  /// @param serial_port Port to flush.
  /// @param what Determines the buffers to flush.
  /// @param error Set to indicate what error occurred, if any.
  inline void flush_serial_port(
    boost::asio::serial_port& serial_port,
    flush_type what,
    boost::system::error_code& error)
  {
#if defined(BOOST_ASIO_WINDOWS)
    // On windows it's a noop for now.
    error = boost::system::error_code();
#else 
    if (0 == ::tcflush(serial_port.native_handle(), what))
    {
      error = boost::system::error_code();
    }
    else
    {
      error = boost::system::error_code(errno,
          boost::asio::error::get_system_category());
    }
#endif
  }

  inline void flush_serial_port(
    basic_serial_port<mockup_serial_port_service>&,
    flush_type,
    boost::system::error_code& error) {
    error = boost::system::error_code();
  }

}}

#endif
