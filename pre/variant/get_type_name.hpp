#ifndef PRE_VARIANT_GET_TYPE_NAME_HPP
#define PRE_VARIANT_GET_TYPE_NAME_HPP

#include <boost/variant.hpp>
#include <boost/type_index.hpp>

namespace pre { namespace variant {

  namespace detail {

    struct get_type_name_visitor : public boost::static_visitor<std::string> {
      template<class U>
      std::string operator()(const U&) const { 
        return boost::typeindex::type_id<U>().pretty_name();
      }
    };

  }

  /**
   * \brief Gets the string representing the active type.
   * \param variant the variant to get the active type string
   * \return The active type name as string demangled.
   */
  template<class InspectedVariant>
  inline std::string get_type_name(const InspectedVariant& variant) {
    return boost::apply_visitor(detail::get_type_name_visitor{}, variant);
  }

}}

#endif
