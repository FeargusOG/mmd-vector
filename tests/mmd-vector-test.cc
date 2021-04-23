
#include "mmd-vector.h"
#include <iostream>
#include <vector>
#include "gtest/gtest.h"

namespace mmd
{
    namespace
    {
        const std::size_t k1Mb = 1000000;
        const std::size_t k2Mb = 2000000;
        const std::size_t k4Mb = 4000000;

        TEST(MmdVector, ctor_default)
        {
            const double test_val = 2.0;
            MmdVector<double> vector;
            EXPECT_EQ(vector.get_mapped_vector()->size(), 0);
            EXPECT_GT(vector.get_file_size(),0);
            vector.push_back(test_val);
            EXPECT_EQ(vector.get_mapped_vector()->at(0), test_val);
        }

        TEST(MmdVector, ctor_size)
        {
            const double test_val = 2.0;
            MmdVector<double> vector{10};
            EXPECT_EQ(vector.get_mapped_vector()->size(), 0);
            EXPECT_GT(vector.get_file_size(), 10 * sizeof(double));
            vector.push_back(test_val);
            EXPECT_EQ(vector.get_mapped_vector()->at(0), test_val);
        }

        TEST(MmdVector, ctor_vec)
        {
            // Create a vector
            const double test_val = 2.0;
            std::vector<double> orig_vector;
            EXPECT_EQ(orig_vector.size(), 0);

            // Push a value to it
            orig_vector.push_back(test_val);
            EXPECT_EQ(orig_vector.size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.at(0), test_val);

            // Copy it to a new vector
            MmdVector<double> copy_vector{orig_vector};
            EXPECT_EQ(copy_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(copy_vector.get_mapped_vector()->at(0), test_val);

            // Make sure that the original vector is left untouched
            EXPECT_EQ(orig_vector.size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.at(0), test_val);

        }

        TEST(MmdVector, ctor_copy)
        {
            // Create a vector
            const double test_val = 2.0;
            MmdVector<double> orig_vector{10};
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 0);

            // Push a value to it
            orig_vector.push_back(test_val);
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);

            // Copy it to a new vector
            MmdVector<double> copy_vector{orig_vector};
            EXPECT_EQ(copy_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(copy_vector.get_mapped_vector()->at(0), test_val);

            // Make sure that the original vector is left untouched
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);

        }

        TEST(MmdVector, ctor_move)
        {
            // Create an empty vector
            const double test_val = 2.0;
            MmdVector<double> orig_vector{10};
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 0);

            // Push a value to it
            orig_vector.push_back(test_val);
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);

            // Move the vector to a new one
            MmdVector<double> move_vector{std::move(orig_vector)};
            EXPECT_EQ(move_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(move_vector.get_mapped_vector()->at(0), test_val);

            // Ensure the original vector has been 'reset'
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 0);
            orig_vector.push_back(test_val);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);
        }

        TEST(MmdVector, optor_copy)
        {
            // Create a vector
            const double test_val = 2.0;
            MmdVector<double> orig_vector{10};
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 0);

            // Push a value to it
            orig_vector.push_back(test_val);
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);

            // Copy it to a new vector
            MmdVector<double> copy_vector;
            copy_vector = orig_vector;
            EXPECT_EQ(copy_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(copy_vector.get_mapped_vector()->at(0), test_val);

            // Make sure that the original vector is left untouched
            EXPECT_EQ(orig_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(orig_vector.get_mapped_vector()->at(0), test_val);

        }

        TEST(MmdVector, optor_move)
        {
            const double test_val = 2.0;
            MmdVector<double> move_vector;
            EXPECT_EQ(move_vector.get_mapped_vector()->size(), 0);

            {
                // Create a new vector
                MmdVector<double> new_vector{10};
                EXPECT_EQ(new_vector.get_mapped_vector()->size(), 0);

                // Push a value to it
                new_vector.push_back(test_val);
                EXPECT_EQ(new_vector.get_mapped_vector()->size(), 1);
                EXPECT_DOUBLE_EQ(new_vector.get_mapped_vector()->at(0), test_val);

                // Move ownership
                move_vector = std::move(new_vector);
                //move_vector.swap(new_vector);
                EXPECT_EQ(move_vector.get_mapped_vector()->size(), 1);

                // Ensure the original vector has been 'reset'
                EXPECT_EQ(new_vector.get_mapped_vector()->size(), 0);
                new_vector.push_back(test_val);
                EXPECT_DOUBLE_EQ(new_vector.get_mapped_vector()->at(0), test_val);
            }

            // The other vector has gone out of scope. Lets see we do still own it's memory.
            EXPECT_EQ(move_vector.get_mapped_vector()->size(), 1);
            EXPECT_DOUBLE_EQ(move_vector.get_mapped_vector()->at(0), test_val);
        }

        TEST(MmdVector, push_back_grow_double)
        {
            MmdVector<double> vector{10};
            unsigned long orig_size = vector.get_file_size();
            for (double i = 1.0; i <= 2000000; i++)
                vector.push_back(i);
            EXPECT_GT(vector.get_file_size(),orig_size);
        }

        TEST(MmdVector, push_back_grow_int)
        {
            MmdVector<int> vector{10};
            unsigned long orig_size = vector.get_file_size();
            for (double i = 1; i <= 2000000; i++)
                vector.push_back(i);
            EXPECT_GT(vector.get_file_size(),orig_size);
        }

        TEST(MmdVector, operator_square_brackets)
        {
            const int init_val = 2;
            const int new_val = 4;
            MmdVector<int> vector{10};
            vector.push_back(init_val);
            EXPECT_EQ(vector[0], init_val);
            vector[0] = new_val;
            EXPECT_EQ(vector[0], new_val);
        }

        

    } // namespace
} // namespace mmd
