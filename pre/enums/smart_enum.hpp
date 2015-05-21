#ifndef SWISSARMYKNIFE_ENUMS_SMART_ENUM_HPP
#define SWISSARMYKNIFE_ENUMS_SMART_ENUM_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <ostream>

#include <boost/assert.hpp> 
#include <boost/algorithm/string.hpp>

/**
 * \brief This macro generates for you a Smart Enum, that is to say a namespace containing the enum with functions to transform it from and back to string.
 *
 * This is meant to be used this way :
 *      SMART_ENUM(State, enum State {
 *          FAULT,
 *          RUNNING,
 *          STOPPED
 *      });
 *
 * This results in providing the declared enum and a stream operator overload for this enum :
 *
 *      namespace State {
 *          enum State {
 *              FAULT,
 *              RUNNING,
 *              STOPPED
 *          };
 *      
 *          inline std::ostream& operator<<(std::ostream& os, const enumTypeArg& val);
 *          inline std::string to_string(const enumTypeArg& val);
 *          inline enumTypeArg from_string(const std::string &str);
 *                    
 *      }
 *  
 *  This has the effect that you can seamlessly transform the enum into string by streaming it to a stringstream
 *  or directly to std::cout as it follow :
 *      std::stringstream ss;
 *      ss << State::FAULT 
 *      std::string myEnumStr = ss.str();
 *
 *      or
 *
 *      std::cout << State::RUNNING << std::endl;
 *
 *      or
 *
 *      State::to_string(State::FAULT);
 *
 *  Additionally if you get some string version of your enum you can destringify it :
 *      
 *      State::State myEnumVal = State::from_string("FAULT");
 *
 * \param enumTypeArg Provide here the enum name (e.g. Hello)
 * \param enumDeclaration Provide here the enum declaration as you are used to (e.g. enum Hello { Wo, rld })
 */
#define SMART_ENUM(enumTypeArg, ...)                                                     \
namespace enumTypeArg {                                                                  \
    __VA_ARGS__;                                                                         \
    inline std::ostream& operator<<(std::ostream& os, const enumTypeArg& val) {          \
            os << swissarmyknife::enums::to_string(#__VA_ARGS__, val);                   \
            return os;                                                                   \
    }                                                                                    \
                                                                                         \
    inline std::string to_string(const enumTypeArg& val) {                               \
            return swissarmyknife::enums::to_string(#__VA_ARGS__, val);                  \
    }                                                                                    \
                                                                                         \
    inline enumTypeArg from_string(const std::string &str) {                             \
            return swissarmyknife::enums::from_string<enumTypeArg>(#__VA_ARGS__, str);   \
    }                                                                                    \
}                                                                                        \


namespace pre { namespace enums {

   
    /**
     * \brief This functions helps retrieving the string representation from a gvien enumVal in a given
     * stringified enum declaration.
     *
     * TODO: Add support for enum declaration with custom values
     *
     * \param completeEnumDeclaration The enumDeclaration as stringified by the preprocessor
     * \param enumVal The value of the enum to find in this enum declaration
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
              
        return std::string("Error in generating enum string ");
    }                                                  

    /**
     * \brief This functions helps retrieving the enum value from a string. 
     *
     * TODO: Add support for enum declaration with custom values
     *
     * \param completeEnumDeclaration The enumDeclaration as stringified by the preprocessor
     * \param enumStr The enum entry declaration string
     * \return The enum value corresponding
     * \throws std::runtime_error in case no enum value exists for this string
     */
    template <typename EnumType>
    static inline EnumType from_string(const std::string completeEnumDeclaration, const std::string &enumStr) throw (std::runtime_error) {
        size_t begin = completeEnumDeclaration.find_first_of('{');
        size_t end = completeEnumDeclaration.find_last_of('}');
        const std::string identifiers = completeEnumDeclaration.substr(begin + 1, end );

        size_t count = 0;
        size_t found = 0;
        do {
            found = identifiers.find_first_of(",}", found+1);

            std::string identifiersSubset = identifiers.substr(0, found);
            size_t beginId = identifiersSubset.find_last_of("{,");
            identifiersSubset = identifiersSubset.substr(beginId+1);
            boost::algorithm::trim(identifiersSubset);

            if (identifiersSubset == enumStr) {
                return static_cast<EnumType>(count);
            }

            ++count;
        } while (found != std::string::npos);
              
        throw std::runtime_error("No valid enum value for the provided string");
    }                      

}}

#endif
