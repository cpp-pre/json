//
// mockup_serial_port_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_MOCKUP_SERIAL_PORT_SERVICE_HPP
#define BOOST_ASIO_MOCKUP_SERIAL_PORT_SERVICE_HPP

#include <boost/asio/detail/config.hpp>

#if defined(BOOST_ASIO_HAS_SERIAL_PORT) \
  || defined(GENERATING_DOCUMENTATION)

#include <cstddef>
#include <string>
#include <boost/asio/async_result.hpp>
#include <pre/boost/asio/detail/mockup_serial_port_service.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port_base.hpp>

namespace boost {
namespace asio {

/**
 * \brief mockup_serial_port_service : A virtual serial port allowing to write cross platform unit tests of serial communicating application.
 *
 * This class can be used in your unit tests to simulate a [boost::asio::serial_port](http://www.boost.org/doc/libs/release/doc/html/boost_asio/reference/serial_port.html).
 *
 * ### Example
 *
 * \code{.cpp}
 *
 *   using namespace boost::asio;

 *   boost::thread simulate_writing_device([](){
 *     io_service ios;
 *     basic_serial_port<mockup_serial_port_service> port{ios, "my_fake_serial_port"};

 *     const std::string message = "Hello";

 *     for (;;) {
 *      boost::asio::write(port, buffer(message.data(), message.size()));
 *      boost::this_thread::sleep_for(boost::chrono::seconds(1));
 *     }

 *   });

 *   boost::thread simulate_reading_device([](){
 *     io_service ios;
 *     basic_serial_port<mockup_serial_port_service> port{ios, "my_fake_serial_port"};
 *     
 *     for (;;) {
 *       char message[5];
 *       boost::asio::read(port, buffer(message, 5));
 *       std::cout << "received : " << std::string(message, 5) << std::endl;
 *     }

 *   });
 *
 * \endcode
 * 
 *   - [Full example](../examples/boost_asio_mockup_serial_port.cpp)
 *   - [Extended examples](../test/asio_mockup_serial_port_service_test.cpp)
 *
 */
class mockup_serial_port_service
  : public boost::asio::detail::service_base<mockup_serial_port_service>
{
public:

private:
  typedef detail::mockup_serial_port_service service_impl_type;

public:
  /// The type of a serial port implementation.
  typedef service_impl_type::implementation_type implementation_type;

  /// (Deprecated: Use native_handle_type.) The native handle type.
  typedef service_impl_type::native_handle_type native_type;

  /// The native handle type.
  typedef service_impl_type::native_handle_type native_handle_type;

  /// Construct a new serial port service for the specified io_service.
  explicit mockup_serial_port_service(boost::asio::io_service& io_service)
    : boost::asio::detail::service_base<mockup_serial_port_service>(io_service),
      service_impl_(io_service)
  {
  }

  /// Construct a new serial port implementation.
  void construct(implementation_type& impl)
  {
    service_impl_.construct(impl);
  }

#if defined(BOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move-construct a new serial port implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    service_impl_.move_construct(impl, other_impl);
  }

//  /// Move-assign from another serial port implementation.
//  void move_assign(implementation_type& impl,
//      mockup_serial_port_service& other_service,
//      implementation_type& other_impl)
//  {
//    service_impl_.move_assign(impl, other_service.service_impl_, other_impl);
//  }
#endif // defined(BOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destroy a serial port implementation.
  void destroy(implementation_type& impl)
  {
    service_impl_.destroy(impl);
  }

  /// Open a serial port.
  boost::system::error_code open(implementation_type& impl,
      const std::string& device, boost::system::error_code& ec)
  {
    return service_impl_.open(impl, device, ec);
  }

  /// Assign an existing native handle to a serial port.
  boost::system::error_code assign(implementation_type& impl,
      const native_handle_type& handle, boost::system::error_code& ec)
  {
    return service_impl_.assign(impl, handle, ec);
  }

  /// Determine whether the handle is open.
  bool is_open(const implementation_type& impl) const
  {
    return service_impl_.is_open(impl);
  }

  /// Close a serial port implementation.
  boost::system::error_code close(implementation_type& impl,
      boost::system::error_code& ec)
  {
    return service_impl_.close(impl, ec);
  }

  /// (Deprecated: Use native_handle().) Get the native handle implementation.
  native_type native(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Get the native handle implementation.
  native_handle_type native_handle(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Cancel all asynchronous operations associated with the handle.
  boost::system::error_code cancel(implementation_type& impl,
      boost::system::error_code& ec)
  {
    return service_impl_.cancel(impl, ec);
  }

  /// Set a serial port option.
  template <typename SettableSerialPortOption>
  boost::system::error_code set_option(implementation_type& impl,
      const SettableSerialPortOption& option, boost::system::error_code& ec)
  {
    return service_impl_.set_option(impl, option, ec);
  }

  /// Get a serial port option.
  template <typename GettableSerialPortOption>
  boost::system::error_code get_option(const implementation_type& impl,
      GettableSerialPortOption& option, boost::system::error_code& ec) const
  {
    return service_impl_.get_option(impl, option, ec);
  }

  /// Send a break sequence to the serial port.
  boost::system::error_code send_break(implementation_type& impl,
      boost::system::error_code& ec)
  {
    return service_impl_.send_break(impl, ec);
  }

  /// Write the given data to the stream.
  template <typename ConstBufferSequence>
  std::size_t write_some(implementation_type& impl,
      const ConstBufferSequence& buffers, boost::system::error_code& ec)
  {
    return service_impl_.write_some(impl, buffers, ec);
  }

  /// Start an asynchronous write.
  template <typename ConstBufferSequence, typename WriteHandler>
  BOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
      void (boost::system::error_code, std::size_t))
  async_write_some(implementation_type& impl,
      const ConstBufferSequence& buffers,
      BOOST_ASIO_MOVE_ARG(WriteHandler) handler)
  {
    detail::async_result_init<
      WriteHandler, void (boost::system::error_code, std::size_t)> init(
        BOOST_ASIO_MOVE_CAST(WriteHandler)(handler));

    service_impl_.async_write_some(impl, buffers, init.handler);

    return init.result.get();
  }

  /// Read some data from the stream.
  template <typename MutableBufferSequence>
  std::size_t read_some(implementation_type& impl,
      const MutableBufferSequence& buffers, boost::system::error_code& ec)
  {
    return service_impl_.read_some(impl, buffers, ec);
  }

  /// Start an asynchronous read.
  template <typename MutableBufferSequence, typename ReadHandler>
  BOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
      void (boost::system::error_code, std::size_t))
  async_read_some(implementation_type& impl,
      const MutableBufferSequence& buffers,
      BOOST_ASIO_MOVE_ARG(ReadHandler) handler)
  {
    detail::async_result_init<
      ReadHandler, void (boost::system::error_code, std::size_t)> init(
        BOOST_ASIO_MOVE_CAST(ReadHandler)(handler));

    service_impl_.async_read_some(impl, buffers, init.handler);

    return init.result.get();
  }

private:
  // Destroy all user-defined handler objects owned by the service.
  void shutdown_service()
  {
    service_impl_.shutdown_service();
  }

  // The platform-specific implementation.
  service_impl_type service_impl_;
};

} // namespace asio
} // namespace boost

//#include <boost/asio/detail/pop_options.hpp>

#endif // defined(BOOST_ASIO_HAS_SERIAL_PORT)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // BOOST_ASIO_SERIAL_PORT_SERVICE_HPP
