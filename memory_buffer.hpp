// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include "concept.hpp"
#include "generator.hpp"
#include "string_util.hpp"
#include "version.hpp"
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

class MemoryBufferKey
{
    private:
        std::type_index mtype_index;
        pd::Id mid;
    public:
        template <class T>
        MemoryBufferKey (const pensar_digital::cpplib::Id& id) noexcept
			: mtype_index (std::type_index (typeid(T))), mid (id) {}

        std::type_index type_index() const noexcept { return mtype_index; }
        pd::Id id() const noexcept { return mid; }
};
struct MemoryBufferValue
{
    size_t offset;
    size_t size;
};

template<>
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
                size_t moffset; //!< Member variable "offset"
                std::unordered_map<MemoryBufferKey, MemoryBufferValue> mindex; //!< Member variable "index"
            public:
                /// Default constructor.
                MemoryBuffer (const Id& id = NULL_ID, size_t initial_size = 10000) noexcept 
                {
                    // Allocate memory.
                    mbuffer = std::span<std::byte>(new std::byte[initial_size], initial_size);
                }

                /** Default destructor */
                virtual ~MemoryBuffer() = default;

                /// \brief Returns the buffer size in bytes.
                const size_t size() const noexcept { return mbuffer.size(); }

                /// \brief Returns the buffer offset in bytes.
                const size_t offset() const noexcept { return moffset; }

                /// \brief Available space in the buffer in bytes.  
				const size_t available() const noexcept { return mbuffer.size() - moffset; }

                /// \brief Adds data to the buffer. 
                template <Sizeable T>
                void add (const T& data) noexcept
				{
					// Check if there is enough space in the buffer.
					if (available() < sizeof(T))
					{
						// Allocate more memory.
						auto new_buffer = std::span<std::byte>(new std::byte[mbuffer.size() + sizeof(T)], mbuffer.size() + sizeof(T));
						// Copy the old buffer to the new one.
						memcpy (new_buffer.data(), mbuffer.data(), mbuffer.size());
						// Delete the old buffer.
						delete[] mbuffer.data();
						// Update the buffer.
						mbuffer = new_buffer;
					}
					// Copy the data to the buffer.
					memcpy (mbuffer.data() + moffset, &data, sizeof(T));

                    // Add index information.
                    mindex.insert (std::make_pair (MemoryBufferKey (data.get_id ()), MemoryBufferValue { moffset, sizeof(T) }));   

					// Update the offset.
					moffset += sizeof(T);
				}
        }; // MemoryBuffer

    } // namespace cpplib
} // namespace pensar_digital
#endif // MEMORY_BUFFER_HPP
