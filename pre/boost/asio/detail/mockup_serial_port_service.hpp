//
// detail/mockup_serial_port_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef BOOST_ASIO_DETAIL_MOCKUP_SERIAL_PORT_SERVICE_HPP
#define BOOST_ASIO_DETAIL_MOCKUP_SERIAL_PORT_SERVICE_HPP

#include <boost/asio/detail/config.hpp>

#include <string>
#include <deque>
#include <memory>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/make_unique.hpp>
#include <boost/function.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/streambuf.hpp>

#include <boost/asio/buffer.hpp> 
#include <boost/asio/buffers_iterator.hpp> 

#include <boost/asio/detail/buffer_sequence_adapter.hpp> 

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/recursive_mutex.hpp>

#if defined(GENERATING_DOCUMENTATION)
# define BOOST_ASIO_OPTION_STORAGE implementation_defined
#elif defined(BOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
# define BOOST_ASIO_OPTION_STORAGE DCB
#else
# define BOOST_ASIO_OPTION_STORAGE termios
#endif

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

    // Each read handler that get registered, get's a work object associated,
    // to avoid io_service.run() from returning until the handler can effectively 
    // be called
    typedef std::pair<boost::function<void (std::string)>, std::unique_ptr<io_service::work>> read_handler_entry;

    // Thread specific values
    native_handle_type handle_ = -1;
    bool open_ = false;
    bool cancelled_ = false;
    
    // io_service associated stack of handler to call in case a write occurs.
    std::shared_ptr<std::map<
      io_service*,
      std::shared_ptr<std::deque< read_handler_entry >>
    >> pending_read_handlers_
      = std::make_shared<std::map<
          io_service*,
          std::shared_ptr<std::deque< read_handler_entry >>
        >>();

    // Mutex to protect access the shared(_ptr) values
    std::shared_ptr<boost::recursive_mutex> mutex_ 
      = std::make_shared<boost::recursive_mutex>();

  };

  BOOST_ASIO_DECL mockup_serial_port_service(
      boost::asio::io_service& io_service);

  // Destroy all user-defined handler objects owned by the service.
  BOOST_ASIO_DECL void shutdown_service();

  // Construct a new serial port implementation.
  void construct(implementation_type& impl)
  {
  }

  // Move-construct a new serial port implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
  }

