#include <iostream>

#include <boost/fusion/include/define_struct.hpp>
#include <swissarmyknife/boost/fusion/adapted_struct_printer.hpp>

namespace bla {
  using boost::fusion::detail::operator <<;
}

BOOST_FUSION_DEFINE_STRUCT((bla), innerbim,
    (std::string, mystring)
    )

BOOST_FUSION_DEFINE_STRUCT((bla), bimbim,
    (int, boom)
    (int, bam)
    (bla::innerbim, my_inner_bim)
    )

BOOST_FUSION_DEFINE_STRUCT((bla), blabla,
    (bla::bimbim, bim)
    (int, i)
    (int, j)
    (std::vector<double>, list)
    (std::list<bla::bimbim>, list_of_bimbim)
    )

int main(int argc, char** argv) {
  using namespace swak;

  bla::blabla instance{
    {22, 12, bla::innerbim{"COOL"} }, 
    23,
    43, 
    {2.00, 39.07, 24.05},
    { 
      {24, 9, bla::innerbim{"FEEL GOOD"} },
      {26, 14, bla::innerbim{"SO BAD"} },
    }
  };
  std::cout << instance << std::endl;

  return 0;
}
