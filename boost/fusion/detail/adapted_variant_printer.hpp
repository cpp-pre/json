#ifndef BOOST_FUSION_DETAIL_ADAPTED_VARIANT_PRINTER_HPP
#define BOOST_FUSION_DETAIL_ADAPTED_VARIANT_PRINTER_HPP

#include <boost/fusion/traits/is_boost_variant.hpp>

#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/for_each_member.hpp>
#include <boost/fusion/include/io.hpp>

#include <indenting_ostream.hpp>

#include <boost/fusion/detail/cstdint_to_hexa_stream_operators.hpp>

namespace boost { namespace fusion {

	struct adapted_struct_printer;

namespace detail {


	struct adapted_variant_printer : public boost::static_visitor<> {

		adapted_variant_printer(std::ostream& os) 
			: boost::static_visitor<>(), m_os(os) {}

		template<class T, 
				typename std::enable_if<
					! (boost::fusion::traits::is_sequence<T>::value || 
						is_boost_variant<T>::value)
					>::type* = nullptr>
		void operator()(T& value) const {
				using boost::fusion::detail::operator <<;
				m_os << value << ",\n";
		}

		template<class T, 
			typename std::enable_if<
				boost::fusion::traits::is_sequence<T>::value &&
				!is_boost_variant<T>::value 
				>::type* = nullptr>
		void operator()(T& value) const {
				m_os << indent;
				boost::fusion::for_each_member(value, adapted_struct_printer(m_os));
				m_os << deindent;
		}

		template<class T, 
			typename std::enable_if<
				(boost::fusion::traits::is_sequence<typename T:: types>::value && 
					is_boost_variant<T>::value)
			>::type* = nullptr>
		void operator()(T& value) const {
				m_os << indent;
				m_os << "<boost::variant>\n";
				boost::apply_visitor(*this, value);
				m_os << "</boost::variant>\n";
				m_os << deindent;
		}

		private:
			std::ostream& m_os;

	};

}

}}

#endif
