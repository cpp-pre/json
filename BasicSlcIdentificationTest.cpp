#include <iostream>

#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/asio/basic_serial_port.hpp>
#include <boost/asio/mockup_serial_port_service.hpp>

#include <pre/chrono/chrono_suffixes.hpp>

int main(int argc, char** argv) {

  using namespace pre::chrono::boost;
  using namespace boost::asio;

  int tries = 5;
  boost::thread t1([](){
    io_service ios;
    basic_serial_port<mockup_serial_port_service> port{ios, "SLC1"};
    std::string buffer = "Hello";
    boost::this_thread::sleep_for(500_ms);

    size_t tries = 5;
    while (tries > 0) {
      std::cout << "Writing to SLC BUS" << std::endl;
      port.write_some(boost::asio::buffer(buffer.data(), buffer.size()));
      boost::this_thread::sleep_for(100_ms);
      --tries;
    }
  });

  boost::thread t2([](){
    io_service ios;
    basic_serial_port<mockup_serial_port_service> port{ios, "SLC1"};
    size_t tries = 5;

    char buffer[255];
    while (tries > 0) {
      std::cout << "Registering 5 bytes read from SLC BUS" << std::endl;
      boost::asio::async_read(port, boost::asio::buffer(buffer, 5),
        [&buffer](const boost::system::error_code& ec, std::size_t bytes_transferred) {
          std::cout << "Read " << bytes_transferred << " bytes : " << buffer << std::endl;
          boost::this_thread::sleep_for(100_ms);
        }
      );

      --tries;
    }
    
    ios.run();
  });

  t2.join();
  t1.join();

  return 0;
}
