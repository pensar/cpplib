// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constant.hpp"
#include "concept.hpp"
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
#include <vector>

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        class Object;
        typedef std::shared_ptr<Object> ObjectPtr;
        class Object 
        {
            public:
                typedef pd::Factory<Object, Id> Factory;
            private:
                inline static Factory mfactory = { 3, 10, NULL_ID }; //!< Member variable "factory"
                Id mid; //!< Member variable "id"
            protected:

                /// Set id
                /// \param val New value to set
                void set_id(const Id& value) { mid = value; }

                String ObjXMLPrefix() const noexcept { return "<object class_name = \"" + class_name() + "\" id = \"" + Object::to_string() + "\""; }

                /// \brief Compare objects.
                ///
                /// \param The object to be compared with.
                /// \return true if they are equal, false otherwise.
                /// \see equals
                ///
                virtual bool _equals(const Object& o) const { return (mid == o.mid); }

            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                typedef Factory FactoryType;
                /// Default constructor.
                Object (const Id& id = NULL_ID, const IO_Mode mode = BINARY) noexcept : mid(id)
                {
                };

                /// Copy constructor
                /// \param other Object to copy from
                Object(const Object& o) { assign(o); }

                /// Move constructor
                Object(Object&& o) noexcept { assign(o); }

                /** Default destructor */
                virtual ~Object() {}

                Object& assign(const Object& o) noexcept { mid = o.mid; return *this; }

                inline virtual void bytes (std::vector<std::byte> v) const noexcept
                {
                    VERSION->bytes(v);
                }

                virtual std::span<std::byte> wbytes() noexcept { return std::as_writable_bytes (std::span {this, this + sizeof(*this)}); }

                virtual std::string class_name() const { String c = typeid(*this).name(); c.erase(0, sizeof("class ") - 1); return c; }

                // Clone method. 
                ObjectPtr clone() const noexcept { return pd::clone<Object>(*this, mid); }
                
                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                bool equals(const Object& o) const noexcept
                {
                    if (get_hash() != 
                        o.get_hash())
                        return false;
                        return _equals(o);
                }

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id id() const noexcept { return mid; };

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual const Hash get_hash() const noexcept { return mid; };

                // Implements initialize method from Initializable concept.
                virtual bool initialize(const Id& id = NULL_ID) noexcept 
                { 
                    mid = id; 
                    return true; 
                }

                // Conversion to json string.
                inline virtual String json () const noexcept 
                { 
                    return pd::json<Object> (*this) + " }";
                }

                virtual void read_bin_obj(std::istream& is, const std::endian& byte_order = std::endian::native);

                void read_bin_version(std::istream& is, const std::endian& byte_order = std::endian::native);

                virtual std::istream& read (std::istream& is, const IO_Mode amode = BINARY, const std::endian& byte_order = std::endian::native);

                virtual std::ostream& write (std::ostream& os, const IO_Mode amode = BINARY, const std::endian& byte_order = std::endian::native) const;

                // Conversion to xml string.
                virtual String xml() const noexcept { return ObjXMLPrefix() + "/>"; }

                XMLNode parse_object_tag(const pensar_digital::cpplib::String& sxml)
                {
                    const char* xml = sxml.c_str();
                    XMLCSTR tag = "object";
                    XMLResults* pResults = 0;
                    XMLNode node = XMLNode::parseString(xml, tag, pResults);
                    String xml_class_name = node.getAttribute("class_name");
                    if (xml_class_name == class_name())
                    {
                        String sid = node.getAttribute("id");
                        mid = std::stoi(sid);
                    }
                    else
                        throw std::runtime_error("Invalid class name");
                    return node;
                }

                // Conversion from xml string.
                virtual void from_xml(const String& sxml)
                {
                    parse_object_tag(sxml);
                }

                bool operator == (const Object& o) const { return   equals(o); }
                bool operator != (const Object& o) const { return !equals(o); }

                Object& from_json(const String& sjson);

                /// Conversion to string.
                /// \return A string with the object id.
                virtual String to_string() const noexcept { return std::to_string(mid); }

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
                /// \param o Object to assign from
                /// \return A reference to this
                Object& operator=(const Object& o) noexcept { return assign(o); }

                /// Move assignment operator
                Object& operator=(Object&& o) noexcept { return assign(o); }

                friend void from_json(const Json& j, Object& o);
                
                static inline Factory::P  get(const Id& aid = NULL_ID)
                {
                    return mfactory.get(aid);
                };

                Factory::P clone ()
                {
                    return get (mid);
                };

                inline static Factory::P get (const Json& j)
                {
                    String json_class = j.at("class");
                    if (json_class != pd::class_name<Object, char>())
                        throw std::runtime_error("Invalid class name: " + pd::class_name<Object, char>());
                    Factory::P ptr = get(j.at("id"));

                    VersionPtr v = Version::get(j["VERSION"]);

                    if (*(ptr->VERSION) != *v)
                        throw std::runtime_error("Factory::parse_json: version mismatch.");

                    return ptr;
                }

                inline static Factory::P get (const String& sjson)
                {
                    Json j;
                    Factory::P ptr = get(pd::id<Object>(sjson, &j));

                    VersionPtr v = Version::get(j);

                    // todo: check version compatibility.
                    if (*(ptr->VERSION) != *v)
                        throw std::runtime_error("Factory::parse_json: version mismatch.");
                    return ptr;
                } // parse_json
        }; // Object
            extern void to_json(Json& j, const Object& o);
            extern void from_json(const Json& j, Object& o);
            //extern Object nlohmann::from_json (const Json& j);

            inline std::istream& operator >> (std::istream& is, Object& o) 
            { 
				return o.read(is, TEXT);
            }
    
            inline std::ostream& operator << (std::ostream& os, const Object& o) 
            { 
                return o.write (os, TEXT);
            }

            inline Object& operator >> (const String& sjson, Object& o) { return o.from_json(sjson); }

            inline std::istream& operator >> (std::istream& is,       ObjectPtr o) { return is >> *o    ; }
            inline std::ostream& operator << (std::ostream& os, const ObjectPtr o) { return os << *o    ; }

            static_assert (std::is_move_constructible_v <Object>);
            static_assert (std::is_move_assignable_v    <Object>);  

            // Dependency class is a Constrainable class used to define dependencies between objects.
            template <Versionable MainClass, Versionable RequiredClass>
            class Dependency
            {
            private:
                VersionInt required_public_interface_version;
                VersionInt required_protected_interface_version;
                VersionInt required_private_interface_version;
            public:
                Dependency(Version v) noexcept
                    : required_public_interface_version(v.get_public ()),
                    required_protected_interface_version(v.get_protected ()),
                    required_private_interface_version(v.get_private ()) {}
                virtual ~Dependency() {}
                virtual bool ok() const noexcept = 0;

                // method to set the class dependency. 
            };
         } // namespace cpplib
} // namespace pensar_digital
#endif // OBJECT_HPP
