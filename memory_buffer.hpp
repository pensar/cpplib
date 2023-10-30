// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include "concept.hpp"
#include "constant.hpp"
#include "string_util.hpp"

#include <memory> // for std::shared_ptr
#include <concepts>
#include <exception>
#include <string>
#include <typeinfo>      // for typeid
#include <string.h>
#include <utility>       // for std::move
#include <unordered_map> // for std::unordered_map
#include <functional>    // for std::hash
#include <typeindex>
#include <span>

namespace pd = pensar_digital::cpplib;

class MemoryBufferKey
{
    private:
        std::type_index mtype_index;
        pd::Id mid;
    public:
        MemoryBufferKey (const std::type_info& type_info, const pensar_digital::cpplib::Id& id) noexcept
			: mtype_index (std::type_index (type_info)), mid (id) {}

        std::type_index type_index() const noexcept { return mtype_index; }
        pd::Id id() const noexcept { return mid; }

        bool operator==(const MemoryBufferKey& other) const noexcept
		{
			return mtype_index == other.mtype_index && mid == other.mid;
		}

        bool operator!=(const MemoryBufferKey& other) const noexcept
		{
			return !(*this == other);
		}

        bool operator<(const MemoryBufferKey& other) const noexcept
		{
			return mtype_index < other.mtype_index || (mtype_index == other.mtype_index && mid < other.mid);
		}
};

struct MemoryBufferValue
{
    size_t offset;
    size_t size;

    MemoryBufferValue (size_t offset, size_t size) noexcept : offset (offset), size (size) {}
};

template <>
struct std::hash<MemoryBufferKey>
{
    std::size_t operator()(const MemoryBufferKey& k) const noexcept
    {
        std::size_t h1 = k.type_index ().hash_code ();
        std::size_t h2 = std::hash<pd::Id> {} (k.id ());
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

namespace pensar_digital
{
    namespace cpplib
    {
        /// <summary>
        /// In memory buffer to deal with binary data.
        /// </summary>
        class MemoryBuffer 
        {
            private:
                std::span<std::byte> mbuffer; //!< Member variable "buffer"
                size_t mwrite_offset; //!< Member variable "offset"
                size_t mread_offset; //!< Member variable "offset"
                std::unordered_map<MemoryBufferKey, MemoryBufferValue> mindex; //!< Member variable "index"

                template <Factorable F>
                void assign_dest(typename F::Factory::P* r, F** dest) noexcept
                {
                    *dest = &(**r);
                }

                template <class T>
                void assign_dest(T* r, T** dest) noexcept
                {
                    *dest = &(*r);
                }
        public:
                /// Default constructor.
                MemoryBuffer (size_t initial_size = 10000) : mread_offset(0), mwrite_offset(0)
                {
                    // Allocate memory.
                    mbuffer = std::span<std::byte>(new std::byte[initial_size], initial_size);
                }

                /** Default destructor */
                virtual ~MemoryBuffer()  
                {
				    // Delete the buffer.
					delete[] mbuffer.data();
				}

                /// \brief Returns the buffer size in bytes.
                const size_t size() const noexcept { return mbuffer.size(); }

                /// \brief Returns the buffer offset in bytes.
                const size_t woffset() const noexcept { return mwrite_offset; }

                /// \brief Returns the buffer read offset in bytes.
                const size_t roffset() const noexcept { return mread_offset; }

                /// \brief Available space in the buffer in bytes.  
				const size_t wavailable() const noexcept { return mbuffer.size() - mwrite_offset; }
                
                /// \brief Available data to read from the buffer.
                const size_t ravailable() const noexcept { return mwrite_offset - mread_offset; }

                void write(const std::span<std::byte>& data) noexcept
                {
                    // Check if there is enough space in the buffer.
                    if (wavailable() < data.size())
                    {
                        // Allocate more memory.
                        auto new_buffer = std::span<std::byte>(new std::byte[mbuffer.size() + data.size()], mbuffer.size() + data.size());
                        // Copy the old buffer to the new one.
                        memcpy(new_buffer.data(), mbuffer.data(), mbuffer.size());
                        // Delete the old buffer.
                        delete[] mbuffer.data();
                        // Update the buffer.
                        mbuffer = new_buffer;
                    }
                
                    // Copy the data to the buffer.
                    memcpy(mbuffer.data() + mwrite_offset, data.data(), data.size());

                    // Update the offset.
                    mwrite_offset += data.size();
                }

                /// \brief Adds data to the buffer. 
                template <BinaryWriteableObject T>
                void write (const T& obj) noexcept
				{
                    // Check if the id already exists.
                    auto it = mindex.find (MemoryBufferKey (typeid(T), obj.id ()));
                    if (it != mindex.end ())
					{
					    if (it->second.size == sizeof(T))
					    {
					        // Update the data.
					        memcpy (mbuffer.data() + it->second.offset, &(*((T&)obj).bytes ().begin ()), sizeof(T));
                            return;
					    }
					    else
					    {
					        // Remove the old data.
					        mindex.erase (it);
					    }
					}

                    // Add index information.
                    mindex.insert (std::make_pair (MemoryBufferKey (typeid(T), obj.id ()), MemoryBufferValue (mwrite_offset, sizeof(T))));

                    write (((T&)obj).bytes ());
				}
                template <BinaryWriteableObject T, typename... Args>
                requires FactoryConstructible<T, Args...>   
                T::Factory::P write (Args&&... args) noexcept
                {
                    typename T::Factory::P p = T::get (std::forward<Args>(args)...);
                    T* dest;
                    assign_dest<T>(&p, &dest);
                    write(*dest);

                    return p;
                }

                void read (std::span<std::byte> dest) 
				{
					// Check if there is enough data in the buffer.
					if (ravailable() < dest.size())
					{
                        throw std::runtime_error ("MemoryBuffer::read: not enough data in the buffer.");
					}

					// Copy the data from the buffer.
					memcpy(dest.data(), mbuffer.data() + mread_offset, dest.size());

					// Update the offset.
					mread_offset += dest.size();
				}

                /// \brief Gets data from the buffer into the passed object pointer. 
                template <Sizeable T>
                void read (T* p)
                {
                    //read (p->bytes ());
                    // Check if there is enough data in the buffer.
                    if (ravailable() < sizeof(T))
                    {
                        throw std::runtime_error("MemoryBuffer::read: not enough data in the buffer.");
                    }

                    auto it = mindex.find(MemoryBufferKey(typeid(T), p->id()));
                    if (it != mindex.end())
                    {
                        if (it->second.size == sizeof(T))
                        {
                            // Copy the data from the buffer.
                            memcpy(p, mbuffer.data() + it->second.offset, sizeof(T));
                            return;
                        }
                        else
                        {
                            throw std::runtime_error("MemoryBuffer::read: Invalid object size.");
                        }
                    }
					else
					{
						throw std::runtime_error("MemoryBuffer::read: Object not found.");
					}
                    // Copy the data from the buffer.
                    //memcpy(p, mbuffer.data() + mread_offset, sizeof(T));

                    // Update the offset.
                    //mread_offset += sizeof(T);
                    
                }


                /// \brief Gets data from the buffer into a factory constructed object. 
                template <class T, typename... Args>
                requires FactoryConstructible<T, Args...>
                typename T::Factory::P read (Args... args)
                {
                    typename T::Factory::P r = T::get (args ...); // Create an object using its factory.
                    
                    T* dest = 0;
                    assign_dest<T>(&r , &dest);
                    read (dest);
                    
                    return r;
                }
        }; // MemoryBuffer
    } // namespace cpplib
} // namespace pensar_digital
#endif // MEMORY_BUFFER_HPP
