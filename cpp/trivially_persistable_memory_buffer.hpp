// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef TRIVIALLY_PERSISTABLE_MEMORY_BUFFER_HPP
#define TRIVIALLY_PERSISTABLE_MEMORY_BUFFER_HPP

#include "constant.hpp"
#include "s.hpp"
#include "bytes_util.hpp"
#include "concept.hpp"
#include "memory_buffer.hpp"

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
        // / \brief Memory buffer to deal with TriviallyPersistible Types.
        template <TriviallyPersistable T>
        class TriviallyPersistableMemoryBuffer : public MemoryBuffer
        {
        public:

            /// \brief Adds data to the buffer. 
            void write_obj (const T& obj) noexcept
            {
                // Check if the id already exists.
                auto offset_it = this->mindex.find(obj.id());

                // If obj is in the buffer.
                if (offset_it != this->mindex.end())
                {
                    // Update the data.
                    memcpy(this->mbuffer.data() + offset_it->second + sizeof(Id), (std::byte*)(obj.data()), obj.data_size());
                }
                else
                {
                    auto id = obj.id();
                    this->write((BytePtr)&id, sizeof(id));
                    this->write(obj.data_bytes(), obj.data_size());
                }
            }

            template <typename... Args> requires FactoryConstructible<T, Args...>
            T::Factory::P write_obj (Args&&... args) noexcept
            {
                typename T::Factory::P p = T::get(std::forward<Args>(args)...);
                write_obj (*p);

                return p;
            }

            T::Factory::P write_obj () noexcept
            {
                typename T::Factory::P p = T::get();
                T obj = *p;
                write_obj (obj);

                return p;
            }

            void read_obj (typename T::Factory::P* p)
            {
                if (*p != nullptr)
                {
                    Id id = (*p)->id();
                    auto offset_it = this->mindex.find(id);
                    if (offset_it != this->mindex.end())
                    {
                        // Copy the data from the buffer.
                        MemoryBuffer::read((BytePtr)((*p)->data()), offset_it->second + sizeof(Id), (*p)->data_size());
                        return;
                    }
                }
                else
                {
                    (*p) = T::get();
                }
                // Reads next data from the buffer.
                Id id = NULL_ID;
                MemoryBuffer::read((BytePtr)(&id), this->mread_offset, sizeof(Id));
                (*p)->set_id(id);
                MemoryBuffer::read((BytePtr)((*p)->data()), this->mread_offset, (*p)->data_size());
            }


            /// \brief Gets data from the buffer into a factory constructed object. 
            template <typename... Args> requires FactoryConstructible<T, Args...>
            typename T::Factory::P read_obj (Args... args)
            {
                typename T::Factory::P p = T::get(args ...); // Create an object using its factory.

                read_obj (&p);

                return p;
            }

            /// \brief Gets data from the buffer into a factory constructed object. 
            typename T::Factory::P read_obj ()
            {
                typename T::Factory::P p = T::get(); // Create an object using its factory.

                read_obj (&p);

                return p;
            }
        }; // TriviallyPersistableMemoryBuffer 
    } // cpplib
} // pensar_digital

#endif // TRIVIALLY_PERSISTABLE_MEMORY_BUFFER_HPP