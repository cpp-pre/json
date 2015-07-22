//
// detail/mockup_serial_port_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef BOOST_ASIO_DETAIL_MOCKUP_SERIAL_PORT_SERVICE_HPP
#define BOOST_ASIO_DETAIL_MOCKUP_SERIAL_PORT_SERVICE_HPP

#include <boost/asio/detail/config.hpp>

#include <string>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/streambuf.hpp>

#include <boost/asio/buffer.hpp> 
#include <boost/asio/buffers_iterator.hpp> 

#include <boost/asio/detail/buffer_sequence_adapter.hpp> 

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace boost {
namespace asio {
namespace detail {

// Extend mockup_descriptor_service to provide serial port support.
class mockup_serial_port_service
{
public:
  // The dummy native type of a serial port.
  typedef int native_handle_type;

  // The dummy implementation type of the serial port.
  struct implementation_type {
    native_handle_type handle_ = -1;
    volatile bool open_ = false;
    volatile bool cancelled_ = false;

    std::shared_ptr<std::string> serial_line_simulation_buffer_in_
      = std::make_shared<std::string>();

    std::shared_ptr<boost::recursive_mutex> mutex_ 
      = std::make_shared<boost::recursive_mutex>();
    std::shared_ptr<boost::condition_variable> ready_read_
      = std::make_shared<boost::condition_variable>();
  };

  BOOST_ASIO_DECL mockup_serial_port_service(
      boost::asio::io_service& io_service);

  // Destroy all user-defined handler objects owned by the service.
  BOOST_ASIO_DECL void shutdown_service();

  // Construct a new serial port implementation.
  void construct(implementation_type& impl)
  {
    impl.serial_line_simulation_buffer_in_->clear();
  }

  // Move-construct a new serial port implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    impl.serial_line_simulation_buffer_in_ = other_impl.serial_line_simulation_buffer_in_;
  }

//  // Move-assign from another serial port implementation.
//  void move_assign(implementation_type& impl,
//      mockup_serial_port_service& other_service,
//      implementation_type& other_impl)
//  {
//    impl = other_service.fake_serial_;
//  }

  // Destroy a serial port implementation.
  void destroy(implementation_type& impl)
  {
    impl.serial_line_simulation_buffer_in_->clear();
    impl.serial_line_simulation_buffer_in_->clear();
  }

  // Open the serial port using the specified device name.
  BOOST_ASIO_DECL boost::system::error_code open(implementation_type& impl,
      const std::string& device, boost::system::error_code& ec);

  // Assign a native descriptor to a serial port implementation.
  boost::system::error_code assign(implementation_type& impl,
      const native_handle_type& native_descriptor,
      boost::system::error_code& ec)
  {
    throw std::runtime_error("Assigning from native handle is unsupported in mockup.");
  }

  // Determine whether the serial port is open.
  bool is_open(const implementation_type& impl) const
  {
    return impl.open_;
  }

  // Destroy a serial port implementation.
  boost::system::error_code close(implementation_type& impl,
      boost::system::error_code& ec)
  {
    impl.open_ = false;
    ec = boost::system::error_code();
    return ec;
  }

  // Get the native serial port representation.
  native_handle_type native_handle(implementation_type& impl)
  {
    return impl.handle_;
  }

  // Cancel all operations associated with the serial port.
  boost::system::error_code cancel(implementation_type& impl,
      boost::system::error_code& ec)
  {
    if (!is_open(impl)) 
    {
      ec = boost::asio::error::bad_descriptor;
      return ec;
    }

    impl.cancelled_ = true;
    impl.ready_read_->notify_all();
    ec = boost::system::error_code();
    return ec;
  }

  // Set an option on the serial port.
  template <typename SettableSerialPortOption>
  boost::system::error_code set_option(implementation_type& impl,
      const SettableSerialPortOption& option, boost::system::error_code& ec)
  {
    return do_set_option(impl,
        &mockup_serial_port_service::store_option<SettableSerialPortOption>,
        &option, ec);
  }

  // Get an option from the serial port.
  template <typename GettableSerialPortOption>
  boost::system::error_code get_option(const implementation_type& impl,
      GettableSerialPortOption& option, boost::system::error_code& ec) const
  {
    return do_get_option(impl,
        &mockup_serial_port_service::load_option<GettableSerialPortOption>,
        &option, ec);
  }

  // Send a break sequence to the serial port.
  boost::system::error_code send_break(implementation_type& impl,
      boost::system::error_code& ec)
  {
    ec = boost::asio::error::operation_not_supported;
    return ec;
  }

  // Write the given data. Returns the number of bytes sent.
  template <typename ConstBufferSequence>
  size_t write_some(implementation_type& impl,
      const ConstBufferSequence& buffers, boost::system::error_code& ec)
  {
    boost::recursive_mutex::scoped_lock lock{*impl.mutex_};

    auto begin_bytes = boost::asio::buffers_iterator<ConstBufferSequence>::begin(buffers); 
    auto end_bytes = boost::asio::buffers_iterator<ConstBufferSequence>::end(buffers); 

    std::copy(begin_bytes, end_bytes, std::back_inserter(*impl.serial_line_simulation_buffer_in_));
    impl.ready_read_->notify_all();
    ec = boost::system::error_code();
    return std::distance(begin_bytes, end_bytes);
  }

  // Start an asynchronous write. The data being written must be valid for the
  // lifetime of the asynchronous operation.
  template <typename ConstBufferSequence, typename Handler>
  void async_write_some(implementation_type& impl,
      const ConstBufferSequence& buffers, Handler& handler)
  {

    auto perform_write = [&impl, buffers, handler, this]() mutable {
      boost::system::error_code ec{};
      auto bytes_written = write_some(impl, buffers, ec);
      handler(ec, bytes_written);
    };

    io_service_.post(perform_write); 
  }

  // Read some data. Returns the number of bytes received.
  template <typename MutableBufferSequence>
  size_t read_some(implementation_type& impl,
      const MutableBufferSequence& buffers, boost::system::error_code& ec)
  {
    bool bytes_available = false;
    { boost::recursive_mutex::scoped_lock lock{*impl.mutex_};
      bytes_available = !impl.serial_line_simulation_buffer_in_->empty();
    }

    if (!bytes_available) {
      // Wait for bytes to be available
      boost::mutex mutex_wait{};
      boost::mutex::scoped_lock wait_lock{mutex_wait};
      impl.ready_read_->wait(wait_lock);
    }

    { boost::recursive_mutex::scoped_lock lock{*impl.mutex_};

      size_t read_count = 0;

      for (auto buf : buffers) {
        char* bytes = boost::asio::buffer_cast<char*>(buf);
        auto bytes_to_read =  std::min(boost::asio::buffer_size(buf), impl.serial_line_simulation_buffer_in_->size());
        memcpy(bytes, impl.serial_line_simulation_buffer_in_->data(), bytes_to_read);
        impl.serial_line_simulation_buffer_in_->erase(0, bytes_to_read);
        read_count += bytes_to_read;
      }

      ec = boost::system::error_code();
      return read_count;
    }
  }

  // Start an asynchronous read. The buffer for the data being received must be
  // valid for the lifetime of the asynchronous operation.
  template <typename MutableBufferSequence, typename Handler>
  void async_read_some(implementation_type& impl,
      const MutableBufferSequence& buffers, Handler& handler)
  {
    auto perform_read = [&impl, buffers, handler, this]() mutable {
      boost::system::error_code ec{};
      auto bytes_read = read_some(impl, buffers, ec);
      handler(ec, bytes_read);
    };

    io_service_.post(perform_read);
  }

private:
  // Function pointer type for storing a serial port option.
  typedef boost::system::error_code (*store_function_type)(
      const void*, termios&, boost::system::error_code&);

  // Helper function template to store a serial port option.
  template <typename SettableSerialPortOption>
  static boost::system::error_code store_option(const void* option,
      termios& storage, boost::system::error_code& ec)
  {
    return static_cast<const SettableSerialPortOption*>(option)->store(
        storage, ec);
  }

  // Helper function to set a serial port option.
  BOOST_ASIO_DECL boost::system::error_code do_set_option(
      implementation_type& impl, store_function_type store,
      const void* option, boost::system::error_code& ec);

  // Function pointer type for loading a serial port option.
  typedef boost::system::error_code (*load_function_type)(
      void*, const termios&, boost::system::error_code&);

  // Helper function template to load a serial port option.
  template <typename GettableSerialPortOption>
  static boost::system::error_code load_option(void* option,
      const termios& storage, boost::system::error_code& ec)
  {
    return static_cast<GettableSerialPortOption*>(option)->load(storage, ec);
  }

  // Helper function to get a serial port option.
  BOOST_ASIO_DECL boost::system::error_code do_get_option(
      const implementation_type& impl, load_function_type load,
      void* option, boost::system::error_code& ec) const;


  io_service& io_service_;
  termios termios_storage;
};

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/impl/mockup_serial_port_service.ipp>

#endif // BOOST_ASIO_DETAIL_MOCKUP_SERIAL_PORT_SERVICE_HPP
