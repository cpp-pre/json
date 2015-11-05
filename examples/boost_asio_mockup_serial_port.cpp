#include <iostream>
#include <boost/thread.hpp>

#include <boost/asio/basic_serial_port.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <pre/boost/asio/mockup_serial_port_service.hpp>


int main() {

  using namespace boost::asio;

  boost::thread simulate_writing_device([](){
    io_service ios;
    basic_serial_port<mockup_serial_port_service> port{ios, "my_fake_serial_port"};

    const std::string message = "Hello";

    for (;;) {
     boost::asio::write(port, buffer(message.data(), message.size()));
     boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }

  });

  boost::thread simulate_reading_device([](){
    io_service ios;
    basic_serial_port<mockup_serial_port_service> port{ios, "my_fake_serial_port"};
    
    for (;;) {
      char message[5];
      boost::asio::read(port, buffer(message, 5));
      std::cout << "received : " << std::string(message, 5) << std::endl;
    }

  });

  simulate_writing_device.join();
  simulate_reading_device.join();

  return 0;
}
