// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "iobject.hpp"
#include "concept.hpp"
#include "string_util.hpp"
#include "header_lib/xmlParser.h"
#include "clone_util.hpp"
#include "xml_util.hpp"   // for read_xml and write_xml.
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

        class Object : public IObject
        {
            private:

                Id id; //!< Member variable "id"

            protected:

                /// Set id
                /// \param val New value to set
                void set_id(const Id& value) { id = value; }

                String ObjXMLPrefix() const noexcept { return "<object class_name = \"" + class_name() + "\" id = \"" + Object::to_string() + "\""; }

                /// \brief Compare objects.
                ///
                /// \param The object to be compared with.
                /// \return true if they are equal, false otherwise.
                /// \see equals
                ///
                virtual bool _equals(const IObjectRO& o) const { return (id == o.get_id ()); }

            public:
                inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
                
                typedef IObject    I;  // Interface type.
                typedef IObjectRO IRO; // Read only interface type.

                // Constructors. 
                
                /// Default constructor.
                Object(const Id& aid = NULL_ID) noexcept : id(aid) 
                {
                };

                /// Copy constructor
                /// \param other Object to copy from
                Object(const Object& o) { assign(o); }

                /// Move constructor
                Object(Object&& o) noexcept { assign(o); }

                /** Default destructor */
                virtual ~Object() {}

                Object& assign(const IObjectRO& o) noexcept { id = o.get_id(); return *this; }

                virtual String class_name() const { String c = typeid(*this).name(); c.erase(0, sizeof("class ") - 1); return c; }

                // Clone method. 
                IObjectPtr clone() const noexcept { return pd::clone<Object>(*this, id); }
                
                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                bool equals(const IObjectRO& o) const noexcept { return (get_hash() != o.get_hash() ? false : _equals(o)); }

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id get_id() const noexcept { return id; };

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual const Hash get_hash() const noexcept { return id; };

                // Implements initialize method from Initializable concept.
                virtual bool initialize(const Id& aid = NULL_ID) noexcept { id = aid; return true; }

                // Conversion to json string.
                inline virtual String json () const noexcept 
                { 
                    return pd::json<Object> (*this) + " }";
                }

                virtual std::istream& read (std::istream& is, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN);

                virtual std::ostream& write (std::ostream& os, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) const;

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
                        id = std::stoi(sid);
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

                bool operator == (const IObjectRO& o) const { return   equals(o); }
                bool operator != (const IObjectRO& o) const { return !equals(o); }

                /// Conversion to string.
                /// \return A string with the object id.
                virtual String to_string() const noexcept { return std::to_string(id); }

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
                Object& operator=(const IObjectRO& o) noexcept { return assign(o); }

                /// Move assignment operator
                Object& operator=(IObjectRO&& o) noexcept { return assign(o); }

                //friend void from_json(const Json& j, Object& o);
            };
            //extern void to_json(Json& j, const Object& o);
            //extern void from_json(const Json& j, Object& o);

            inline std::istream& operator >> (std::istream& is,          Object& o) { return o.read (is) ; }
            inline std::ostream& operator << (std::ostream& os, const   IObject& o) { return o.write (os); }
            inline std::istream& operator >> (std::istream& is,        ObjectPtr o) { return is >> *o    ; }
            inline std::ostream& operator << (std::ostream& os, const IObjectPtr o) { return os << *o    ; }


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
