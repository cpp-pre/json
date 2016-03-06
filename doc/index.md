#<cldoc:index>
C++11 header-only pre::boost companion library baked with love.

# cpp pre library 
C++11 header-only pre::boost companion library baked with love.

## Features

  * [to\_json](index.html#pre::json/pre::json::to_json) & [from\_json](index.html#pre::json/pre::json::to_json) functions for **any custom or standard types & aggregate**.
  * [Lambda & Function instrospector traits](index.html#pre::type_traits::function_traits/pre::type_traits::function_traits)
  * A [mockup serial port](index.html#boost::asio::mockup_serial_port_service) working with boost::asio
  * safe cast from enums underlying\_type in the enum type.
  * std::chrono::duration & boost::chrono::duration suffixes for ms, sec... until C++14 is mainstream.
  * bytes & bits manipulation utilities

## About
This C++11 header-only library provides utilities that we miss in Boost for the moment when writing productive code.

We author it in our free time to help our personal project and companies to centralize useful reusable code, that we polish until we can propose it to the Boost Libraries.

We always deprecate our own implementation when the Boost community finally accepts them, because we are just a backward-compatible staging-library for small Boost utilities. 

That's why we named it pre, like pre::boost.

## Getting started
The library is header only, but has dependencies on Boost 1.59.0 and nlohmann-json.

### With hunter CMake Package manager
Simply drop in your CMakeLists.txt the following : 
```cmake
hunter_add_package(lib-cpp-pre)
find_package(lib-cpp-pre 1.3.2 REQUIRED)
include_directories(AFTER ${LIB_CPP_PRE_INCLUDE_DIRS})
```

### Without hunter
You can install these dependencies yourself, and then install the library this way : 
```shell
mkdir build/
cd build/
cmake .. && make install
```

## What we already brought to Boost

- [BOOST\_FUSION\_ADAPT\_STRUCT auto type deduction](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/fusion/adapted/adapt_struct.html) and we maitain it there.

## License
Licensed under the MIT License, see [LICENSE](LICENSE).

## Contributors

- Damien Buhl (alias daminetreg)
- Patrick Wieder (alias linkineo) for his reviews
