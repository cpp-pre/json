/**
 * This is a temporary trick to avoid repeating the type of the member adapted.
 * As soon as https://github.com/boostorg/fusion/tree/fusion_adapters goes stable 
 * we can remove it.
 */
#ifndef BOOST_FUSION_AUTO_ADAPT_STRUCT_HPP
#define BOOST_FUSION_AUTO_ADAPT_STRUCT_HPP

#include <boost/typeof/typeof.hpp> 
#include <boost/fusion/include/adapt_struct.hpp> 
#include <boost/preprocessor/seq/for_each.hpp> 

#define ATTRIBUTE_DEFINITION(r, data, elem) \
    (BOOST_TYPEOF(data::elem), elem)

#define BOOST_FUSION_AUTO_ADAPT_STRUCT(NAME, ATTRIBUTES)                       \
    BOOST_FUSION_ADAPT_STRUCT(NAME,                                            \
        BOOST_PP_SEQ_FOR_EACH(ATTRIBUTE_DEFINITION, NAME, ATTRIBUTES))

#endif // BOOST_FUSION_AUTO_ADAPT_STRUCT_HPP
