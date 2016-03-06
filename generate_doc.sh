#all find . | grep -E ".hpp$" | xargs cldoc generate --std=c++11 -I. -I/opt/libs/boost_1_59_0 -- --output doc/ 

# only the one we want to sell
cldoc generate --std=c++11 -I. -I/opt/libs/boost_1_59_0 -- --loglevel info --merge doc/ --output .  pre/json/to_json.hpp pre/json/from_json.hpp pre/enums/to_underlying.hpp pre/boost/asio/mockup_serial_port_service.hpp  pre/type_traits/function_traits.hpp
