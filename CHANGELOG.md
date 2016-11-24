# CHANGELOG

## v1.4.2

  * FEATURE: pre::retry_for (Function which retries a callback during a given duration)
  * Support build on MSVC 2015 update 4

### pre::variant
  * Include mapbox.variant and support of it for pre/json/[to, from]\_json and for Boost.Spirit.Qi/Karma.
  * `pre::variant::apply_visitor`, `pre::variant::get` unified for Boost.Variant / Mapbox.Variant to make the variant type easily switchable.
  * FEATURE: pre::variant::polymorphic_get implemented for mapbox::variant as well.

### pre::asio
  * BUGFIX: Fix #5 thread datarace in mockup_serial_port.
  * BUGFIX: Mockup implementations of boost::asio::flush & boost::asio::get_bytes_available for code tested under mockup_serial_port.

### pre::bytes
  * [BREAKING] REFACTORING: This is a change in the api of to_hexstring(const string) into buffer_from_hexstring.

## v1.3.4

 * pre::type_traits::function_traits for lambda, functions, member functions
 * fix for stopping the mockup_serial_port for ASIO as well as windows builds.

