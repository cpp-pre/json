#ifndef BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP

#include <boost/variant.hpp>
#include <swissarmyknife/boost/fusion/traits/is_boost_variant.hpp>
#include <swissarmyknife/boost/fusion/traits/is_container.hpp>

#include <boost/fusion/include/is_sequence.hpp>
#include <swissarmyknife/boost/fusion/for_each_member.hpp>

#include <boost/fusion/include/io.hpp>
#include <swissarmyknife/iostreams/indenting_ostream.hpp>
#include <swissarmyknife/boost/fusion/detail/cstdint_to_hexa_stream_operators.hpp>
#include <swissarmyknife/boost/fusion/detail/std_containers_ostream_operators.hpp>


namespace boost { namespace fusion {

  using swissarmyknife::iostreams::indent;
  using swissarmyknife::iostreams::deindent;

  template<class T>
  using enable_if_is_not_sequence_nor_variant_t = typename std::enable_if<
    ! (boost::fusion::traits::is_sequence<T>::value || is_boost_variant<T>::value)
  ,T>::type;

  template<class T>
  using enable_if_is_sequence_and_not_variant_t = typename std::enable_if<
    boost::fusion::traits::is_sequence<T>::value && !is_boost_variant<T>::value 
  ,T>::type;

  template<class T>
  using enable_if_is_variant_t = typename std::enable_if<
    is_boost_variant<T>::value
  ,T>::type;


	struct adapted_struct_printer : public boost::static_visitor<> {

		adapted_struct_printer(std::ostream& os) 
			: boost::static_visitor<>(),  m_os(os) {}

		template<class T, 
			enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
		void operator()(const char* name, T& value) const {
      m_os << name << " : ";
      this->operator()(value);
      m_os << ",\n";
		}

		template<class T, 
			enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
		void operator()(T& value) const {
      using namespace boost::fusion::detail;
      m_os << value;
		}







    template<class T, 
			enable_if_is_sequence_and_not_variant_t<T>* = nullptr>
		void operator()(const char* name, T& value) const {
      m_os << name << " :";
      m_os << indent;
      this->operator()(value);
      m_os << deindent;
		}

		template<class T, 
			enable_if_is_sequence_and_not_variant_t<T>* = nullptr>
		void operator()(T& value) const {
      m_os << "\n{" << indent << "\n";
      boost::fusion::for_each_member(value, *this);
      m_os << deindent << "}\n";
		}









    template<class T, 
			enable_if_is_variant_t<T>* = nullptr>
		void operator()(const char* name, T& value) const {
      m_os << name << " : \n";
      m_os << indent;
      this->operator()(value);
      m_os << deindent;
		}

		template<class T, 
			enable_if_is_variant_t<T>* = nullptr>
		void operator()(T& value) const {
      m_os << "<boost::variant>";
      boost::apply_visitor(*this, value);
      m_os << "</boost::variant>";
		}

		private:
			std::ostream& m_os;
		
	};

namespace detail {
  template<class T, 
    enable_if_is_sequence_and_not_variant_t<T>* = nullptr>
  inline std::ostream& operator<<(std::ostream& os, T& value) {
      swak::iostreams::indenting_stream indentos(os);
      adapted_struct_printer printer(indentos);
      printer(value);
      return os;
  }

  template<class T, 
    enable_if_is_variant_t<T>* = nullptr>
  inline std::ostream& operator<<(std::ostream& os, T& value) {
      swak::iostreams::indenting_stream indentos(os);
      indentos << indent;
      boost::apply_visitor(adapted_struct_printer(indentos), value);
      indentos << deindent;
      return os;
  }
}


}}

#endif
