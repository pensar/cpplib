// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DUMMY_HPP
#define DUMMY_HPP

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "idummy.hpp"


#include <memory>

namespace pensar_digital
{
    namespace cpplib
    {
        class Dummy;
#pragma warning( disable : 4250) // Disable warning C4250: inherits via dominance.
typedef std::shared_ptr<Dummy> DummyPtr;

        /// <summary>
        /// Dummy class is streamable and comparable.
        /// </summary>
        class Dummy : public IDummy, public Object   
        {
        public:
            inline static const Version VERSION = Version(1, 1, 1);
            typedef IDummy    I;    // Interface type.
            typedef IDummy_RO I_RO; // Read only interface type.

            Dummy(const Id& id = NULL_ID, const String& aname = "") : Object(id), name(aname) {}
            Dummy(const Dummy& d) : Object(d) { name = d.name; }
            Dummy(Dummy&& d) noexcept : Object(d) { name = d.name; }
            Dummy& operator = (const Dummy& d) noexcept { assign(d); return *this; }
            Dummy& operator = (Dummy&& d) noexcept { assign(d); return *this; }
            bool operator == (const Dummy& d) const { return (Object::operator == (d) && (name == d.name)); }
            using Object::operator !=;
            virtual ~Dummy() {}
            virtual Dummy& assign(const Dummy& d) noexcept { Object::assign(d); name = d.name; return *this; }

            virtual String class_name() const noexcept { return Object::class_name (); }

            // Implements initialize method from Initializable concept.
            virtual bool initialize(const Id& aid = NULL_ID, const String& aname = "") noexcept
            {
                Object::set_id(aid);
                name = aname;
                return true;
            }

            IDummyPtr clone() const  noexcept { return pd::clone<Dummy>(*this, get_id (), name); }

            // Conversion to json string.
            virtual String json() const noexcept
            {
                return pd::json<Dummy>(*this);
            }

            virtual std::istream& read(std::istream& is)
            {
                return read_json<Dummy>(is, *this);
            };

            virtual std::ostream& write(std::ostream& os) const
            {
                return write_json<Dummy>(os, *this);
            };

            // Convertion to xml string.
            virtual String xml() const noexcept
            {
                String xml = ObjXMLPrefix() + ">";
                xml += "<name>" + name + "</name>";
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual void from_xml(const String& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                XMLNode n = node.getChildNode("name");
                if (!n.isEmpty()) name = n.getText();
            }

            virtual String get_name() const noexcept { return name; }
            void   set_name(const String& aname) noexcept { name = aname; }
            
            virtual String to_string() const noexcept { return Object::to_string () + " " + name; }

            /// Implicit conversion to string.
            /// \return A string with the object id.
            operator String () const noexcept { return to_string(); }

            /// Debug string.
            /// \return A string with the object id.
            virtual String debug_string() const noexcept
            {
				return Object::debug_string() + " name = " + name;
            }

            friend void to_json(Json& j, const Dummy& d);
            friend void from_json(const Json& j, Dummy& d);
        protected:
            virtual bool _equals(const Object& o) const { return Object::_equals(o); }
        private:

            String name;
        };
        extern void to_json(Json& j, const Dummy& o);
        extern void from_json(const Json& j, Dummy& o);

        extern std::istream& operator >> (std::istream& is, IDummy& o);
        extern std::ostream& operator << (std::ostream& os, const IDummy& o);

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // DUMMY_HPP