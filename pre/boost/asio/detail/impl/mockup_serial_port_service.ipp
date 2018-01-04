//
// detail/impl/mockup_serial_port_service.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef BOOST_ASIO_DETAIL_IMPL_MOCKUP_SERIAL_PORT_SERVICE_IPP
#define BOOST_ASIO_DETAIL_IMPL_MOCKUP_SERIAL_PORT_SERVICE_IPP

#include <boost/asio/detail/config.hpp>

#include <functional>
#include <memory>
#include <cstring>
#include <string>
#include <map>

#include <boost/thread/recursive_mutex.hpp>

#include <pre/boost/asio/detail/mockup_serial_port_service.hpp>

namespace boost {
namespace asio {
namespace detail {

  typedef std::map<std::string, std::reference_wrapper<mockup_serial_port_service::implementation_type>>
    mockup_device_map;

  /**
   * \brief static storage for mockup device, so that we can share implementation_type, 
   *    and by the way receive buffer by the device name;
   */
  struct mockup_device_storage {
    size_t handle_count = 0 ;
    mockup_device_map device_store{};
    
    boost::recursive_mutex mutex_;
  };

  //size_t mockup_device_storage::handle_count = 0;
  //mockup_device_map mockup_device_storage::device_store{};
  //boost::recursive_mutex mockup_device_storage::mutex_;

  inline mockup_device_storage& mockup_storage() {
    static mockup_device_storage store{};
    return store;
  }

mockup_serial_port_service::mockup_serial_port_service(
    boost::asio::io_service& io_service)
  : io_service_(io_service)
{
}

void mockup_serial_port_service::shutdown_service()
{
}

void mockup_serial_port_service::destroy(implementation_type& impl) {
  boost::recursive_mutex::scoped_lock lock{mockup_storage().mutex_};
  
  for (auto iter = mockup_storage().device_store.begin();
      iter != mockup_storage().device_store.end();) {
    implementation_type& device = iter->second;
    if (std::addressof(device) == std::addressof(impl)) {
      iter = mockup_storage().device_store.erase(iter);
    } else {
      ++iter;
    }
  }
}

boost::system::error_code mockup_serial_port_service::open(
    mockup_serial_port_service::implementation_type& impl,
    const std::string& device, boost::system::error_code& ec)
{
  boost::recursive_mutex::scoped_lock lock{mockup_storage().mutex_};

  if (is_open(impl))
  {
    ec = boost::asio::error::already_open;
    return ec;
  }

  if (mockup_storage().device_store.find(device) == mockup_storage().device_store.end()) {
    mockup_storage().device_store.emplace(device, std::ref(impl));
    impl = mockup_storage().device_store.at(device);
  } else {
    impl = mockup_storage().device_store.at(device);
  }

  // Assign thread specific variables
  { 
    boost::recursive_mutex::scoped_lock lock{*impl.mutex_};
    mockup_storage().handle_count++;
    impl.handle_ = mockup_storage().handle_count;
    impl.open_ = true;
    impl.cancelled_ = false;
    impl.pending_read_handlers_->emplace(
        std::addressof(io_service_), 
        std::make_shared<std::deque<implementation_type::read_handler_entry>>());
  }

  ec = boost::system::error_code();
  return ec;
}


boost::system::error_code mockup_serial_port_service::do_set_option(
    mockup_serial_port_service::implementation_type& impl,
    mockup_serial_port_service::store_function_type store,
    const void* option, boost::system::error_code& ec)
{
  if (store(option, termios_storage, ec))
    return ec;

  ec = boost::system::error_code();
  return ec;
}

boost::system::error_code mockup_serial_port_service::do_get_option(
    const mockup_serial_port_service::implementation_type& impl,
    mockup_serial_port_service::load_function_type load,
    void* option, boost::system::error_code& ec) const
{
  return load(option, termios_storage, ec);
}

} // namespace detail
} // namespace asio
} // namespace boost

#endif // BOOST_ASIO_DETAIL_IMPL_MOCKUP_SERIAL_PORT_SERVICE_IPP
