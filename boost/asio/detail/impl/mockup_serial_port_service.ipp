//
// detail/impl/mockup_serial_port_service.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef BOOST_ASIO_DETAIL_IMPL_MOCKUP_SERIAL_PORT_SERVICE_IPP
#define BOOST_ASIO_DETAIL_IMPL_MOCKUP_SERIAL_PORT_SERVICE_IPP

#include <boost/asio/detail/config.hpp>

#include <cstring>
#include <boost/asio/detail/mockup_serial_port_service.hpp>

namespace boost {
namespace asio {
namespace detail {

mockup_serial_port_service::mockup_serial_port_service(
    boost::asio::io_service& io_service)
  : io_service_(io_service)
{
}

void mockup_serial_port_service::shutdown_service()
{
}

boost::system::error_code mockup_serial_port_service::open(
    mockup_serial_port_service::implementation_type& impl,
    const std::string& device, boost::system::error_code& ec)
{
  if (is_open(impl))
  {
    ec = boost::asio::error::already_open;
    return ec;
  }

  static int handle_count = 0;
  ++handle_count;
  impl.handle_ = handle_count;
  impl.open_ = true;
  impl.cancelled_ = false;

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
