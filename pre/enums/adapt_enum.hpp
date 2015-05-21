#ifndef SWISSARMYKNIFE_ENUMS_ADAPT_ENUM_HPP
#define SWISSARMYKNIFE_ENUMS_ADAPT_ENUM_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <string>
#include <ostream>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>


#define SWISSARMYKNIFE_ADAPT_ENUM_EACH_ENUMERATION_ENTRY_C(                     \
    R, unused, ENUMERATION_ENTRY)                                               \
    case ENUMERATION_ENTRY:                                                     \
      return BOOST_PP_STRINGIZE(ENUMERATION_ENTRY);                             \
    break;                                                                      

/**
 * \brief Adapts ENUM to reflectable types.
 *
 * \param ENUM_TYPE To be adapted
 * \param ENUMERATION_SEQ Sequence of enum states
 */
#define SWISSARMYKNIFE_ADAPT_ENUM(ENUM_TYPE, ENUMERATION_SEQ)                   \
    inline std::string to_string(const ENUM_TYPE& enum_value) {                 \
      switch (enum_value) {                                                     \
      BOOST_PP_SEQ_FOR_EACH(                                                    \
          SWISSARMYKNIFE_ADAPT_ENUM_EACH_ENUMERATION_ENTRY_C,                   \
          unused, ENUMERATION_SEQ)                                              \
        default:                                                                \
          return BOOST_PP_STRINGIZE(ENUM_TYPE);                                 \
      }                                                                         \
    }                                                                           \
                                                                                \
    inline std::ostream& operator<<(std::ostream& os, const ENUM_TYPE& value) { \
      os << to_string(value);                                                   \
      return os;                                                                \
    }

#endif
