
#include "mmd-vector.h"
#include <iostream>
#include "gtest/gtest.h"

namespace mmd
{
    namespace
    {

        TEST(MmdVector, HelloWorld)
        {
            MmdVector<double> vector{1};
            //vector.get_mapped_vector()->push_back(2.0);
            std::cout<<"capacity: "<<vector.get_mapped_vector()->capacity()<<std::endl;
            EXPECT_EQ(1, 1);
        }

    } // namespace
} // namespace mmd
