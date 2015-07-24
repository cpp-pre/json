#include <iostream>

#include <boost/format.hpp>

#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/basic_serial_port.hpp>
#include <boost/asio/mockup_serial_port_service.hpp>

#include <pre/chrono/chrono_suffixes.hpp>

int main(int argc, char** argv) {

  using namespace boost::asio;
  using namespace pre::chrono::boost;

  {
    boost::thread t1([](){
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC1"};
      std::string buffer = "Hello";
      boost::this_thread::sleep_for(500_ms);

      size_t tries = 5;
      std::cout << "Writing to SLC BUS" << std::endl;
      boost::function<void(const boost::system::error_code& ec, std::size_t bytes_transferred)> write_handler = [&tries, &port, &buffer, &write_handler](const boost::system::error_code& ec, std::size_t bytes_transferred) { 
        --tries;
        buffer = "Holla";
        boost::this_thread::sleep_for(100_ms);
        if (tries > 0) {
          std::cout << "Write done, scheduling next write" << std::endl;
          boost::asio::async_write(port, boost::asio::buffer(buffer.data(), buffer.size()), write_handler);
        } else {
          std::cout << "Write done, finishing" << std::endl;
        }
      };
      //port.write_some(boost::asio::buffer(buffer.data(), buffer.size()));
      boost::asio::async_write(port, boost::asio::buffer(buffer.data(), buffer.size()), write_handler);
          
      ios.run();
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
          }
        );

        --tries;
      }
      
      ios.run();
    });

    t2.join();
    t1.join();
  }

  {

    boost::thread producer([]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};
      
      boost::this_thread::sleep_for(1_sec);

      for (size_t trie = 0; trie < 10; ++trie) {
        std::cout << "Sending message " << trie << std::endl;
        std::string message = str(boost::format("This is a message %1%") % trie);
        boost::asio::write(port, buffer(message.data(), message.size()));
        std::string message_end = ", and this ends here.||";
        boost::asio::write(port, buffer(message_end.data(), message_end.size()));
        boost::this_thread::sleep_for(100_ms);
      }

    });

    boost::thread consumer([]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};

      streambuf receive_buf;

      size_t retries = 10;
      boost::function<void (const boost::system::error_code& ec, size_t bytes_read)> readHandler =
        [&receive_buf, &readHandler, &port, &retries](const boost::system::error_code& ec, size_t bytes_read) {
          --retries;
          std::cout << "message arrived of size " << bytes_read << std::endl;

          std::string received_message(bytes_read, '\0');
          std::istream is(&receive_buf);
          is.readsome(&received_message[0], received_message.size());
          std::cout << "bytes_read are : " << bytes_read << ", stream input size  :" << receive_buf.size() << std::endl;
          std::cout << "Message is \"" << received_message << "\"" << std::endl;

          if (retries > 0) {
            boost::asio::async_read_until(port, receive_buf, "||", readHandler);
          }
        };

      boost::asio::async_read_until(port, receive_buf, "||", readHandler);

      ios.run();
    });

    producer.join();
    consumer.join();
  }

  {
    boost::thread producer([]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};
      
      for (size_t trie = 0 ; trie < 10; ++trie) {
        std::cout << "Sending " << trie << std::endl;
        std::string message = str(boost::format("This is a message %1% without end...") % trie);
        boost::asio::write(port, buffer(message.data(), message.size()));
        boost::this_thread::sleep_for(10_ms);
      }

    }); 

    boost::thread safe_consumer([]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};

      streambuf receive_buf;

      boost::function<void (const boost::system::error_code& ec, size_t bytes_read)> readHandler =
        [&](const boost::system::error_code& ec, size_t bytes_read) {
          std::cout << "message arrived of size " << bytes_read << " and error : " << ec << std::endl;

          std::string received_message(bytes_read, '\0');
          std::istream is(&receive_buf);
          is.readsome(&received_message[0], received_message.size());
        };

      boost::asio::steady_timer timeout{ios};
      timeout.expires_from_now(std::chrono::milliseconds(1000));
      timeout.async_wait([&port](const boost::system::error_code& ec){
        if (ec != boost::asio::error::operation_aborted) {
          std::cout << "Cancelling all read !" << std::endl;
          port.cancel();
        }
      });

      boost::asio::async_read_until(port, receive_buf, "||", readHandler);

      ios.run();
    });

    producer.join();
    safe_consumer.join();
  }

  return 0;
}
