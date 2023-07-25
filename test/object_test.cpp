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
        using namespace pensar_digital::cpplib::obj;
        /// <summary>
        /// Dummy class is streamable and comparable.
        /// </summary>
        class Dummy: public Object 
        {
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);

            Dummy (const Id& id = NULL_ID, const String& aname = "") : Object(id), name(aname) {}
            Dummy(const Dummy& d) : Object(d) { name = d.name; }
            Dummy(Dummy&& d) noexcept : Object(d) { name = d.name; }
			Dummy& operator = (const Dummy& d) noexcept {assign (d); return *this;}
			Dummy& operator = (Dummy&& d) noexcept {assign(d); return *this;}
            bool operator == (const Dummy& d) const {return (Object::operator == (d) && (name == d.name));}
            using Object::operator !=;
			virtual ~Dummy () {}
            virtual Dummy assign(const Dummy& d) noexcept { name = d.name; return *this; }
            
            Dummy clone () const { return pd::clone<Dummy>(*this); }

            // Conversion to json string.
            virtual String json() const
            {
                return Object::json<Dummy> (*this);
            }

            virtual std::istream& read(std::istream& is)
            {
                return Object::read<Dummy> (is, *this);
            };

            virtual std::ostream& write(std::ostream& os) const
            {
                return Object::write<Dummy> (os, *this);
            };

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

            String get_name () const noexcept { return name; }
            void   set_name (const String& aname) noexcept { name = aname; }


            friend void to_json(Json& j, const Dummy& d);
            friend void from_json(const Json& j, Dummy& d);
			protected:
				virtual bool _equals (const Object& o) const {return Object::_equals (o);}
            private:
                
                String name;
        };

        /// Makes Dummy Streamable.
        std::ostream& operator << (std::ostream& os, const Dummy& d) { return d.write (os); }
        std::istream& operator >> (std::istream& is, Dummy& d) { return d.read (is); }

        
        
        void to_json(Json& j, const Dummy& d)
        {
            j["class"                      ] = d.class_name();
            j["id"                         ] = d.get_id();
            j["private_interface_version"  ] = d.VERSION.PRIVATE;
            j["protected_interface_version"] = d.VERSION.PROTECTED;
            j["public_interface_version"   ] = d.VERSION.PUBLIC;
            j["name"] = d.get_name();
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
            obj::Object o(42);
            obj::Object o1 = o.clone ();
            CHECK(o == o1, "0. o == o1 should be true");

            Dummy d(42, "d");
            Dummy d1 = pd::clone<Dummy> (d);
            Dummy d2 = d.clone();
            static_assert(OutputStreamable<Dummy>);
            CHECK_EQ(Dummy, d, d1, "1. d == d1 should be true");
        TEST_END(ObjectClone)

        TEST(ObjectStreaming)
            Object o(42);
			std::stringstream ss;
			ss << o;
            String expected = "{\"class\":\"pensar_digital::cpplib::obj::Object\",\"id\":42,\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
            CHECK_EQ(String, ss.str(), expected, "0");

            Object o2;
            ss >> o2;
            CHECK(o == o2, "0. o == o2 should be true");

            std::stringstream ss2;

			Dummy d(42, "d");
            ss2 << d;
            expected =  "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"name\":\"d\",\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
            CHECK_EQ(String, ss2.str(), expected, "1");  
			Dummy d2;
			ss2 >> d2;
			CHECK_EQ(Dummy, d, d2, "1. d == d2 should be true");
        TEST_END(ObjectStreaming)

        TEST(ObjectJsonConversion)
			Object o(42);
			Json j = o;
            String expected = "{\"class\":\"pensar_digital::cpplib::obj::Object\",\"id\":42,\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
            CHECK_EQ(String, j.dump (), expected, "0. json should be equal to " + expected + " but was " + j.dump() + ".");

            Object o1 = j;
            CHECK(o == o1, "1. o should be equal to o1");

           Dummy d(42, "d");
           j = d;
           expected = "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"name\":\"d\",\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
           CHECK_EQ(String, d.json (), expected, "2");

           Dummy d1(1, "d1");
           j.get_to(d1);
           CHECK_EQ(Dummy, d, d1, "3. d should be equal to d1");
        TEST_END(ObjectJsonConversion)
        
        TEST(ObjectXMLConversion)
            Object o(42);
            String xml = o.xml ();
            String expected = "<object class_name = \"pensar_digital::cpplib::obj::Object\" id = \"42\"/>";
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
