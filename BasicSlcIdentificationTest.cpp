#include <iostream>

#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/asio/basic_serial_port.hpp>
#include <boost/asio/mockup_serial_port_service.hpp>

#include <pre/chrono/chrono_suffixes.hpp>

int main(int argc, char** argv) {

  using namespace pre::chrono::boost;
  using namespace boost::asio;
  io_service ios;
  basic_serial_port<mockup_serial_port_service> port{ios, "SLC1"};
  boost::thread t1([&port](){
    std::string buffer = "Hello";

    while (true) {
      std::cout << "Writing to SLC BUS" << std::endl;
      port.write_some(boost::asio::buffer(buffer.data(), buffer.size()));
      boost::this_thread::sleep_for(100_ms);
    }
  });

  boost::thread t2([&port](){

    while (true) {
      std::cout << "Reading from SLC BUS" << std::endl;
      char buffer[255];
      auto bytes_read = boost::asio::read(port, boost::asio::buffer(buffer, 5));
      std::cout << "Read " << bytes_read << " bytes : " << buffer << std::endl;
      boost::this_thread::sleep_for(100_ms);
    }

  });

  t1.join();
  t2.join();
  return 0;
}
