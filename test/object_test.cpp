// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "dummy.hpp"

#include <memory>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(ObjectClone)
            Object&  o = Object::get(42);
            Object& o1 = o.clone ();
            CHECK(o == o1, "0. o == o1 should be true");

            Dummy d(42, "d");
            Dummy d1 = pd::clone<Dummy> (d);
            Dummy d2 = d.clone();
            static_assert(OutputStreamable<Dummy>);
            CHECK_EQ(Dummy, d1, d, "1. d != d1");
            CHECK_EQ(Dummy, d2, d, "1. d != d2");
            TEST_END(ObjectClone)

        TEST(ObjectStreaming)
            Object& o = Object::get(42);
			std::stringstream ss;
			ss << o;
            String expected = "{\"class\":\"pensar_digital::cpplib::Object\",\"id\":42,\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
            CHECK_EQ(String, ss.str(), expected, "0");

            Object& o2 = Object::get();
            ss >> o2;
            CHECK(o == o2, "0. o == o2 should be true");

            std::stringstream ss2;

			Dummy& d = Dummy::get (42, "d");
            ss2 << d;
            expected =  "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"name\":\"d\",\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
            CHECK_EQ(String, ss2.str(), expected, "1");  
			Dummy& d2 = Dummy::get ();
			ss2 >> d2;
			CHECK_EQ(Dummy, d, d2, "1. d == d2 should be true");
        TEST_END(ObjectStreaming)

        TEST(ObjectJsonConversion)
			Object& o = Object::get(42);
			Json j = o;
            String expected = "{\"class\":\"pensar_digital::cpplib::Object\",\"id\":42,\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
            CHECK_EQ(String, j.dump (), expected, "0. json should be equal to " + expected + " but was " + j.dump() + ".");

            Object o1 = j;
            CHECK(o == o1, "1. o should be equal to o1");

           Dummy& d = Dummy::get (42, "d");
           j = d;
           expected = "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"name\":\"d\",\"private_interface_version\":1,\"protected_interface_version\":1,\"public_interface_version\":1}";
           CHECK_EQ(String, d.json (), expected, "2");

           Dummy& d1 = Dummy::get (1, "d1");
           j.get_to(d1);
           CHECK_EQ(Dummy, d, d1, "3. d should be equal to d1");
        TEST_END(ObjectJsonConversion)
        
        TEST(ObjectXMLConversion)
            Object& o = Object::get(42);
            String xml = o.xml ();
            String expected = "<object class_name = \"pensar_digital::cpplib::Object\" id = \"42\"/>";
            CHECK_EQ(String, xml, expected, "0. xml should be equal to " + expected + " but was " + xml + ".");

            Object& o1 = Object::get ();
            CHECK (o != o1, "1. o == o1");
            o1.from_xml(xml);
            CHECK (o == o1, "2. o != o1");

            Dummy& d = Dummy::get(42, "d");
            xml = d.xml_str ();
            expected = "<object class_name = \"pensar_digital::cpplib::Dummy\" id = \"42\"><name>d</name></object>";
            CHECK_EQ(String, xml, expected, "2. xml should be equal to " + expected + " but was " + xml + ".");

            Dummy& d1 = Dummy::get();
            CHECK(d != d1, "3. d == d1");
            d1.from_xml(xml);
            CHECK(d == d1, "4. d != d1");
        TEST_END(ObjectXMLConversion)
        
    }
}