//  // Move-assign from another serial port implementation.
//  void move_assign(implementation_type& impl,
//      mockup_serial_port_service& other_service,
//      implementation_type& other_impl)
//  {
//    impl = other_service.fake_serial_;
//  }

  // Destroy a serial port implementation.
  void destroy(implementation_type& impl);
  
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
    cancel(impl, ec);
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


    { boost::recursive_mutex::scoped_lock lock{*impl.mutex_};

      impl.cancelled_ = true;

      auto pending_handlers = impl.pending_read_handlers_
        ->at(std::addressof(io_service_));

      while (!pending_handlers->empty()) {
        // operation_aborted will be passed as they 
        // will notice all was cancelled
        io_service_.post(std::bind(pending_handlers->back().first, ""));
        pending_handlers->pop_back();
      }

      io_service_.stop();
    }

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

    impl.cancelled_ = false;

    if (!is_open(impl)) {
      ec = boost::asio::error::no_such_device; 
      return 0;
    }

    auto begin_bytes = boost::asio::buffers_iterator<ConstBufferSequence>::begin(buffers);
    auto end_bytes = boost::asio::buffers_iterator<ConstBufferSequence>::end(buffers);

    std::string data{begin_bytes, end_bytes};
    size_t bytes_transferred = std::distance(begin_bytes, end_bytes);

    for (auto& pending_ioservice : *impl.pending_read_handlers_) {
      // Iterating over std::deque< read_handler_entry >
      if (pending_ioservice.first != std::addressof(io_service_)) {

        if (!pending_ioservice.second->empty()) {
          const auto& pending_read_handler = std::bind(pending_ioservice.second->back().first, data);
          pending_ioservice.first->post(pending_read_handler);
          pending_ioservice.second->pop_back();
        }
      }
    }

    ec = boost::system::error_code();
    return bytes_transferred;
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
    impl.cancelled_ = false;

    if (!is_open(impl)) {
      ec = boost::asio::error::no_such_device; 
      return 0;
    }
    
    // empy out buffer is a no-op.
    buffer_sequence_adapter<boost::asio::mutable_buffer,
        MutableBufferSequence> bufs_adapted(buffers);
    if (bufs_adapted.all_empty()) {
      ec = boost::system::error_code();
      return 0;
    }

    size_t read_count = 0;
    boost::function<void (std::string)> perform_read = [&impl, buffers, &read_count, &ec](std::string data) {
      if ( (impl.cancelled_) || (!impl.open_) ) {
        ec = boost::asio::error::operation_aborted;
        return; 
      }

      for (auto buf : buffers) {
        char* bytes = boost::asio::buffer_cast<char*>(buf);
        auto bytes_to_read =  std::min(boost::asio::buffer_size(buf), data.size());
        std::memcpy(bytes, data.data(), bytes_to_read);
        data.erase(0, bytes_to_read);

        read_count += bytes_to_read;
      }

      ec = boost::system::error_code();
    };

    // Register a read handler and run the ios when it's time to (write_some will post the handler) !
    io_service ios_for_read_notification;
    { boost::recursive_mutex::scoped_lock lock{*impl.mutex_};

      impl.pending_read_handlers_
        ->at(std::addressof(ios_for_read_notification))
        ->push_front(std::make_pair(perform_read, boost::make_unique<io_service::work>(ios_for_read_notification)));
    }

    ios_for_read_notification.run(); // We wait here until write_some dispatches the call to perform_read
    return read_count;
  }

  // Start an asynchronous read. The buffer for the data being received must be
  // valid for the lifetime of the asynchronous operation.
  template <typename MutableBufferSequence, typename Handler>
  void async_read_some(implementation_type& impl,
      const MutableBufferSequence& buffers, Handler& handler)
  {
    boost::function<void (std::string)> perform_read = [&impl, buffers, handler, this](std::string data) mutable {
      size_t read_count = 0;
      boost::system::error_code ec{}; 
      if (!is_open(impl)) {
        ec = boost::asio::error::no_such_device; 
        handler(ec,read_count);
        return;
      }
      
      // empy out buffer is a no-op.
      buffer_sequence_adapter<boost::asio::mutable_buffer,
          MutableBufferSequence> bufs_adapted(buffers);
      if (bufs_adapted.all_empty()) {
        ec = boost::system::error_code();
        handler(ec,read_count);
        return;
      }

      if ( (impl.cancelled_) || (!impl.open_) ) {
        ec = boost::asio::error::operation_aborted;
        handler(ec,read_count);
        return;
      }

      for (auto buf : buffers) {
        char* bytes = boost::asio::buffer_cast<char*>(buf);
        auto bytes_to_read =  std::min(boost::asio::buffer_size(buf), data.size());
        std::memcpy(bytes, data.data(), bytes_to_read);
        data.erase(0, bytes_to_read);

        read_count += bytes_to_read;
      }

      handler(ec, read_count);
    };


    { boost::recursive_mutex::scoped_lock lock{*impl.mutex_};

      impl.pending_read_handlers_
        ->at(std::addressof(io_service_))
        ->push_front(std::make_pair(perform_read, boost::make_unique<io_service::work>(io_service_)));
    }

  }

private:

  // Function pointer type for storing a serial port option.
  typedef boost::system::error_code (*store_function_type)(
      const void*, BOOST_ASIO_OPTION_STORAGE&, boost::system::error_code&);

  // Helper function template to store a serial port option.
  template <typename SettableSerialPortOption>
  static boost::system::error_code store_option(const void* option,
      BOOST_ASIO_OPTION_STORAGE& storage, boost::system::error_code& ec)
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
      void*, const BOOST_ASIO_OPTION_STORAGE&, boost::system::error_code&);

  // Helper function template to load a serial port option.
  template <typename GettableSerialPortOption>
  static boost::system::error_code load_option(void* option,
      const BOOST_ASIO_OPTION_STORAGE& storage, boost::system::error_code& ec)
  {
    return static_cast<GettableSerialPortOption*>(option)->load(storage, ec);
  }

  // Helper function to get a serial port option.
  BOOST_ASIO_DECL boost::system::error_code do_get_option(
      const implementation_type& impl, load_function_type load,
      void* option, boost::system::error_code& ec) const;


  io_service& io_service_;
  BOOST_ASIO_OPTION_STORAGE termios_storage;
};

} // namespace detail
} // namespace asio
} // namespace boost

#include <pre/boost/asio/detail/impl/mockup_serial_port_service.ipp>

#endif // BOOST_ASIO_DETAIL_MOCKUP_SERIAL_PORT_SERVICE_HPP
