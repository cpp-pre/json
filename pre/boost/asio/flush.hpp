#ifndef PRE_BOOST_ASIO_FLUSH_HPP
#define PRE_BOOST_ASIO_FLUSH_HPP

#include <boost/asio/serial_port.hpp>

namespace boost { namespace asio {

  /// @brief Different ways a serial port may be flushed.
  enum flush_type
  {
    flush_receive = TCIFLUSH,
    flush_send = TCIOFLUSH,
    flush_both = TCIOFLUSH
  };

  /// @brief Flush a serial port's buffers.
  ///
  /// @param serial_port Port to flush.
  /// @param what Determines the buffers to flush.
  /// @param error Set to indicate what error occurred, if any.
  void flush_serial_port(
    boost::asio::serial_port& serial_port,
    flush_type what,
    boost::system::error_code& error)
  {
    if (0 == ::tcflush(serial_port.native_handle(), what))
    {
      error = boost::system::error_code();
    }
    else
    {
      error = boost::system::error_code(errno,
          boost::asio::error::get_system_category());
    }
  }

}}

#endif
