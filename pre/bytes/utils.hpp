#ifndef SWISSARMYKNIFE_BYTES_UTILS_HPP
#define SWISSARMYKNIFE_BYTES_UTILS_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <string>
#include <sstream>
#include <cctype>
#include <math.h>
#include <stdexcept>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

namespace pre { namespace bytes {

    /**
     * \brief Constant telling how much bits there is in a nibble
     */
    const size_t NIBBLE_BITS = 4;

    /**
     * \brief Constant telling how much Nibbles are in a byte
     */
    const size_t BYTE_NIBBLES = 2;

    /**
     * \brief Convers the given byte array of the given length to a corresponding hexadecimal number in a string.
     * \param bytes Array of bytes
     * \param length Length of the byte array
     */
    inline std::string to_hexstring(const boost::uint8_t *bytes, const size_t length) {
        uint8_t ch = 0x00;
        size_t i = 0; 

        if (bytes == NULL || length <= 0)
            return std::string("");

        /**
         * Constant array which can be used to convert hex strings to real nibbles.
         */
        const char nibbleToChar[] = {'0', '1', '2', '3', '4', '5',
                                     '6', '7', '8', '9', 'a', 'b',
                                     'c', 'd', 'e', 'f'};
	
        std::string out;
        out.reserve(length*2);
        while (i < length) {
	    
            ch = (bytes[i] & 0xF0); // Strip off high nibble
            ch = (ch >> NIBBLE_BITS); // shift the bits down
            ch = (ch & 0x0F); // must do this is high order bit is on!
	    
            out.push_back(nibbleToChar[ch]); // convert the nibble to a String Character

            ch = (bytes[i] & 0x0F); // Strip off low nibble 
	    
            out.push_back(nibbleToChar[ch]); // convert the nibble to a String Character
	    
            i++;
        }

        return out;
    }
    
    /**
     * \brief Convers the given byte array of the given length to a corresponding hexadecimal number in a human readable string.
     * \param bytes buffer of bytes
     */
    inline std::string to_hexstring(const std::string& bytes) {
        uint8_t ch = 0x00;
        size_t i = 0; 

        if (bytes.size() == 0)
            return std::string("");

        /**
         * Constant array which can be used to convert hex strings to real nibbles.
         */
        const char nibbleToChar[] = {'0', '1', '2', '3', '4', '5',
                                     '6', '7', '8', '9', 'a', 'b',
                                     'c', 'd', 'e', 'f'};
	
        std::string out;
        out.reserve(bytes.size()*2);
        while (i < bytes.size()) {
	    
            ch = (bytes[i] & 0xF0); // Strip off high nibble
            ch = (ch >> NIBBLE_BITS); // shift the bits down
            ch = (ch & 0x0F); // must do this is high order bit is on!
	    
            out.push_back(nibbleToChar[ch]); // convert the nibble to a String Character

            ch = (bytes[i] & 0x0F); // Strip off low nibble 
	    
            out.push_back(nibbleToChar[ch]); // convert the nibble to a String Character
	    
            i++;
        }

        return out;
    }
    

    /**
     * \brief Convers the given byte array of the given length to a corresponding binary number in a string.
     * \param byteArray Array of bytes
     */
    inline std::string to_binstring(const boost::container::vector<boost::uint8_t> &byteArray) {
        std::stringstream bitsRepresentation;
        const int shift = 8 * sizeof( uint8_t ) - 1;
        const uint8_t mask = 1 << shift;

        BOOST_FOREACH(uint8_t byte, byteArray) {

            for ( size_t i = 1; i <= shift + 1; i++ ) {
                bitsRepresentation << ( byte & mask ? '1' : '0' );
                byte <<= 1;
            }
        }

        return bitsRepresentation.str();
    }

