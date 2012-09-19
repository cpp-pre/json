#ifndef SWISSARMYKNIFE_ENUMS_UTILS_HPP
#define SWISSARMYKNIFE_ENUMS_UTILS_HPP

#include <ostream>
#include <algorithm>

#include <boost/assert.hpp> 
#include <boost/algorithm/string.hpp>

/**
 * \brief This macro generates for you a Smart Enum, that is to say a struct inheriting the SmartEnum class,
 * providing the ability to transform enum to string and from string.
 *
 * This is meant to be used this way :
 *      SMART_ENUM(State, enum State {
 *          FAULT,
 *          RUNNING,
 *          STOPPED
 *      });
 *
 * This results in a namespace being created named State, containing an the provide enum and a stream operator
 * overload for this enum :
 *
 *  namespace State {
 *      enum State { 
 *          FAULT,
 *          RUNNING,
 *          STOPPED
 *      };
 *      
 *      std::ostream& operator<<(std::ostream& os, const enumTypeArg& val) {
 *         os << swissarmyknife::enums::to_string(#__VA_ARGS__, val);
 *         return os;
 *      }
 *  }
 *  
 *  This has the effect that you can seamlessly transform the enum into string by streaming it to a stringstream
 *  or directly to std::cout as follow :
 *      std::stringstream ss;
 *      ss << State::FAULT 
 *      std::string myEnumStr = ss;
 *
 * \param enumTypeArg Provide here the enum name (e.g. Hello)
 * \param enumDeclaration Provide here the enum declaration as you are used to (e.g. enum Hello { Wo, rld })
 */
#define SMART_ENUM(enumTypeArg, ...) namespace enumTypeArg {                        \
    __VA_ARGS__;                                                                    \
    std::ostream& operator<<(std::ostream& os, const enumTypeArg& val) {            \
            os << swissarmyknife::enums::to_string(#__VA_ARGS__, val);              \
            return os;                                                              \
    }                                                                               \
}                                                                                   \


namespace swissarmyknife { namespace enums {

   
    /**
     * \brief This functions helps retrieving the string representation from a gvien enumVal in a given
     * stringified enum declaration.
     *
     * TODO: Add support for enum declaration with custom values
     *
     * \param completeEnumDeclaration The enumDeclaration as stringified by the preprocessor
     * \param enumVal The value of the enum to find in this enum declaration
     * \throws std::runtime_error If the provided enumVal cannot be found in the completeEnumDeclaration
     */
    static inline std::string to_string(const std::string completeEnumDeclaration, size_t enumVal) throw (std::runtime_error) {
        size_t begin = completeEnumDeclaration.find_first_of('{');
        size_t end = completeEnumDeclaration.find_last_of('}');
        const std::string identifiers = completeEnumDeclaration.substr(begin + 1, end );

        size_t count = 0;
        size_t found = 0;
        do {
            found = identifiers.find_first_of(",}", found+1);

            if (enumVal == count) {
                std::string identifiersSubset = identifiers.substr(0, found);
                size_t beginId = identifiersSubset.find_last_of("{,");
                identifiersSubset = identifiersSubset.substr(beginId+1);
                boost::algorithm::trim(identifiersSubset);
                return identifiersSubset;
            }

            ++count;
        } while (found != std::string::npos);
                                                                                                        
        throw std::runtime_error("Error in generating enum string ");
    }                                                  

 

}}

#endif
