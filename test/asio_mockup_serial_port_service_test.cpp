#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE asio_mockup_serial_port_service_test
#include <boost/test/unit_test.hpp>

#include <iostream>

#include <boost/format.hpp>

#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/basic_serial_port.hpp>
#include <pre/boost/asio/mockup_serial_port_service.hpp>

#include <pre/chrono/chrono_suffixes.hpp>

BOOST_AUTO_TEST_CASE (asio_mockup_serial_port_service_test_simplereadwrite) {

  using namespace boost::asio;
  using namespace pre::chrono::boost;

  {
    const size_t try_count = 5;
    boost::thread t1([try_count](){
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC1"};
      std::string buffer = "Hello";
      boost::this_thread::sleep_for(500_ms);

      size_t tries = try_count;
      size_t called = 0;
      std::cout << "Writing to SLC BUS" << std::endl;
      boost::function<void(const boost::system::error_code& ec, std::size_t bytes_transferred)> write_handler 
        = [&tries, &called, &port, &buffer, &write_handler]
          (const boost::system::error_code& ec, std::size_t bytes_transferred) { 
        BOOST_ASSERT(!ec);
        --tries; ++called;

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
      BOOST_ASSERT(called == try_count);
    });

    boost::thread t2([try_count](){
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC1"};
      size_t tries = try_count;
      size_t called = 0;

      char buffer[255];
      while (tries > 0) {
        std::cout << "Registering 5 bytes read from SLC BUS" << std::endl;
        boost::asio::async_read(port, boost::asio::buffer(buffer, 5),
          [try_count, tries, &buffer, &called](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            std::cout << "Read " << bytes_transferred << " bytes : " << buffer << std::endl;
            BOOST_ASSERT(bytes_transferred == 5);
            BOOST_ASSERT(!ec);

            if (tries == try_count) {
              BOOST_ASSERT(std::string(buffer, bytes_transferred) == "Hello");
            } else {
              BOOST_ASSERT(std::string(buffer, bytes_transferred) == "Holla");
            }
            ++called;
          }
        );

        --tries;
      }
      
      ios.run();
      BOOST_ASSERT(called == try_count);
    });

    t2.join();
    t1.join();
  }

  {

    const size_t try_count = 10;

    boost::thread producer([try_count]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};
      
      boost::this_thread::sleep_for(1_sec);

      for (size_t trie = 0; trie < try_count; ++trie) {
        std::cout << "Sending message " << trie << std::endl;

        std::string message = str(boost::format("This is a message %1%") % trie);
        auto bytes_written = boost::asio::write(port, buffer(message.data(), message.size()));
        BOOST_ASSERT(bytes_written == message.size());

        std::string message_end = ", and this ends here.||";
        bytes_written = boost::asio::write(port, buffer(message_end.data(), message_end.size()));
        BOOST_ASSERT(bytes_written == message_end.size());

        boost::this_thread::sleep_for(100_ms);
      }

    });

    boost::thread consumer([try_count]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};

      streambuf receive_buf;

      size_t retries = try_count;
      boost::function<void (const boost::system::error_code& ec, size_t bytes_read)> readHandler =
        [&receive_buf, &readHandler, &port, &retries](const boost::system::error_code& ec, size_t bytes_read) {
          BOOST_ASSERT(!ec);
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
      BOOST_ASSERT_MSG(retries == 0, "Not all message where given back");
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
        auto bytes = boost::asio::write(port, buffer(message.data(), message.size()));
        BOOST_ASSERT(bytes == message.size());
        boost::this_thread::sleep_for(10_ms);
      }

    }); 

    boost::thread safe_consumer([]() {
      io_service ios;
      basic_serial_port<mockup_serial_port_service> port{ios, "SLC0"};

      streambuf receive_buf;

      boost::function<void (const boost::system::error_code& ec, size_t bytes_read)> readHandler =
        [&](const boost::system::error_code& ec, size_t bytes_read) {

          BOOST_ASSERT(ec == boost::asio::error::operation_aborted);
          BOOST_ASSERT(bytes_read == 0);
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

}
