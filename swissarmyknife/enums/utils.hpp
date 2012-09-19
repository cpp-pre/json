#ifndef SWISSARMYKNIFE_ENUMS_UTILS_HPP
#define SWISSARMYKNIFE_ENUMS_UTILS_HPP

#include <stdexcept>
#include <boost/algorithm/string.hpp>

#define STRINGIFY_ENUM(enumTypeArg, ...) __VA_ARGS__;                                               \
static inline std::string enumValueToString(enumTypeArg enumVal) {                                  \
                                                                                                    \
    static const std::string enumRepr(#__VA_ARGS__);                                                \
    size_t begin = enumRepr.find_first_of('{');                                                     \
    size_t end = enumRepr.find_last_of('}');                                                        \
    const std::string identifiers = enumRepr.substr(begin + 1, end );                               \
                                                                                                    \
    size_t count = 0;                                                                               \
    size_t found = 0;                                                                               \
    do {                                                                                            \
        found = identifiers.find_first_of(",}", found+1);                                           \
                                                                                                    \
        if (enumVal == count) {                                                                     \
            std::string identifiersSubset = identifiers.substr(0, found);                           \
            size_t beginId = identifiersSubset.find_last_of("{,");                                  \
            identifiersSubset = identifiersSubset.substr(beginId+1);                                \
            boost::algorithm::trim(identifiersSubset);                                              \
            return identifiersSubset;                                                               \
        }                                                                                           \
                                                                                                    \
        ++count;                                                                                    \
    } while (found != std::string::npos);                                                           \
                                                                                                    \
    throw std::runtime_error("Error in generating enum string ");                                   \
}                                                                                                   \



#endif
