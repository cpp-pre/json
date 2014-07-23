#ifndef BOOST_FUSION_DETAIL_CSTDINT_TO_HEXA_STREAM_OPERATORS_HPP
#define BOOST_FUSION_DETAIL_CSTDINT_TO_HEXA_STREAM_OPERATORS_HPP

#include <boost/io/ios_state.hpp>

namespace boost { namespace fusion {

namespace detail {

	inline std::ostream& operator<<(std::ostream& os, const uint8_t& value) {
		boost::io::ios_flags_saver ifs(os);
		os << std::hex  << "0x" << int(value);
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const int8_t& value) {
		boost::io::ios_flags_saver ifs(os);
		os << std::hex  << "0x" << int(value);
		return os;
	}

}

}}

#endif 
