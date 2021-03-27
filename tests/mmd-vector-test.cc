
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
            EXPECT_EQ(k1Mb, vector.get_file_size());
        }

        TEST(MmdVector, constructor_size)
        {
            MmdVector<double> vector{k2Mb};
            EXPECT_EQ(k2Mb, vector.get_file_size());
        }

        TEST(MmdVector, push_back)
        {
            MmdVector<double> vector{k1Mb};
            vector.push_back(1.0);
            EXPECT_EQ(k1Mb, vector.get_file_size());
        }

        // Create a vector of size 1Mb, then fill just beyond that space. 
        // Expect vector to now be 2Mb.
        TEST(MmdVector, push_back_grow)
        {
            MmdVector<double> vector{k1Mb};
            for (double i = 1.0; i <= k1Mb / sizeof(double); i++)
                vector.push_back(i);
            EXPECT_EQ(k2Mb, vector.get_file_size());
        }

        

    } // namespace
} // namespace mmd
