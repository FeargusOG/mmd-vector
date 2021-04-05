#ifndef MMD_INCLUDE_MMD_VECTOR_H
#define MMD_INCLUDE_MMD_VECTOR_H

#include <cstddef>
#include <string>
#include <iostream>
#include <vector>

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

namespace mmd
{
#define k1MB 1000000
#define kDefaultSizeMb 1

    template <typename T>
    using MappedVector = boost::interprocess::vector<T, boost::interprocess::allocator<T, boost::interprocess::managed_mapped_file::segment_manager>>;

    template <typename T>
    class MmdVector
    {
    public:
        MmdVector() : MmdVector(kDefaultSizeMb) {}  // ctor default
        MmdVector(const MmdVector& other);          // ctor copy
        MmdVector(MmdVector&& other) noexcept;      // ctor move
        MmdVector(const std::size_t size);          // ctor custom size
        ~MmdVector();
        MmdVector<T>& operator=(const MmdVector& other);        // optor copy
        MmdVector<T>& operator=(MmdVector&& other) noexcept;    // optor move
        const unsigned long get_file_size() const;
        const std::string get_file_path() const;
        const MappedVector<T> *get_mapped_vector() const;
        void push_back(const T &val);

    private:
        void swap(MmdVector& other);
        void init(const std::size_t size);
        const std::size_t min_file_size = k1MB * 1;
        unsigned long file_size;
        std::string file_path;
        boost::interprocess::managed_mapped_file::handle_t vector_handle;
        boost::interprocess::managed_mapped_file mfile_memory;
        MappedVector<T> *mmd_vector;

        std::string generate_filepath();
        void double_filesize();
    };

    template <typename T>
    MmdVector<T>::MmdVector(const std::size_t size)
    {
        this->init(size);
    }

    template <typename T>
    MmdVector<T>::MmdVector(const MmdVector& other)
    {
        this->init(other.file_size);
        for (int i=0; i < other.get_mapped_vector()->size(); i++)
        {
            this->mmd_vector->push_back(other.get_mapped_vector()->at(i));
        }
    }

    template <typename T>
    MmdVector<T>::MmdVector(MmdVector&& other) noexcept
    {
        this->swap(other);
    }

    template <typename T>
    void MmdVector<T>::init(const std::size_t size)
    {
        std::size_t bytes_needed = size * sizeof(T) * 1.5;
        bytes_needed = (bytes_needed < min_file_size) ? min_file_size : bytes_needed;
        this->file_path = this->generate_filepath();
        boost::interprocess::file_mapping::remove(this->file_path.c_str());
        this->mfile_memory = boost::interprocess::managed_mapped_file(boost::interprocess::create_only, this->file_path.c_str(), bytes_needed);
        this->file_size = this->mfile_memory.get_size();
        this->mmd_vector = this->mfile_memory.construct<MappedVector<T>>("MappedVector<T>")(mfile_memory.get_segment_manager());
        this->vector_handle = this->mfile_memory.get_handle_from_address(this->mmd_vector);
    }

    template <typename T>
    void MmdVector<T>::swap(MmdVector& other)
    {
        // Remove the original file that backed this vector
        boost::interprocess::file_mapping::remove(this->file_path.c_str());

        // Ensure all data is flushed from 'other'.
        other.mfile_memory.flush();

        // Swap ownership of file from 'other'.
        this->mfile_memory.swap(other.mfile_memory);
        this->file_path = other.file_path;
        this->vector_handle = other.vector_handle;
        this->mmd_vector = static_cast<MappedVector<T> *>(this->mfile_memory.get_address_from_handle(this->vector_handle));
        this->file_size = this->mfile_memory.get_size();

        // Reset 'other'
        other.init(kDefaultSizeMb);
    }

    template <typename T>
    MmdVector<T>& MmdVector<T>::operator=(const MmdVector& other)
    {
        for (int i=0; i < other.get_mapped_vector()->size(); i++)
        {
            this->mmd_vector->push_back(other.get_mapped_vector()->at(i));
        }
        return *this;
    }

    template <typename T>
    MmdVector<T>& MmdVector<T>::operator=(MmdVector&& other) noexcept
    {
        this->swap(other);
        return *this;
    }
    
    template <typename T>
    void MmdVector<T>::double_filesize()
    {
        boost::interprocess::managed_mapped_file::grow(this->file_path.c_str(), this->file_size);
        this->mfile_memory = boost::interprocess::managed_mapped_file(boost::interprocess::open_only, this->file_path.c_str());
        this->mmd_vector = static_cast<MappedVector<T> *>(this->mfile_memory.get_address_from_handle(this->vector_handle));
        this->file_size = this->mfile_memory.get_size();
    }

    template <typename T>
    void MmdVector<T>::push_back(const T &val)
    {
        bool try_again = true;
        while (true)
        {
            try
            {
                this->mmd_vector->push_back(val);
                break;
            }
            catch (const boost::interprocess::bad_alloc e)
            {
                if (try_again)
                {
                    try_again = false;
                    this->double_filesize(); 
                }
                else
                {
                    throw e;
                }
            }
        }
    }

    template <typename T>
    const unsigned long MmdVector<T>::get_file_size() const
    {
        return this->file_size;
    }

    template <typename T>
    const std::string MmdVector<T>::get_file_path() const
    {
        return this->file_path;
    }

    template <typename T>
    MmdVector<T>::~MmdVector()
    {
        boost::interprocess::file_mapping::remove(this->file_path.c_str());
    }

    template <typename T>
    std::string MmdVector<T>::generate_filepath()
    {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        std::string path(boost::interprocess::ipcdetail::get_temporary_path());
        path += "/";
        path += boost::uuids::to_string(uuid);
        return path;
    }

    template <typename T>
    const MappedVector<T> *MmdVector<T>::get_mapped_vector() const
    {
        return this->mmd_vector;
    }
} // namespace mmd

#endif // MMD_INCLUDE_MMD_VECTOR_H