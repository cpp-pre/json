//  Copyright (c) 2015 Damien Buhl (alias daminetreg)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef PRE_SPIRIT_KARMA_CRC_OF_HPP
#define PRE_SPIRIT_KARMA_CRC_OF_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/karma/meta_compiler.hpp>
#include <boost/spirit/home/karma/generator.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/detail/output_iterator.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/home/karma/detail/attributes.hpp>

#include <string>
#include <boost/crc.hpp>

///////////////////////////////////////////////////////////////////////////////
// definition the place holder
namespace pre { namespace spirit { namespace karma {
  BOOST_SPIRIT_TERMINAL_EX(crc_of);
}}}

namespace boost { namespace spirit {
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct use_directive<karma::domain
      , terminal_ex<pre::spirit::karma::tag::crc_of                  // enables crc_of(boost::ref(crc))[p]
        , fusion::vector1<T> >
    > : mpl::true_ {};

}}

namespace pre { namespace spirit { namespace karma {
  namespace mpl = boost::mpl;

    using boost::spirit::buffer_type;

    ///////////////////////////////////////////////////////////////////////////
    // crc_of_directive buffers all generated output of the embedded generator
    // and flushes it only if the whole embedded generator succeeds
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject>
    struct crc_of_directive : boost::spirit::karma::unary_generator<crc_of_directive<Subject> >
    {
        typedef Subject subject_type;
        typedef mpl::int_<
            subject_type::properties::value | 
            boost::spirit::karma::generator_properties::countingbuffer
        > properties;

        crc_of_directive(Subject const& subject, uint16_t& crc)
          : subject(subject), crc(crc) {}

        template <typename Context, typename Iterator>
        struct attribute
          : boost::spirit::traits::attribute_of<subject_type, Context, Iterator>
        {};

        template <typename OutputIterator, typename Context, typename Delimiter
          , typename Attribute>
        bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d
          , Attribute const& attr) const
        {
            // wrap the given output iterator to compute crc on output
            boost::spirit::karma::detail::enable_buffering<OutputIterator> buffering(sink);
            boost::spirit::karma::detail::enable_counting<OutputIterator> counting(sink);
            bool r = subject.generate(sink, ctx, d, attr);
            
            std::string buffered_data;
            std::back_insert_iterator<std::string> crc_sink(buffered_data);
            buffering.buffer_copy_to(crc_sink);

            boost::crc_ccitt_type crc_ccitt;
            crc_ccitt.reset();
            crc_ccitt.process_bytes(buffered_data.data(), buffered_data.size());
            crc = crc_ccitt.checksum();	

            buffering.buffer_copy();
            return r;
        }

        template <typename Context>
        boost::spirit::info what(Context& context) const
        {
            return info("crc_of", subject.what(context));
        }

        Subject subject;
        uint16_t& crc;
    };
}}}


namespace boost { namespace spirit { namespace karma {

    ///////////////////////////////////////////////////////////////////////////
    // Generator generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Subject, typename Modifiers>
    struct make_directive<
      terminal_ex<pre::spirit::karma::tag::crc_of, fusion::vector1<T> >, 
      Subject, Modifiers>
    {
        typedef pre::spirit::karma::crc_of_directive<Subject> result_type;

        template <typename Terminal>
        result_type operator()(Terminal & term,  Subject const& subject
          , unused_type) const
        {
            return result_type(subject, fusion::at_c<0>(term.args));
        }
    };

}}}

namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject>
    struct has_semantic_action<pre::spirit::karma::crc_of_directive<Subject> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Attribute, typename Context
        , typename Iterator>
    struct handles_container<pre::spirit::karma::crc_of_directive<Subject>, Attribute
        , Context, Iterator>
      : unary_handles_container<Subject, Attribute, Context, Iterator> {};
}}}

#endif
