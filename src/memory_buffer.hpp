// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include "constant.hpp"
#include "concept.hpp"

#include "factory.hpp"

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
                using Offset = size_t;
                using Ptr    = std::unique_ptr<MemoryBuffer>;
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

            // Copy constructor.
			MemoryBuffer(const MemoryBuffer& mb) : MemoryBuffer()
			{
				// Copy the data to the buffer.
				write(mb.mbuffer.data(), mb.size());
			}

            // Memory buffer constructor that takes a trivially copyable type as input argument.
			template <class T> requires std::is_trivially_copyable_v<T>
            MemoryBuffer(const T* t) : MemoryBuffer()
            {
                write(t, sizeof(T));
            }

            // += operator for StdLayoutTriviallyCopyableData types.    
			template <HasStdLayoutTriviallyCopyableData T>
			MemoryBuffer& operator+=(const T& t)
			{
				write(t.data_bytes(), T::DATA_SIZE);
				return *this;
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
            const Offset woffset() const noexcept { return mwrite_offset; }

            /// \brief Returns the data size in bytes.
			const size_t data_size() const noexcept { return mwrite_offset; }

            /// \brief Returns the buffer read offset in bytes.
            const Offset roffset() const noexcept { return mread_offset; }

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

            MemoryBuffer& copy (const MemoryBuffer& mb, const Offset offset = 0)
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
				memcpy(mbuffer.data() + mwrite_offset, mb.mbuffer.data(), mb.size());

				// Update the offset.
				mwrite_offset += mb.size();

				return *this;
			}

            inline MemoryBuffer& append (const MemoryBuffer& mb)
            {
                return copy(mb, mwrite_offset);
            }

			inline MemoryBuffer& append(const BytePtr data, const size_t size)
			{
				write(data, size);
				return *this;
			}

			// += operator
            inline MemoryBuffer& operator+=(const MemoryBuffer& mb)
            {
                return append (mb);
            }

            template <HasStdLayoutTriviallyCopyableData T>
			MemoryBuffer& append(const T& t)
			{
				write(t.data_bytes (), T::DATA_SIZE);
				return *this;
			}
        }; // MemoryBuffer

        // MemoryBufferPtrConvertible concept requires a public method bytes() returning something convertible to MemoryBuffer::Ptr.
        template <typename T>
        concept MemoryBufferPtrConvertible = requires(T t) { { t.bytes() } -> std::convertible_to<MemoryBuffer::Ptr>; };

        // BinaryConstructible concept requires a constructor with this parameter: (MemoryBuffer& bytes) returning something convertible to T.
        template <typename T>
        concept BinaryConstructible = requires(MemoryBuffer & bytes) { { T(bytes) } -> std::convertible_to<T>; };

        // BinaryIO concept requires MemoryBufferPtrConvertible and BinaryConstructible.
        template <typename T>
        concept BinaryIO = MemoryBufferPtrConvertible<T> && BinaryConstructible<T>;

        // BinaryOutputtableObject concept requires MemoryBufferPtrConvertible and SizeableIdentifiable.
        template <class T>
        concept BinaryWriteableObject = MemoryBufferPtrConvertible<T> && Sizeofable<T>;

        template <typename T>
        concept BinaryStreamableObject = MemoryBufferPtrConvertible<T> && Streamable<T>;
        // ObjectBinaryWriteable concept requires a type T with a public method write<Obj> (const Obj& object). Where Obj must comply with BinaryWriteableObject.
        template <typename T, typename Obj>
        concept ObjectBinaryWriteable = requires(T t, const Obj & object)
        {
            { t.template write<Obj>(object) } -> std::convertible_to<void>;
        }&& BinaryWriteableObject<Obj>;

        // FactoryObjectBinaryWriteable concept requires a type T with a public method write<Obj> (const Obj& object). Where Obj must comply with BinaryOutputtableObject.
        template <typename T, typename Obj, typename... Args>
        concept FactoryObjectBinaryWriteable = requires(T t, const Obj & object)
        {
            { t.template write<Obj, Args...>(object) } -> std::convertible_to<void>;
        }&& ObjectBinaryWriteable<T, Obj>&& FactoryConstructible<Obj, Args ...>;

        // BinaryWriteable concept requires a public method write (std::span<std::byte>& wbytes).
        template <typename T>
        concept BinaryWriteable = requires(T t, std::span<std::byte>&wbytes) { { t.write(wbytes) } -> std::convertible_to<void>; };

        // BinaryPersistable concept requires a type T that is trivially copyable and has a public method virtual std::ostream& binary_write (std::ostream& os, const std::endian& byte_order = std::endian::native) const; and a public method virtual std::istream& binary_read (std::istream& is, const std::endian& byte_order = std::endian::native);
        template <typename T>
        concept BinaryPersistable = BinaryWriteable<T> && BinaryReadable<T>;

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
