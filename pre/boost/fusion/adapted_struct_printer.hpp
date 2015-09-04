#ifndef BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP

#include <boost/variant.hpp>
#include <pre/boost/fusion/traits/is_boost_variant.hpp>
#include <boost/fusion/include/tag_of.hpp>

#include <boost/fusion/include/is_sequence.hpp>
#include <pre/boost/fusion/for_each_member.hpp>

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <pre/iostreams/indenting_ostream.hpp>
#include <pre/boost/fusion/detail/cstdint_to_hexa_stream_operators.hpp>
#include <pre/boost/fusion/detail/std_containers_ostream_operators.hpp>


namespace boost { namespace fusion {


  namespace detail {
    using pre::iostreams::indent;
    using pre::iostreams::deindent;

    template<class T>
    using enable_if_is_not_sequence_nor_variant_t = typename std::enable_if<
      ! ( (boost::fusion::traits::is_sequence<T>::value )

          || traits::is_boost_variant<T>::value)
    ,T>::type;

    template<class T>
    using enable_if_is_other_sequence_and_not_variant_t = typename std::enable_if<
      boost::fusion::traits::is_sequence<T>::value
      && !std::is_same< 
            typename boost::fusion::traits::tag_of<T>::type, 
            boost::fusion::struct_tag
          >::value
      && !traits::is_boost_variant<T>::value 
    ,T>::type;

    template<class T>
    using enable_if_is_adapted_struct_t = typename std::enable_if< 
      std::is_same<
        typename boost::fusion::traits::tag_of<T>::type, 
        boost::fusion::struct_tag
      >::value
    ,T>::type;


    template<class T>
    using enable_if_is_variant_t = typename std::enable_if<
      traits::is_boost_variant<T>::value
    ,T>::type;


    struct adapted_struct_printer : public boost::static_visitor<> {

      adapted_struct_printer(std::ostream& os) 
        : boost::static_visitor<>(),  m_os(os) {}

      template<class T, 
        enable_if_is_adapted_struct_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        m_os << name << " :";
        m_os << indent;
        this->operator()(value);
        m_os << deindent;
      }

      template<class T, 
        enable_if_is_adapted_struct_t<T>* = nullptr>
      void operator()(const T& value) const {
        m_os << "\n{" << indent << "\n";
        boost::fusion::for_each_member(value, *this);
        m_os << deindent << "}\n";
      }

      template<class T, 
        enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        m_os << name << " :";
        m_os << indent;
        this->operator()(value);
        m_os << deindent;
      }

      template<class T, 
        enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        m_os << "\n{" << indent << "\n";
        boost::fusion::operator<<(m_os, value);
        m_os << "\n" << deindent << "}\n";
      }

      template<class T, 
        enable_if_is_variant_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        m_os << name << " : \n";
        m_os << indent;
        this->operator()(value);
        m_os << deindent;
      }

      template<class T, 
        enable_if_is_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        m_os << "\n{" << indent << "\n";
        boost::apply_visitor(*this, value);
        m_os << deindent << "}\n";
      }

      template<class T, 
        enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        m_os << name << " : ";
        this->operator()(value);
        m_os << ",\n";
      }

      template<class T, 
        enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        using boost::fusion::operator<<;
        m_os << value;
      }
      
      private:
        std::ostream& m_os;
      
    };

  }

  namespace adapted_struct_printer {
    template<class T, 
      detail::enable_if_is_adapted_struct_t<T>* = nullptr>
    inline std::ostream& operator<<(std::ostream& os, const T& value) {
        pre::iostreams::indenting_stream indentos(os);
        detail::adapted_struct_printer printer(indentos);
        printer(value);
        return os;
    }

    using std_containers_ostream_operators::operator<<;
  }

}}

#endif
