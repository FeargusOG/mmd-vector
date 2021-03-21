#include <iostream>

#include "include/mmd-vector.h"

int main(int argc, char *argv[])
{
  mmd::MmdVector<double> mmd{20};

  if (*argv[1] == 'm')
  {
    std::cout << "Running Memory Mapping Test..." << std::endl;
    mmd.mmd_test();
    std::cout<<mmd.get_mapped_vector()->at(8)<<std::endl;
  }
  else if (*argv[1] == 's')
  {
    std::cout << "Running Standard Test..." << std::endl;
    mmd.std_test();
  }
}