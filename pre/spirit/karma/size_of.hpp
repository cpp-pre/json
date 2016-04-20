//  Copyright (c) 2015 Damien Buhl (alias daminetreg)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef PRE_SPIRIT_KARMA_SIZE_OF_HPP
#define PRE_SPIRIT_KARMA_SIZE_OF_HPP

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

#include <boost/fusion/include/vector.hpp>

#include <string>

///////////////////////////////////////////////////////////////////////////////
// definition the place holder
namespace pre { namespace spirit { namespace karma {
  namespace mpl = boost::mpl;
  namespace fusion = boost::fusion;

  BOOST_SPIRIT_TERMINAL_EX(size_of);

}}}

namespace boost { namespace spirit {
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct use_directive<karma::domain
      , terminal_ex<pre::spirit::karma::tag::size_of                   // enables size_of(boost::ref(size))[p]
        , fusion::vector1<T> >
    > : mpl::true_ {};

}}

namespace pre { namespace spirit { namespace karma {

  namespace mpl = boost::mpl;

    using boost::spirit::buffer_type;

    ///////////////////////////////////////////////////////////////////////////
    // sizeof_directive discards all generated output of the embedded generator
    // and only give out the size that would have been generated.
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject>
    struct sizeof_directive : boost::spirit::karma::unary_generator<sizeof_directive<Subject> >
    {
        typedef Subject subject_type;
        typedef mpl::int_<
            subject_type::properties::value | 
            boost::spirit::karma::generator_properties::countingbuffer
        > properties;

        sizeof_directive(Subject const& subject, std::size_t& size)
          : subject(subject), size(size) {}

        template <typename Context, typename Iterator>
        struct attribute
          : boost::spirit::traits::attribute_of<subject_type, Context, Iterator>
        {};
        
        template <typename OutputIterator, typename Context, typename Delimiter
          , typename Attribute>
        bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d
          , Attribute const& attr_param) const
        {
            // wrap the given output iterator to compute size of output
            boost::spirit::karma::detail::enable_buffering<OutputIterator> buffering(sink);
            boost::spirit::karma::detail::enable_counting<OutputIterator> counting(sink);

            bool r = subject.generate(sink, ctx, d, attr_param);
            size = buffering.buffer_size();
            return r;
        }

        template <typename Context>
        boost::spirit::info what(Context& context) const
        {
            return info("size_of", subject.what(context));
        }

        Subject subject;
        std::size_t& size;
    };
}}}


namespace boost { namespace spirit { namespace karma {

    ///////////////////////////////////////////////////////////////////////////
    // Generator generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Subject, typename Modifiers>
    struct make_directive<
      terminal_ex<pre::spirit::karma::tag::size_of, fusion::vector1<T> >, 
      Subject, Modifiers>
    {
        typedef pre::spirit::karma::sizeof_directive<Subject> result_type;

        template <typename Terminal>
        result_type operator()(Terminal & term, Subject const& subject
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
    struct has_semantic_action<pre::spirit::karma::sizeof_directive<Subject> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Attribute, typename Context
        , typename Iterator>
    struct handles_container<pre::spirit::karma::sizeof_directive<Subject>, Attribute
        , Context, Iterator>
      : unary_handles_container<Subject, Attribute, Context, Iterator> {};
}}}

#endif
