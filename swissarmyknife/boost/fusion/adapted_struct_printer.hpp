#ifndef BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_PRINTER_HPP

#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/io.hpp>
#include <swissarmyknife/boost/fusion/for_each_member.hpp>

#include <swissarmyknife/iostreams/indenting_ostream.hpp>

#include <swissarmyknife/boost/fusion/detail/adapted_variant_printer.hpp>
#include <swissarmyknife/boost/fusion/detail/cstdint_to_hexa_stream_operators.hpp>

namespace boost { namespace fusion {

	struct adapted_struct_printer {

		adapted_struct_printer(std::ostream& os) 
			: m_os(os) {}

		template<class T, 
			typename std::enable_if<
				! (boost::fusion::traits::is_sequence<T>::value || 
					is_boost_variant<T>::value)
				>::type* = nullptr>
		void operator()(const char* name, T& value) const {
				using boost::fusion::detail::operator <<;
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
				boost::apply_visitor(detail::adapted_variant_printer(m_os), value);
				m_os << "</boost::variant>\n";
				m_os << deindent;
		}


		private:
			std::ostream& m_os;
		
	};

	

}}

#endif
