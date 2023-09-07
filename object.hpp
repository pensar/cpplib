// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "concept.hpp"
#include "string_util.hpp"
#include "header_lib/xmlParser.h"
#include "clone_util.hpp"
#include "version_factory.hpp"
#include "json_util.hpp"

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
        class Object;
        typedef std::shared_ptr<Object> ObjectPtr;

        class Object 
        {
            private:

                Id mid; //!< Member variable "id"
                IO_Mode mmode;
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
                inline static const VersionPtr VERSION = pd::versionf.get (1, 1, 1);
                // Verifies if Object complies with Versionable concept.
                typedef Object   I; // Interface type.
                typedef Object IRO; // Read only interface type.

                // Constructors. 
                
                /// Default constructor.
                Object(const Id& id = NULL_ID, const IO_Mode& mode = BINARY) noexcept : mid(id) 
                {
                };

                /// Copy constructor
                /// \param other Object to copy from
                Object(const Object& o) { assign(o); }

                /// Move constructor
                Object(Object&& o) noexcept { assign(o); }

                /** Default destructor */
                virtual ~Object() {}

                Object& assign(const Object& o) noexcept { mid = o.mid; mmode = o.mmode; return *this; }

                virtual String class_name() const { String c = typeid(*this).name(); c.erase(0, sizeof("class ") - 1); return c; }

                // Clone method. 
                ObjectPtr clone() const noexcept { return pd::clone<Object>(*this, mid); }
                
                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                bool equals(const Object& o) const noexcept { return (get_hash() != o.get_hash() ? false : _equals(o)); }

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id get_id() const noexcept { return mid; };

                /// <summary>
                ///  Sets io mode.
                /// </summary>
                /// <returns></returns>
                void set_mode(const IO_Mode& mode) noexcept { mmode = mode; }

                /// <summary>
                ///  Gets io mode.
                /// </summary>
                /// <returns>IO_Mode</returns>
                const IO_Mode& get_mode() const noexcept { return mmode; }

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual const Hash get_hash() const noexcept { return mid; };

                // Implements initialize method from Initializable concept.
                virtual bool initialize(const Id& id = NULL_ID, const IO_Mode& mode = BINARY) noexcept { mid = id; mmode = mode; return true; }

                // Conversion to json string.
                inline virtual String json () const noexcept 
                { 
                    return pd::json<Object> (*this) + " }";
                }

                virtual std::istream& read (std::istream& is, const IO_Mode& amode = BINARY, const ByteOrder& abyte_order = LITTLE_ENDIAN);

                virtual std::ostream& write (std::ostream& os, const IO_Mode& amode = BINARY, const ByteOrder& abyte_order = LITTLE_ENDIAN) const;

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

                Object& parse_json(const String& sjson);

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
            };
            extern void to_json(Json& j, const Object& o);
            extern void from_json(const Json& j, Object& o);

            inline std::istream& operator >> (std::istream& is, Object& o) 
            { 
                // if is is a stringstream, then calls read with TEXT mode.
                if (typeid(is) == typeid(std::stringstream))
					return o.read(is, TEXT);
				else
                    return o.read  (is) ; 
            }
    
            inline std::ostream& operator << (std::ostream& os, const Object& o) 
            { 
                // if os is a stringstream, then calls write with TEXT mode.
                if (typeid(os) == typeid(std::stringstream))
                    const_cast<Object&>(o).set_mode (TEXT); 
                
                return o.write (os, o.get_mode ());
            }

            //inline std::stringstream& operator >> (std::stringstream& ss, Object& o) { o.read  (ss, TEXT); return ss; }
            //inline std::stringstream& operator << (std::stringstream& ss, Object& o) { o.write (ss, TEXT); return ss; }
            //inline std::stringstream& operator >> (std::stringstream& ss, ObjectPtr o) { o->read(ss, TEXT); return ss; }
            //inline std::stringstream& operator << (std::stringstream& ss, ObjectPtr o) { o->write(ss, TEXT); return ss; }

            inline       Object& operator >> (const String& sjson      , Object& o) { return o.parse_json(sjson); }

            inline std::istream& operator >> (std::istream& is,       ObjectPtr o) { return is >> *o    ; }
            inline std::ostream& operator << (std::ostream& os, const ObjectPtr o) { return os << *o    ; }


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
