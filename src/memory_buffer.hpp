// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include "constant.hpp"

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
        class MemoryBuffer;
		using MemoryBufferPtr = std::unique_ptr<MemoryBuffer>;

        class MemoryBuffer
        {
            public:
                typedef size_t Offset;
            protected:
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

			MemoryBuffer(BytePtr bp, size_t size) : MemoryBuffer()
			{
				// Copy the data to the buffer.
				write (bp, size);
			}

            // Memory buffer constructor that takes a trivially copyable type as input argument.
			template <class T> requires std::is_trivially_copyable_v<T>
            MemoryBuffer(const T* t) : MemoryBuffer()
            {
                write(t, sizeof(T));
            }

            /** Default destructor */
            virtual ~MemoryBuffer()
            {
                // Delete the buffer.
                delete[] mbuffer.data();
            }

			/// \brief Returns a BytePtr to the buffer.
			BytePtr data() noexcept { return mbuffer.data(); }

            /// \brief Returns a read only BytePtr to the buffer.
			const BytePtr data() const noexcept { return mbuffer.data(); }

            /// \brief Returns the buffer size in bytes.
            const size_t size() const noexcept { return mbuffer.size(); }

            /// \brief Returns the number of elements in the buffer.
            const size_t count() const noexcept { return mindex.size(); }

            /// \brief Returns the buffer write offset in bytes.
            const size_t woffset() const noexcept { return mwrite_offset; }

            /// \brief Returns the data size in bytes.
			const size_t data_size() const noexcept { return mwrite_offset; }

            /// \brief Returns the buffer read offset in bytes.
            const size_t roffset() const noexcept { return mread_offset; }

            /// \brief Available space in the buffer in bytes.  
            const size_t wavailable() const noexcept { return mbuffer.size() - mwrite_offset; }

            /// \brief Available data to read from the buffer.
            const size_t ravailable() const noexcept { return mwrite_offset - mread_offset; }

			/// \brief Write data to the buffer.
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
                
                // Update the index.
                mindex[mwrite_offset] = size;

                // Update the offset.
                mwrite_offset += size;

            }

            void write (std::ifstream& in, const size_t size)
            {
                in.read((char*)(mbuffer.data() + mwrite_offset), size);
				mindex[mwrite_offset] = size;
				mwrite_offset += size;
            }

            void read (BytePtr dest, const Offset offset, size_t size)
			{
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

            void read (BytePtr dest, const Offset offset)
            {
                size_t size = mindex[offset];
				if (size == 0)
				{
					throw std::runtime_error("MemoryBuffer::read: not enough data in the buffer.");
				}

				read(dest, offset, size);
            }

			void read(BytePtr dest)
			{
				read(dest, mread_offset);
			}

			void read_known_size(BytePtr dest, const size_t size)
			{
				read(dest, mread_offset, size);
			}   

            MemoryBuffer& copy (MemoryBuffer& mb, const size_t offset = 0)
			{
				// Check if there is enough space in the buffer.
				if (wavailable() < mb.size())
				{
					// Allocate more memory.
					auto new_buffer = std::span<std::byte>(new std::byte[mbuffer.size() + mb.size()], mbuffer.size() + mb.size());
					// Copy the old buffer to the new one.
					memcpy(new_buffer.data(), mbuffer.data(), mbuffer.size());
					// Delete the old buffer.
					delete[] mbuffer.data();
					// Update the buffer variable.
					mbuffer = new_buffer;
				}

                // Update the index.
                mindex[mwrite_offset] = mb.size();
                
                // Copy the data to the buffer.
				memcpy(mbuffer.data() + mwrite_offset, mb.mbuffer.data() + offset, mb.size());

				// Update the offset.
				mwrite_offset += mb.size();

				return *this;
			}

            MemoryBuffer& append (MemoryBuffer& mb)
            {
                return copy(mb, 0);
            }
        }; // MemoryBuffer


/*
        template <class T>
        class ObjMemoryBuffer
        {
            protected:
			
            std::span<std::byte> mbuffer; //!< Member variable "buffer"
			size_t mwrite_offset; //!< Member variable "offset"
			size_t mread_offset; //!< Member variable "offset"
			std::unordered_map<Id, size_t> mindex; //!< Returns the buffer offset in bytes for a given id.
        
            public:

            /// Default constructor.
            ObjMemoryBuffer(size_t initial_size = 1000 * sizeof(T)) : mread_offset(0), mwrite_offset(0)
            {
                // Allocate memory.
                mbuffer = std::span<std::byte>(new std::byte[initial_size], initial_size);
            }

            virtual ~ObjMemoryBuffer()
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
            }

            void read(BytePtr dest, const size_t offset, size_t size)
            {
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
        };
        */

    } // namespace cpplib
} // namespace pensar_digital
#endif // MEMORY_BUFFER_HPP