    /**
     * \brief Converts the given hexChars to the byte corresponding
     * \param highNibbleChar
     * \param lowNibbleChar
     */
    inline boost::uint8_t from_hexchar(char highNibbleChar, char lowNibbleChar) {
        const boost::container::map<char, boost::uint8_t> charToNibble = boost::assign::map_list_of('0', 0)('1', 1)('2', 2)
                                                                                                    ('3', 3)('4', 4)('5', 5)
                                                                                                    ('6', 6)('7', 7)('8', 8)
                                                                                                    ('9', 9)('a', 10)('b', 11)
                                                                                                    ('c', 12)('d', 13)('e', 14)
                                                                                                    ('f', 15);
        if (!isdigit(highNibbleChar)) {
            highNibbleChar = tolower(highNibbleChar);
        }

        if (!isdigit(lowNibbleChar)) {
            lowNibbleChar = tolower(lowNibbleChar);
        }
	
        if ( (charToNibble.find(highNibbleChar) != charToNibble.end()) && ( charToNibble.find(lowNibbleChar) != charToNibble.end() ) ) {
            boost::uint8_t highNibble = charToNibble.find(highNibbleChar)->second;
            boost::uint8_t lowNibble = charToNibble.find(lowNibbleChar)->second;

            highNibble <<= NIBBLE_BITS; // make it high
            lowNibble |= highNibble; // merge them together
        
            return lowNibble;
        }

        return 0; // This is invalid
    }

    /**
     * \brief This function can be used to parse a string of hexadecimal number back into bits representation. 
     * Note that it runs from left to right, putting eventually the last nibble empty if the size of the input string isn't a multiple of 2.
     * \param hexString to convert
     */
    inline boost::container::vector<boost::uint8_t> from_hexstring(const std::string &hexString) {
        size_t nibblePaddedSize = ( (hexString.size() / BYTE_NIBBLES) + (hexString.size() % BYTE_NIBBLES) );
        
        if ((nibblePaddedSize & 1) != 0) {
            ++nibblePaddedSize;
        }

        boost::container::vector<boost::uint8_t> result;
        result.reserve(nibblePaddedSize);
        
        for (std::string::const_iterator iter = hexString.begin(); iter != hexString.end(); iter += BYTE_NIBBLES) {
	    
            if ( (iter+1) != hexString.end() ) {
                result.push_back(from_hexchar(*iter, *(iter+1)));
            } else {
                result.push_back(from_hexchar(*iter, '0'));
                break;
            }
        }
        
        return result;
    }

    /**
     * \brief Converts a string of hexadecimal number into a binary buffer of this valeu
     * \param hex byte string
     */
    inline std::string buffer_from_hexstring(const std::string& hex) {
        std::vector<char> bytes;
        for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
        }
        std::string bb(bytes.begin(),bytes.end());
        return bb;
    }

    /**
     * \brief This function transforms any byteArray in the chosen (u)int*_t type.
     * \param byteArray To transform into native uint
     * \return The int requested with the value of the byteArray.
     */
    template<typename intX_t>
    inline intX_t from_byteArray(const boost::container::vector<boost::uint8_t> &byteArray) {
        intX_t result = 0x0;

        for (size_t i = 0; i < sizeof(intX_t); ++i) {
            result |= static_cast<intX_t>(byteArray[i] << (CHAR_BIT * (sizeof(intX_t) - (i+1))));
        }

        return result;
    }
    
    /**
     * \brief This methods opens up a file and parses it's bytes into a bytearray.
     * \param binaryFilePath Where to find the file to load
     * \return The bytearray corresponding to the file content
     *
     * \throws std::runtime_error In case the file isn't found.
     */
    inline boost::shared_ptr<boost::container::vector<boost::uint8_t> > load_bytearray(const std::string binaryFilePath) {
        boost::shared_ptr<boost::container::vector<boost::uint8_t> > bytearray;
        std::ifstream in(binaryFilePath.data(), std::ios_base::in | std::ios::binary);
        
        if (in.good()) {
            in.seekg(0, in.end);
            std::streampos lengthToRead = in.tellg();
            in.seekg(0, in.beg);
            
            bytearray.reset(new boost::container::vector<boost::uint8_t>(lengthToRead, 0x0));
            in.read(reinterpret_cast<char*>(&bytearray->at(0)), lengthToRead);
            in.close();
        } else {
            throw std::runtime_error("File " + binaryFilePath + " not found.");
        }

        return bytearray;
    }
}}

#endif
