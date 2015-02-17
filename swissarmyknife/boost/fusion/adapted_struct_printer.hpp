#ifndef BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP

#include <boost/variant.hpp>
#include <swissarmyknife/boost/fusion/traits/is_boost_variant.hpp>

#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/io.hpp>
#include <swissarmyknife/boost/fusion/for_each_member.hpp>

#include <swissarmyknife/iostreams/indenting_ostream.hpp>

//#include <swissarmyknife/boost/fusion/detail/adapted_variant_printer.hpp>
#include <swissarmyknife/boost/fusion/detail/cstdint_to_hexa_stream_operators.hpp>

namespace boost { namespace fusion {

	struct adapted_struct_printer : public boost::static_visitor<> {

		adapted_struct_printer(std::ostream& os) 
			: boost::static_visitor<>(),  m_os(os) {}


		template<class T, 
				typename std::enable_if<
					! (boost::fusion::traits::is_sequence<T>::value || 
						is_boost_variant<T>::value)
					>::type* = nullptr>
		void operator()(T& value) const {
      this->operator()("", value);
		}

		template<class T, 
			typename std::enable_if<
				boost::fusion::traits::is_sequence<T>::value &&
				!is_boost_variant<T>::value 
				>::type* = nullptr>
		void operator()(T& value) const {
      this->operator()("", value);
		}

		template<class T, 
			typename std::enable_if<
				(boost::fusion::traits::is_sequence<typename T:: types>::value && 
					is_boost_variant<T>::value)
			>::type* = nullptr>
		void operator()(T& value) const {
      this->operator()("", value);
		}

		template<class T, 
			typename std::enable_if<
				! (boost::fusion::traits::is_sequence<T>::value || 
					is_boost_variant<T>::value)
				>::type* = nullptr>
		void operator()(const char* name, T& value) const {
				using boost::fusion::detail::operator <<; //TODO: Override char printer elsewhere
				m_os << name << " : " << value << ",\n";
		}

		template<class T, 
			typename std::enable_if<
				boost::fusion::traits::is_sequence<T>::value &&
				!is_boost_variant<T>::value 
				>::type* = nullptr>
		void operator()(const char* name, T& value) const {
				m_os << name << " : ";
				m_os << indent;
				boost::fusion::for_each_member(value, *this);
				m_os << deindent;
		}

		template<class T, 
			typename std::enable_if<
				(boost::fusion::traits::is_sequence<typename T:: types>::value && 
					is_boost_variant<T>::value)
			>::type* = nullptr>
		void operator()(const char* name, T& value) const {
				m_os << name << " : \n";
				m_os << indent;
				m_os << "<boost::variant>\n";
				boost::apply_visitor(*this, value);
				m_os << "</boost::variant>\n";
				m_os << deindent;
		}


		private:
			std::ostream& m_os;
		
	};

	
  template<class T, 
    typename std::enable_if<
      boost::fusion::traits::is_sequence<T>::value &&
      !is_boost_variant<T>::value 
      >::type* = nullptr>
  inline std::ostream& operator<<(std::ostream& os, T& value) {
      os << indent;
      boost::fusion::for_each_member(value, adapted_struct_printer(os));
      os << deindent;
      return os;
  }

  template<class T, 
    typename std::enable_if<
      (boost::fusion::traits::is_sequence<typename T:: types>::value && 
        is_boost_variant<T>::value)
    >::type* = nullptr>
  inline std::ostream& operator<<(std::ostream& os, T& value) {
      os << indent;
      boost::apply_visitor(adapted_struct_printer(os), value);
      os << deindent;
      return os;
  }
  

}}

#endif
