// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "string_def.hpp"
#include "cpplib_concepts.hpp"
#include "string_util.hpp"
#include "header_lib/json.hpp"
#include "header_lib/xmlParser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <concepts>
#include <exception>
#include <string>
#include <typeinfo>
#include <string.h>

namespace pensar_digital
{
    namespace cpplib
    {
        using Json = nlohmann::json;
        namespace pd = pensar_digital::cpplib;

        class Object
        {
            private:

            Id id; //!< Member variable "id"
            
            protected:

            String ObjXMLPrefix() const noexcept { return "<object class_name = \"" + class_name() + "\" id = \"" + to_string() + "\""; }

            virtual std::istream& ReadFromStream(std::istream& is)
            {
               return is >> id;
            };

            virtual std::ostream& WriteToStream(std::ostream& os) const
            {
                switch (get_public_interface_version ())
                {
                case 1:
                    return os << id;
                };
                return os;
            };

            /** Set id
             * \param val New value to set
             */
            void set_id(const Id& value) { id = value; }

            /// \brief Compare objects.
            ///
            /// \param The object to be compared with.
            /// \return true if they are equal, false otherwise.
            /// \see equals
            ///
            virtual bool _equals(const Object& o) const { return (id == o.id); }

            public:
            const static Version PUBLIC_INTERFACE_VERSION    = 1;
            const static Version PROTECTED_INTERFACE_VERSION = 1;
            const static Version PRIVATE_INTERFACE_VERSION   = 1;

            virtual Version get_public_interface_version    () const noexcept { return PUBLIC_INTERFACE_VERSION;    }
            virtual Version get_protected_interface_version () const noexcept { return PROTECTED_INTERFACE_VERSION; }
            virtual Version get_private_interface_version   () const noexcept { return PRIVATE_INTERFACE_VERSION;   }
            
            // get_version () returns a string formed by the concatenation of public, protected and private interface versions using a dot as separator.
            virtual String get_version () const noexcept 
            { 
                return pd::to_string<String, false> (get_public_interface_version ()) + "." + pd::to_string<String, false>(get_protected_interface_version ()) + "." + pd::to_string<String, false>(get_private_interface_version ());
            }
            
                

            virtual String class_name() const { String c = typeid(*this).name(); c.erase(0, sizeof("class ") - 1); return c; }

            /// Check if passed object equals self.
            /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
            /// _equals is called from template method equals and should only implement the specific comparison.
            /// \see _equals
            /// \return true if objects have the same id, false otherwise.
            const bool equals (const Object& o) const { return (get_hash () != o.get_hash () ? false : _equals (o)); }
                

            /// Access object id
            /// \return The current value of id
            ///
            virtual const Id get_id() const {return id;};

            /// \brief Access hash
            ///
            /// \return  The current value of hash
            virtual const Hash get_hash() const {return id;};
                
            // Conversion to json string.
            virtual String json_str() const { Json j = *this; return j.dump(); }

            // Conversion to xml string.s
            virtual String xml_str() const noexcept 
            { 
                return ObjXMLPrefix () + "/>";
            }

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
                     
            bool operator == (const Object& o) const {return   equals (o);}
            bool operator != (const Object& o) const {return ! equals (o);}
            /** Default constructor */
            Object () noexcept : id (NULL_ID) {};
            Object(Id aid) noexcept : id (aid) {};
                
            /// Move constructor
            Object(Object&& o) noexcept {assign (o);}

            /// Move assignment operator
            Object& operator=(Object&& o) noexcept {return assign (o);}

            /// Conversion to string.
            /// \return A string with the object id.
            virtual String to_string () const {return std::to_string (id);}

            /// Implicit conversion to string.
            /// \return A string with the object id.
            operator String () const {return to_string ();}

            /// Debug string.
            /// \return A string with the object id.
            virtual String debug_string() const 
            { 
                std::stringstream ss;
                ss << "id = " << to_string(); 
                return ss.str ();

            }

            /** Default destructor */
            virtual ~Object() {}

            /** Copy constructor
                *  \param other Object to copy from
                */
            Object(const Object& o) {assign (o);}


            virtual Object& assign (const Object& o) {id = o.get_id (); return *this;}

            /** Assignment operator
                *  \param o Object to assign from
                *  \return A reference to this
                */
            Object& operator=(const Object& o) { return assign (o); }

            virtual bool initialize(Id aid = NULL_ID) noexcept { id = aid; return true; }
            std::istream& operator >> (std::istream& is)       { return ReadFromStream (is);};
            std::ostream& operator << (std::ostream& os) const { return WriteToStream  (os);};
            friend void from_json(const Json& j, Object& o);
         };

         template<std::copy_constructible T = Object>
         static T clone(const T& o) { return T(o); }

        static_assert(Initializable<Object, Id>);

        extern void to_json(Json& j, const Object& o);
        extern void from_json(const Json& j, Object& o);
        
        // Dependency class is a Constrainable class used to define dependencies between objects.
        template <Versionable MainClass, Versionable RequiredClass>
        class Dependency
        {
            private:
                Version required_public_interface_version;
                Version required_protected_interface_version;   
                Version required_private_interface_version;
            public:
			Dependency (Version public_interface_version, Version protected_interface_version, Version private_interface_version) noexcept
				: required_public_interface_version (public_interface_version), required_protected_interface_version (protected_interface_version), required_private_interface_version (private_interface_version) {}
				virtual ~Dependency () {}
                virtual bool ok() const noexcept = 0;

            // method to set the class dependency. 
        };
   } // namespace pensar_digital::cpplib
} // namespace pensar_digital
#endif // OBJECT_HPP
