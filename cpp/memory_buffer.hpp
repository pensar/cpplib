// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include "concept.hpp"
#include "constant.hpp"
#include "s.hpp"
#include "bytes_util.hpp"

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

namespace pensar_digital
{
    namespace cpplib
    {
        class MemoryBuffer
        {
            public:
                typedef size_t Offset;
            private:
                std::span<std::byte> mbuffer; //!< Buffer.
                Offset mwrite_offset;         //!< Write offset.
                Offset mread_offset;          //!< Read offset.
                std::unordered_map<Offset, size_t> mindex; //!< Returns the element size in bytes at the given offset.

        public:
            /// Default constructor.
            MemoryBuffer(size_t initial_size = 1024*1024) : mread_offset(0), mwrite_offset(0)
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

            /// \brief Returns the number of elements in the buffer.
            const size_t count() const noexcept { return mindex.size(); }

            /// \brief Returns the buffer write offset in bytes.
            const size_t woffset() const noexcept { return mwrite_offset; }

            /// \brief Returns the buffer read offset in bytes.
            const size_t roffset() const noexcept { return mread_offset; }

            /// \brief Available space in the buffer in bytes.  
            const size_t wavailable() const noexcept { return mbuffer.size() - mwrite_offset; }

            /// \brief Available data to read from the buffer.
            const size_t ravailable() const noexcept { return mwrite_offset - mread_offset; }

            void write(const BytePtr data, const size_t size) noexcept
            {
                // Check if there is enough space in the buffer.
                if (wavailable() < size)
                {
                    // Allocate more memory.
                    auto new_buffer = std::span<std::byte>(new std::byte[mbuffer.size() + size], mbuffer.size() + size);
                    // Copy the old buffer to the new one.
                    memcpy(new_buffer.data(), mbuffer.data(), mbuffer.size());
                    // Delete the old buffer.
                    delete[] mbuffer.data();
                    // Update the buffer variable.
                    mbuffer = new_buffer;
                }

                // Copy the data to the buffer.
                memcpy(mbuffer.data() + mwrite_offset, data, size);
                
                // Update the offset.
                mwrite_offset += size;

                // Update the index.
                mindex[mwrite_offset] = size;
            }

            void read(BytePtr dest, const Offset offset)
            {
                size_t size = mindex[offset];
                // Check if there is enough data in the buffer.
                if (ravailable() < size)
                {
                    throw std::runtime_error("MemoryBuffer::read: not enough data in the buffer.");
                }

                // Copy the data from the buffer.
                memcpy(dest, mbuffer.data() + offset, size);

                // Update the offset.
                if (mread_offset == offset)
                {
                    mread_offset += size;
                }
            }
        }; // MemoryBuffer

        /// <summary>
        /// Specialization of memory buffer to deal with binary data for a Persistable type.
        /// </summary>
        template <Persistable T>
        class ObjMemoryBuffer 
        {
            private:
                std::span<std::byte> mbuffer; //!< Member variable "buffer"
                size_t mwrite_offset; //!< Member variable "offset"
                size_t mread_offset; //!< Member variable "offset"
                std::unordered_map<Id, size_t> mindex; //!< Returns the buffer offset in bytes for a given id.
                inline static const size_t DATA_SIZE = sizeof (T::Datatype); //!< Size of the data.
                inline static const size_t TOTAL_SIZE = DATA_SIZE + sizeof (Id); //!< Total size includes id size.

        public:
                /// Default constructor.
            ObjMemoryBuffer(size_t initial_size = 1000 * TOTAL_SIZE) : mread_offset(0), mwrite_offset(0)
                {
                    // Allocate memory.
                    mbuffer = std::span<std::byte>(new std::byte[initial_size], initial_size);
                }

                /** Default destructor */
                virtual ~ObjMemoryBuffer()
                {
				    // Delete the buffer.
					delete[] mbuffer.data();
				}

                /// \brief Returns the buffer size in bytes.
                const size_t size() const noexcept { return mbuffer.size(); }

                /// \brief Returns the number of elements in the buffer.
                const size_t count () const noexcept { return mindex.size (); }

                /// \brief Returns the buffer write offset in bytes.
                const size_t woffset() const noexcept { return mwrite_offset; }

                /// \brief Returns the buffer read offset in bytes.
                const size_t roffset() const noexcept { return mread_offset; }

                /// \brief Available space in the buffer in bytes.  
				const size_t wavailable() const noexcept { return mbuffer.size() - mwrite_offset; }
                
                /// \brief Available data to read from the buffer.
                const size_t ravailable() const noexcept { return mwrite_offset - mread_offset; }

                void write(const void* data, const size_t size) noexcept
                {
                    // Check if there is enough space in the buffer.
                    if (wavailable() < size)
                    {
                        // Allocate more memory.
                        auto new_buffer = std::span<std::byte>(new std::byte[mbuffer.size() + size], mbuffer.size() + size);
                        // Copy the old buffer to the new one.
                        memcpy(new_buffer.data(), mbuffer.data(), mbuffer.size());
                        // Delete the old buffer.
                        delete[] mbuffer.data();
                        // Update the buffer variable.
                        mbuffer = new_buffer;
                    }
                
                    // Copy the data to the buffer.
                    memcpy (mbuffer.data() + mwrite_offset, data, size);

                    // Update the offset.
                    mwrite_offset += size;
                }

                /// \brief Adds data to the buffer. 
                void write (const T& obj) noexcept
				{
                    // Check if the id already exists.
                    auto offset_it = mindex.find (obj.id ());

                    // If obj is in the buffer.
                    if (offset_it != mindex.end())
                    {
						// Update the data.
						memcpy (mbuffer.data() + offset_it->second + sizeof (Id), (std::byte*)(obj.data()), DATA_SIZE);
					}
                    else
                    {
                        auto id = obj.id();
                        write(&id, sizeof(id));
                        write(obj.data(), DATA_SIZE);
                    }
				}

                template <typename... Args> requires FactoryConstructible<T, Args...>
                T::Factory::P write (Args&&... args) noexcept
                {
                    typename T::Factory::P p = T::get (std::forward<Args>(args)...);
                    write(*p);

                    return p;
                }

                void read (BytePtr dest, const size_t offset, size_t size)
				{
					// Check if there is enough data in the buffer.
					if (ravailable() < size)
					{
                        throw std::runtime_error ("MemoryBuffer::read: not enough data in the buffer.");
					}

					// Copy the data from the buffer.
					memcpy(dest, mbuffer.data() + offset, size);

					// Update the offset.
                    if (mread_offset == offset)
                    {
						mread_offset += size;
					}
				}

                void read (T* p)
                {
                    auto offset_it = mindex.find(p->id());
                    if (offset_it != mindex.end())
                    {
                        // Copy the data from the buffer.
                        read((BytePtr)(p->data()), offset_it->second + sizeof(Id), DATA_SIZE);
                    }
					else
					{
						// Reads next data from the buffer.
                        Id id;
                        read ((BytePtr)(&id), mread_offset, sizeof(Id));
                        p->set_id(id);
                        read((BytePtr)(p->data()), mread_offset, DATA_SIZE);
					}
                }


                /// \brief Gets data from the buffer into a factory constructed object. 
                template <typename... Args> requires FactoryConstructible<T, Args...>
                typename T::Factory::P read (Args... args)
                {
                    typename T::Factory::P p = T::get (args ...); // Create an object using its factory.
                    
                    read (p.get ());
                    
                    return p;
                }
        }; // MemoryBuffer
    } // namespace cpplib
} // namespace pensar_digital
#endif // MEMORY_BUFFER_HPP
