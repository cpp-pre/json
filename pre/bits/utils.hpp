#ifndef SWISSARMYKNIFE_BITS_UTILS_HPP
#define SWISSARMYKNIFE_BITS_UTILS_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <math.h>
#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/container/vector.hpp>

namespace pre { namespace bits {

    /**
     * \brief Create string representation of the dynamic bitset given
     * \param bitset
     */
    inline std::string to_string(const boost::dynamic_bitset<boost::uint8_t> & bitset) {
        std::string result;
	    boost::to_string(bitset, result);
        return result;
    }

    /**
     * \brief Returns a string representation in bits of the int given.
     * \param value
     * \return The string representing the int given
     */
    template <typename uintX_t>
    inline std::string to_binstring(uintX_t value) {
        std::stringstream bitsRepresentation;
        const int shift = 8 * sizeof( uintX_t ) - 1;
        uintX_t one = 1;
        const uintX_t mask = one << shift;

        for ( size_t i = 1; i <= shift + 1; i++ ) {
          bitsRepresentation << ( value & mask ? '1' : '0' );
          value <<= 1;
        }

       return bitsRepresentation.str();
    }

    template <typename uintX_t, size_t bitsetSize>
    inline uintX_t to_uint(const std::bitset<bitsetSize> value) {
        const size_t uintX_t_size = 8 * sizeof ( uintX_t ); 
        
        BOOST_ASSERT(uintX_t_size >= bitsetSize);

        const uintX_t one = 1;
        const uintX_t zero = 0;
        uintX_t result = 0x0;
        
        for (size_t i=0; i < value.size(); ++i) {
            result |= (value.test(i)) ? (one << i) : (zero << i);
        }

        return result;
    }

    /**
     * \brief This function pads the given client bitset in the host by making a copy of clientBitset, 
     * resizing the clientBitset or-ing it in the hostBitset and returning the hostBitset
     * \param hostBitset Which will get the clientBitset or-ed in it (i.e. The hostBitset get shifted to the left of the size of the clientBitset) hostBitset should be bigger than clientBitset
     * \param clientBitset The bitset which will be integrated in the host bitset
     * \return The hostBitset
     */
    inline boost::dynamic_bitset<boost::uint8_t> & shift_in_bitset(boost::dynamic_bitset<boost::uint8_t> &hostBitset, const boost::dynamic_bitset<boost::uint8_t> & clientBitset) {
        boost::dynamic_bitset<boost::uint8_t> clientBitsetCopy = clientBitset; 
        clientBitsetCopy.resize(hostBitset.size()); // Resize to have the hostBitset size
	  
        hostBitset <<= clientBitset.size();
        hostBitset |= clientBitsetCopy;
	
        return hostBitset;
    }

    /**
     * \brief This converts the given boost::dynamic_bitset to an array of 8-bit wide bytes.
     * \param bitset to convert
     * \return The vector of bytes resulting of the conversion in fixed byte width
     */
    inline boost::container::vector<boost::uint8_t> to_bytearray(const boost::dynamic_bitset<boost::uint8_t> &bitset) {
        size_t sizeBytesPadded = static_cast<size_t>(ceil(static_cast<double>(bitset.size()) / CHAR_BIT));
        boost::container::vector<boost::uint8_t> bytes(sizeBytesPadded);
        boost::container::vector<boost::uint8_t>::reverse_iterator iter = bytes.rbegin();
        boost::to_block_range(bitset, iter);
	
        return bytes;
    }

    /**
     * \brief Converts a byte array into a bitset
     * \param byteArray
     * \return The bitset corresponding to the given byte array
     */
    inline boost::dynamic_bitset<boost::uint8_t> to_bitset(const boost::container::vector<boost::uint8_t> &byteArray, size_t resultingBitSetSize = 0) {
        boost::dynamic_bitset<boost::uint8_t> bitset((resultingBitSetSize == 0) ? (byteArray.size() * CHAR_BIT) : resultingBitSetSize);
        boost::from_block_range(byteArray.rbegin(), byteArray.rend(), bitset);
        
        return bitset;
    }
}}

#endif
