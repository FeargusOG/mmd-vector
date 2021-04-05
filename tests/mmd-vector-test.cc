
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
            EXPECT_EQ(vector.get_mapped_vector()->size(), 0);
            EXPECT_GT(vector.get_file_size(),0);
        }

        TEST(MmdVector, constructor_size)
        {
            MmdVector<double> vector{10};
            EXPECT_EQ(vector.get_mapped_vector()->size(), 0);
            EXPECT_GT(vector.get_file_size(), 10 * sizeof(double));
        }

        TEST(MmdVector, constructor_copy)
        {
            const double test_val = 2.0;
            MmdVector<double> orig_vector{10};
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 0);
            orig_vector.push_back(test_val);
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);

            MmdVector<double> copy_vector{orig_vector};
            EXPECT_EQ(copy_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(copy_vector.get_mapped_vector()->at(0), test_val);
        }

        TEST(MmdVector, swap)
        {
            const double test_val = 2.0;
            MmdVector<double> swap_vector;
            EXPECT_EQ(swap_vector.get_mapped_vector()->size(), 0);

            // Create a new vector, add to it, swap it, let it go out of scope.
            {
                // Create a new vector
                MmdVector<double> new_vector{10};
                EXPECT_EQ(new_vector.get_mapped_vector()->size(), 0);

                // Push a value to it
                new_vector.push_back(test_val);
                EXPECT_EQ(new_vector.get_mapped_vector()->size(), 1);
                EXPECT_DOUBLE_EQ(new_vector.get_mapped_vector()->at(0), test_val);

                // Swap ownership
                swap_vector.swap(new_vector);
                EXPECT_EQ(new_vector.get_mapped_vector()->size(), 0);
                EXPECT_EQ(swap_vector.get_mapped_vector()->size(), 1);
            }

            // The other vector has gone out of scope. Lets see we do still own it.
            EXPECT_EQ(swap_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(swap_vector.get_mapped_vector()->at(0), test_val);
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
