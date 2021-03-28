
#include "mmd-vector.h"
#include <iostream>
#include "gtest/gtest.h"

namespace mmd
{
    namespace
    {
        const std::size_t k1Mb = 1000000;
        const std::size_t k2Mb = 2000000;
        const std::size_t k4Mb = 4000000;

        TEST(MmdVector, constructor_default)
        {
            MmdVector<double> vector;
            EXPECT_GT(vector.get_file_size(),0);
        }

        TEST(MmdVector, constructor_size)
        {
            MmdVector<double> vector{10};
            EXPECT_GT(vector.get_file_size(), 10 * sizeof(double));
        }

        TEST(MmdVector, push_back_grow)
        {
            MmdVector<double> vector{10};
            unsigned long orig_size = vector.get_file_size();
            for (double i = 1.0; i <= 2000000; i++)
                vector.push_back(i);
            EXPECT_GT(vector.get_file_size(),orig_size);
        }

        

    } // namespace
} // namespace mmd
