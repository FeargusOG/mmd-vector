#ifndef MMD_INCLUDE_VECTOR_H
#define MMD_INCLUDE_VECTOR_H

#include <cstddef>
#include <string>
#include <iostream>
#include <vector>

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

namespace mmd
{
    #define MB 1000000

    template <typename T>
    using MappedVector = boost::interprocess::vector<T, boost::interprocess::allocator<T, boost::interprocess::managed_mapped_file::segment_manager>>;
    /**
     * I think I should have a non-size specific impl and, like a vector, do the growing when I need to.
     * Start with a 1mb file, and double it each time I cannot alloc.
     */
    template <typename T>
    class MmdVector
    {
        //using MappedVector = boost::interprocess::vector<T, boost::interprocess::allocator<T, boost::interprocess::managed_mapped_file::segment_manager>>;

    public:
        /**
         * 
         */
        MmdVector(const std::size_t size);
        ~MmdVector();
        void mmd_test();
        void std_test();
        const MappedVector<T>* get_mapped_vector() const;

    private:
        /**
         * 
         */
        std::size_t file_size;
        std::string file_path;
        boost::interprocess::managed_mapped_file::handle_t vector_handle;
        boost::interprocess::managed_mapped_file mfile_memory;
        MappedVector<T> *mmd_vector;

        std::string generate_filepath();
    };

    template <typename T>
    MmdVector<T>::MmdVector(const std::size_t size)
    {
        this->file_size = size * MB;
        this->file_path = this->generate_filepath();
        boost::interprocess::file_mapping::remove(this->file_path.c_str());
        this->mfile_memory = boost::interprocess::managed_mapped_file(boost::interprocess::create_only, this->file_path.c_str(), this->file_size);
        this->mmd_vector = this->mfile_memory.construct<MappedVector<T>>("MappedVector<T>")(this->mfile_memory.get_segment_manager());
    }

    template <typename T>
    MmdVector<T>::~MmdVector()
    {
        boost::interprocess::file_mapping::remove(this->file_path.c_str());
    }

    template <typename T>
    std::string MmdVector<T>::generate_filepath()
    {
        std::string path(boost::interprocess::ipcdetail::get_temporary_path());
        path += "/";
        path += "tmp.out"; //TODO: Make this timestamp or something unique.
        return path;
    }

    template <typename T>
    void MmdVector<T>::mmd_test()
    {
        try
        {
            std::cout << "Size before: " << this->mmd_vector->size() << std::endl;
            for (int i = 0; i < 1000000; i++)
                this->mmd_vector->push_back(i);
            std::cout << "Size after: " << this->mmd_vector->size() << std::endl;
        }
        catch (const boost::interprocess::bad_alloc &)
        {
            //mapped file is full
        }
    }

    template <typename T>
    void MmdVector<T>::std_test()
    {
        std::vector<double> numbers(0);
        std::cout << "Size before: " << numbers.size() << std::endl;
        for (int i = 0; i < 1000000; i++)
            numbers.push_back(i);
        std::cout << "Size after: " << numbers.size() << std::endl;
    }

    template <typename T>
    const MappedVector<T>* MmdVector<T>::get_mapped_vector() const 
    {
        return this->mmd_vector;
    }
} // namespace mmd

#endif // MMD_INCLUDE_VECTOR_H