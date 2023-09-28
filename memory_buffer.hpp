// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include "concept.hpp"
#include "object.hpp"
#include "generator.hpp"
#include "string_util.hpp"
#include "header_lib/xmlParser.h"
#include "clone_util.hpp"
#include "version.hpp"
#include "json_util.hpp"
#include "factory.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <memory> // for std::shared_ptr
#include <concepts>
#include <exception>
#include <string>
#include <typeinfo> // for typeid
#include <string.h>
#include <utility> // for std::move

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        class MemoryBuffer;
        typedef std::shared_ptr<MemoryBuffer> MemoryBufferPtr;
        typedef Factory<MemoryBuffer, Id> MemoryBufferFactory;
        /// <summary>
        /// In memory buffer to deal with binary data.
        /// </summary>
        class MemoryBuffer : public Object
        {
        private:
            inline static MemoryBufferFactory mfactory = { 3, 10, NULL_ID }; //!< Member variable "factory"
            inline static Generator<MemoryBuffer, Id> mgenerator; //!< Member variable "generator"
            
            std::span<std::byte> mbuffer; //!< Member variable "buffer"

            Endianess mbyte_order = LITTLE_ENDIAN; //!< Member variable "byte_order"
            Alignment  malignment = BIT64;         //!< Member variable "alignment"
        protected:

            /// \brief Compare objects.
            ///
            /// \param The object to be compared with.
            /// \return true if they are equal, false otherwise.
            /// \see equals
            ///
            virtual bool _equals(const MemoryBuffer& o) const { return (get_id() == o.get_id()); }

        public:
            inline static const VersionPtr VERSION = pd::Version::get(1, 1, 1);

            // Constructors. 

            /// Default constructor.
            MemoryBuffer (const Id& id = NULL_ID, const Endianess byte_order = LITTLE_ENDIAN, const Alignment alignment = BIT64) noexcept 
                : Object (id == NULL_ID ? mgenerator.get() : id), mbyte_order (byte_order), malignment(alignment) {}

            /// Copy constructor
            /// \param other MemoryBuffer to copy from
            MemoryBuffer(const MemoryBuffer& o) { assign(o); }

            /// Move constructor
            MemoryBuffer(MemoryBuffer&& o) noexcept { assign(o); }

            /** Default destructor */
            virtual ~MemoryBuffer() {}

            MemoryBuffer& assign(const MemoryBuffer& o) noexcept { set_id(o.get_id()); malignment = o.malignment; return *this; }

            // Clone method. 
            MemoryBufferPtr clone() const noexcept { return pd::clone<MemoryBuffer>(*this, get_id()); }

            /// <summary>
            ///  Gets io mode.
            /// </summary>
            /// <returns>Alignment</returns>
            const Alignment& alignment() const noexcept { return malignment; }

            const Endianess& byte_order() const noexcept { return mbyte_order; }

            /// \brief Returns the buffer size in bytes.
            const size_t size() const noexcept { return mbuffer.size(); }

             /// <summary>
            ///  Sets alignment.
            ///  Changes the alignment of the buffer.  
            /// </summary>
            /// <returns></returns>
            void set_alignment(const Alignment& alignment) noexcept { malignment = alignment; }

            void set_byte_order(const Endianess& byte_order) noexcept { mbyte_order = byte_order; }

            /// \brief Access hash
            ///
            /// \return  The current value of hash
            virtual const Hash get_hash() const noexcept { return get_id(); };

            // Implements initialize method from Initializable concept.
            virtual bool initialize(const Id& id = NULL_ID, const Alignment alignment = BIT64) noexcept
            {
                set_id(id);
                malignment = alignment;
                return true;
            }

            // Conversion to json string.
            inline virtual String json() const noexcept
            {
                return pd::json<MemoryBuffer>(*this) + " }";
            }

            inline std::istream& read(std::istream& is, const std::endian& byte_order = std::endian::native) {}

            inline std::ostream& write(std::ostream& os, const std::endian& byte_order = std::endian::native) const {}

            // Conversion to xml string.
            virtual String xml() const noexcept { return ObjXMLPrefix() + "/>"; }

            // Conversion from xml string.
            virtual void from_xml(const String& sxml)
            {
                parse_object_tag(sxml);
            }

            MemoryBuffer& from_json(const String& sjson) {}

            /// Conversion to string.
            /// \return A string with the object id.
            virtual String to_string() const noexcept { return std::to_string(get_id ()); }

            /// Implicit conversion to string.
            /// \return A string with the object id.
            operator String () const noexcept { return to_string(); }

            /// Debug string.
            /// \return A string with the object id.
            virtual String debug_string() const noexcept
            {
                std::stringstream ss;
                ss << "id = " << to_string();
                return ss.str();

            }

            /// Assignment operator
            /// \param o MemoryBuffer to assign from
            /// \return A reference to this
            MemoryBuffer& operator=(const MemoryBuffer& o) noexcept { return assign(o); }

            /// Move assignment operator
            MemoryBuffer& operator=(MemoryBuffer&& o) noexcept { return assign(o); }

            friend void from_json(const Json& j, MemoryBuffer& o) {}

            static inline MemoryBufferFactory::P  get(const Id& aid = NULL_ID)
            {
                return mfactory.get(aid);
            };

            MemoryBufferFactory::P clone()
            {
                return get(get_id ());
            };

            inline static MemoryBufferFactory::P get(const Json& j)
            {
                String json_class = j.at("class");
                if (json_class != pd::class_name<MemoryBuffer>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<MemoryBuffer>());
                MemoryBufferFactory::P ptr = get(j.at("id"));

                VersionPtr v = Version::get(j["VERSION"]);

                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("MemoryBufferFactory::parse_json: version mismatch.");

                return ptr;
            }

            inline static MemoryBufferFactory::P get(const String& sjson)
            {
                Json j;
                MemoryBufferFactory::P ptr = get(pd::get_id<MemoryBuffer>(sjson, &j));

                VersionPtr v = Version::get(j);

                // todo: check version compatibility.
                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("MemoryBufferFactory::parse_json: version mismatch.");
                return ptr;
            } // parse_json
        }; // MemoryBuffer
        //extern void to_json(Json& j, const MemoryBuffer& o);
        //extern void from_json(const Json& j, MemoryBuffer& o);
        //extern MemoryBuffer nlohmann::from_json (const Json& j);

        inline std::istream& operator >> (std::istream& is, MemoryBuffer& o)
        {
           return o.read(is);
        }

        inline std::ostream& operator << (std::ostream& os, const MemoryBuffer& o)
        {
            return o.write(os, o.byte_order ());
        }

        //inline std::stringstream& operator >> (std::stringstream& ss, MemoryBuffer& o) { o.read  (ss, TEXT); return ss; }
        //inline std::stringstream& operator << (std::stringstream& ss, MemoryBuffer& o) { o.write (ss, TEXT); return ss; }
        //inline std::stringstream& operator >> (std::stringstream& ss, MemoryBufferPtr o) { o->read(ss, TEXT); return ss; }
        //inline std::stringstream& operator << (std::stringstream& ss, MemoryBufferPtr o) { o->write(ss, TEXT); return ss; }

        inline       MemoryBuffer& operator >> (const String& sjson, MemoryBuffer& o) { return o.from_json(sjson); }

        inline std::istream& operator >> (std::istream& is, MemoryBufferPtr o) { return is >> *o; }
        inline std::ostream& operator << (std::ostream& os, const MemoryBufferPtr o) { return os << *o; }

        static_assert (std::is_move_constructible_v <MemoryBuffer>);
        static_assert (std::is_move_assignable_v    <MemoryBuffer>);

    } // namespace cpplib
} // namespace pensar_digital
#endif // MEMORY_BUFFER_HPP
