#include <iostream>

#include <boost/fusion/include/define_struct.hpp>
#include <swissarmyknife/boost/fusion/adapted_struct_printer.hpp>


BOOST_FUSION_DEFINE_STRUCT((bla), blabla,
    (int, i)
    (int, j)
    );

int main(int argc, char** argv) {
  using namespace swak;

  using boost::fusion::adapted_struct_printer;


  bla::blabla instance{23,43};
  adapted_struct_printer printer(std::cout);
  printer(instance);

  return 0;
}
