
# Installing pre::json
json can be installed in three ways :

* [Copy paste in your project](#copy-paste-in-your-project)
* [Install](#install)
* [Install without dependencies](#install-without-dependencies)

## Copy paste in your project
1. Copy this folder in your project
2. Add the following to your include path :

### GCC / Clang / MSVC
```
-I json/ \
-I json/deps/boostorg/algorithm/include/ \
-I json/deps/boostorg/array/include/ \
-I json/deps/boostorg/assert/include/ \
-I json/deps/boostorg/bind/include/ \
-I json/deps/boostorg/concept_check/include/ \
-I json/deps/boostorg/config/include/ \
-I json/deps/boostorg/container_hash/include/ \
-I json/deps/boostorg/core/include/ \
-I json/deps/boostorg/detail/include/ \
-I json/deps/boostorg/exception/include/ \
-I json/deps/boostorg/function/include/ \
-I json/deps/boostorg/function_types/include/ \
-I json/deps/boostorg/functional/include/ \
-I json/deps/boostorg/fusion/include/ \
-I json/deps/boostorg/integer/include/ \
-I json/deps/boostorg/io/include/ \
-I json/deps/boostorg/iterator/include/ \
-I json/deps/boostorg/move/include/ \
-I json/deps/boostorg/mpl/include/ \
-I json/deps/boostorg/numeric_conversion/include/ \
-I json/deps/boostorg/optional/include/ \
-I json/deps/boostorg/predef/include/ \
-I json/deps/boostorg/preprocessor/include/ \
-I json/deps/boostorg/range/include/ \
-I json/deps/boostorg/regex/include/ \
-I json/deps/boostorg/smart_ptr/include/ \
-I json/deps/boostorg/static_assert/include/ \
-I json/deps/boostorg/system/include/ \
-I json/deps/boostorg/test/include/ \
-I json/deps/boostorg/throw_exception/include/ \
-I json/deps/boostorg/timer/include/ \
-I json/deps/boostorg/tuple/include/ \
-I json/deps/boostorg/type_index/include/ \
-I json/deps/boostorg/type_traits/include/ \
-I json/deps/boostorg/typeof/include/ \
-I json/deps/boostorg/utility/include/ \
-I json/deps/boostorg/winapi/include/ \
-I json/deps/nlohmann/json/include/ \
```

## Install
Install the library
```sh
mkdir build/ && cd build/
cmake ..
make install
```

You can tweak destination with `-DCMAKE_INSTALL_PREFIX=/install/path` to change the default `/usr/local`.

## If you use CMake :
```cmake
find_package(json REQUIRED)
add_executable(app your_code.cpp)
target_link_libraries(app pre::json)
```

## If you don't use CMake : 
Add to your include path : 
```
INSTALL_PREFIX/include/json/deps/
INSTALL_PREFIX/include/
```

## Install without dependencies
Install the library
```sh
mkdir build/ && cd build/
cmake .. -DINGLUED=OFF
make install
```

This library was built with [`#inglued <>`](https://github.com/header-only/inglued) therefore it forwards dependencies with it, but you might prefer the use of your own deps, therefore simply disable them via `INGLUED=OFF`.
