#include <iostream>

#include <boost/fusion/include/define_struct.hpp>
#include <swissarmyknife/boost/fusion/adapted_struct_printer.hpp>

BOOST_FUSION_DEFINE_STRUCT((bla), bimbim,
    (int, boom)
    (int, bam)
    )

BOOST_FUSION_DEFINE_STRUCT((bla), blabla,
    (bla::bimbim, bim)
    (int, i)
    (int, j)
    )

int main(int argc, char** argv) {
  using namespace swak;

  using boost::fusion::adapted_struct_printer;

  using boost::fusion::operator<<;

  bla::blabla instance{{22, 12}, 23,43};
  std::cout << instance << std::endl;

  return 0;
}
