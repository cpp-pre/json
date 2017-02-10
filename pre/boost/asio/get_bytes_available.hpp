#ifndef PRE_BOOST_ASIO_GET_BYTES_AVAILABLE_HPP
#define PRE_BOOST_ASIO_GET_BYTES_AVAILABLE_HPP

#include <boost/asio/serial_port.hpp>
#include <pre/boost/asio/mockup_serial_port_service.hpp>

namespace boost { namespace asio { 

  /// @brief Returns the number of bytes available for reading from a serial
  ///        port without blocking.
 inline std::size_t get_bytes_available(
    boost::asio::serial_port& serial_port,
    boost::system::error_code& error)
  {
    error = boost::system::error_code();
    int value = 0;
#if defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
    COMSTAT status;
    if (0 != ::ClearCommError(serial_port.lowest_layer().native_handle(),
                              NULL, &status))
    {
      value = status.cbInQue;
    }
    // On error, set the error code.
    else
    {
      error = boost::system::error_code(::GetLastError(),
         boost::asio::error::get_system_category());
    }
#else // defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
    if (0 == ::ioctl(serial_port.lowest_layer().native_handle(),
                     FIONREAD, &value))
    {
      error = boost::system::error_code(errno,
         boost::asio::error::get_system_category());
    }
#endif // defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)

    return error ? static_cast<std::size_t>(0)
                 : static_cast<size_t>(value);

  }

 inline std::size_t get_bytes_available(
    basic_serial_port<mockup_serial_port_service>& serial_port,
    boost::system::error_code& error)
  {
    error = boost::system::error_code();
    if (!serial_port.is_open()) {
      error = boost::asio::error::operation_aborted;
    }

    return 0;
  }

  /// @brief Returns the number of bytes available for reading from a serial
  ///        port without blocking.  Throws on error.
  template<class SerialPort>
  std::size_t get_bytes_available(SerialPort& serial_port)
  {
    boost::system::error_code error;
    std::size_t bytes_available = get_bytes_available(serial_port, error);
    if (error)
    {
      boost::throw_exception((boost::system::system_error(error)));
    }
    return bytes_available;
  }


}}

#endif
