// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include <memory>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        /// <summary>
        /// Dummy class is streamable and comparable.
        /// </summary>
        class Dummy: public Object 
        {
            public:
            const static Version VERSION                     = 1;
            const static Version PUBLIC_INTERFACE_VERSION    = 1;
            const static Version PROTECTED_INTERFACE_VERSION = 1;
            const static Version PRIVATE_INTERFACE_VERSION   = 1;

            virtual Version get_version                     () const noexcept { return VERSION;                     }
            virtual Version get_public_interface_version    () const noexcept { return PUBLIC_INTERFACE_VERSION;    }
            virtual Version get_protected_interface_version () const noexcept { return PROTECTED_INTERFACE_VERSION; }
            virtual Version get_private_interface_version   () const noexcept { return PRIVATE_INTERFACE_VERSION;   }

            Dummy (const Id& id = NULL_ID, const String& aname = "") : Object(id), name(aname) {}
            Dummy(const Dummy& d) : Object(d) { name = d.name; }
            Dummy(Dummy&& d) noexcept : Object(d) { name = d.name; }
			Dummy& operator = (const Dummy& d) noexcept {assign (d); return *this;}
			Dummy& operator = (Dummy&& d) noexcept {assign(d); return *this;}
            bool operator == (const Dummy& d) const {return (Object::operator == (d) && (name == d.name));}
            //std::ostream& operator << (std::ostream& os) const { Object::operator << (os); return os;  }
            /// Makes Dummy Streamable.
            friend std::ostream& operator << (std::ostream& os, const Dummy& d) { return os << d.get_id (); }   
            using Object::operator !=;
			virtual ~Dummy () {}
            virtual Dummy assign(const Dummy& d) noexcept { name = d.name; return *this; }

            // Convertion to xml string.
            virtual String xml_str () const noexcept 
            {
				String xml = ObjXMLPrefix() + ">";
                xml += "<name>" + name + "</name>";
				xml += "</object>";
				return xml;
			}

            // Convertion from xml string.
            virtual void from_xml(const String& sxml) 
            {
                XMLNode node = parse_object_tag (sxml);
                XMLNode n = node.getChildNode("name");
                if (!n.isEmpty()) name = n.getText();
            }

			/// Implicity convertion to object.
            /// \return A copy of the object.
            operator Object () const noexcept { return Object (get_id ()); }
            
            /// Implicity convertion to object.
            /// \return A copy of the object.
            /// \see Object::operator Object()
            /// \see Object::operator == (const Object&)
            operator Object& () noexcept { return *this; }

            String get_name () const noexcept { return name; }
            friend void to_json(Json& j, const Dummy& d);
            friend void from_json(const Json& j, Dummy& d);
			protected:
				virtual bool _equals (const Object& o) const {return Object::_equals (o);}
            private:
                
                String name;
        };

        void to_json(Json& j, const Dummy& d)
        {
            j["class"] = d.class_name();
            j["id"]    = d.get_id();
            j["name"]  = d.get_name();
        }

        void from_json(const Json& j, Dummy& d)
        {
            String class_name = d.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
            {
                d.set_id(j.at("id"));
            	d.name = j.at("name");
            }
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
        
        
        TEST(ObjectClone)
            Object o(42);
            Object o1 = pd::clone (o);
            CHECK(o == o1, "0. o == o1 should be true");

            Dummy d(42, "d");
            Dummy d1 = pd::clone<Dummy> (d);
            static_assert(OutputStreamable<Dummy>);
            CHECK_EQ(Dummy, d, d1, "1. d == d1 should be true");
        TEST_END(ObjectClone)

        TEST(ObjectJsonConversion)
			Object o(42);
			Json j = o;
            String expected = "{\"class\":\"pensar_digital::cpplib::Object\",\"id\":42}";
            CHECK_EQ(String, j.dump (), expected, "0. json should be equal to " + expected + " but was " + j.dump() + ".");

            Object o1 = j;
            CHECK(o == o1, "1. o should be equal to o1");

           Dummy d(42, "d");
           j = d;
           expected = "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"name\":\"d\"}";
           CHECK_EQ(String, j.dump (), expected, "2. json should be equal to " + expected + " but was " + j.dump() + ".");

           Dummy d1(1, "d1");
           j.get_to(d1);
           CHECK_EQ(Dummy, d, d1, "3. d should be equal to d1");
        TEST_END(ObjectJsonConversion)
        
        TEST(ObjectXMLConversion)
            Object o(42);
            String xml = o.xml_str ();
            String expected = "<object class_name = \"pensar_digital::cpplib::Object\" id = \"42\"/>";
            CHECK_EQ(String, xml, expected, "0. xml should be equal to " + expected + " but was " + xml + ".");

            Object o1;
            CHECK(o != o1, "1. o should be different from o1");
            o1.from_xml(xml);
            CHECK(o == o1, "1. o should be equal to o1");

            Dummy d(42, "d");
            xml = d.xml_str ();
            expected = "<object class_name = \"pensar_digital::cpplib::Dummy\" id = \"42\"><name>d</name></object>";
            CHECK_EQ(String, xml, expected, "2. xml should be equal to " + expected + " but was " + xml + ".");

            Dummy d1;
            CHECK(d != d1, "3. d should be different from d1");
            d1.from_xml(xml);
            CHECK(d == d1, "4. d should be equal to d1");
        TEST_END(ObjectXMLConversion)
        
    }
}
