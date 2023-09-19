// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "dummy_factory.hpp"
#include "../io_util.hpp"
#include "../file.hpp"
#include <memory>

#ifdef _MSC_VER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        static_assert(Assignable<Dummy>);

        TEST(ObjectClone, true)
            ObjectFactory::P o = pd::Object::get(42);
            ObjectFactory::P o1 = o->clone ();
            CHECK(*o == *o1, "0. o == o1 should be true");

            DummyPtr d = Dummy::get (42, "d");
            DummyPtr d1 = d->clone();
            static_assert(OutputStreamable<Dummy>);
            CHECK_EQ(Dummy, *d1, *d, "1. d != d1");
            TEST_END(ObjectClone)

        TEST(ObjectStreaming, true)
            ObjectPtr o = pd::Object::get(42);
			std::stringstream ss;
			ss << *o;
            String expected = "{ \"class\" : \"pensar_digital::cpplib::Object\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }";
            CHECK_EQ(String, ss.str(), expected, "0");

            ObjectPtr o2 = pd::Object::get();
            o->json () >> *o2;
            CHECK(*o == *o2, "0. o == o2 should be true");

			DummyPtr d = Dummy::get (42, "d");
            std::stringstream ss3;
            ss3 << *d;
            String expected2 = "{ \"class\" : \"pensar_digital::cpplib::Dummy\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"name\" : \"d\" }";
            CHECK_EQ(String, ss3.str(), expected2, "1");
			DummyPtr d2 = Dummy::get ();
			ss3 >> *d2;
			CHECK_EQ(Dummy, *d, *d2, "1. d == d2 should be true");
        TEST_END(ObjectStreaming)

        TEST(ObjectJsonConversion, true)
            ObjectPtr o = pd::Object::get(42);
            String expected = "{ \"class\" : \"pensar_digital::cpplib::Object\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }";
            CHECK_EQ(String, o->json (), expected, "0.");

            ObjectPtr o1 = pd::Object::get (o->json());
            CHECK(*o == *o1, "1. o should be equal to o1");

           DummyPtr d = Dummy::get (42, "d");
           expected = "{ \"class\" : \"pensar_digital::cpplib::Dummy\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"name\" : \""
                        + d->get_name() + "\" }";
           CHECK_EQ(String, d->json (), expected, "2");

           DummyPtr d1 = Dummy::parse_json (d->json());
           CHECK_EQ(Dummy, *d, *d1, "3. d should be equal to d1");
        TEST_END(ObjectJsonConversion)
        
        TEST(ObjectXMLConversion, true)
            ObjectPtr o = pd::Object::get(42);
            String xml = o->xml ();
            String expected = "<object class_name = \"pensar_digital::cpplib::Object\" id = \"42\"/>";
            CHECK_EQ(String, xml, expected, "0. xml should be equal to " + expected + " but was " + xml + ".");

            ObjectPtr o1 = pd::Object::get ();
            CHECK_NOT_EQ(Object, *o, *o1, "1. o == o1");
            o1->from_xml(xml);
            CHECK_EQ(Object, *o, *o1, "2. o != o1");

            DummyPtr d = Dummy::get(42, "d");
            xml = d->xml ();
            expected = "<object class_name = \"pensar_digital::cpplib::Dummy\" id = \"42\"><name>d</name></object>";
            CHECK_EQ(String, xml, expected, "2. xml should be equal to " + expected + " but was " + xml + ".");

            DummyPtr d1 = Dummy::get();
            CHECK(*d != *d1, "3. d == d1");
            d1->from_xml(xml);
            CHECK(*d == *d1, "4. d != d1");
            TEST_END(ObjectXMLConversion) 

            TEST(ObjectTextStreaming, true)
                // Creates a vector with 1000 objects
                std::vector<ObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
			        objects.push_back(pd::Object::get(i));
				}
                for (Id i = 0; i < 1000; i++)
                {
                    String si = pd::to_string(i);
                    ObjectPtr o = pd::Object::get(i);
                    String expected = "{ \"class\" : \"pensar_digital::cpplib::Object\", \"id\" : " + si;
                    expected += ", \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }";
                    CHECK_EQ(String, o->json(), expected, si);
                }
            TEST_END(ObjectTextStreaming)

            TEST(ObjectTextStreaming2, true)
                // Creates a vector with 1000 objects
                std::vector<ObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
                    ObjectPtr p = pd::Object::get(i);
                    p->set_mode(TEXT);
                    objects.push_back(p);
                }
                //pd::TextFile f("c:\\tmp\\test\\ObjectTextStreaming\\test.txt", "blah");
                std::ofstream out("c:\\tmp\\test\\ObjectTextStreaming\\test.txt", File::OUTPUT);
                out << "[";
                for (Id i = 0; i < 999; i++)
                {
                    out << *objects[i] << ", " << std::endl;
                }
                out << *objects[999] << "]";
                out.flush ();
                out.close();

               std::ifstream f ("c:\\tmp\\test\\ObjectTextStreaming\\test.txt");
               String s;
               pd::read_all (f, s);
                
                Json j = Json::parse (s);

                int i = 0;
                for (const auto& item : j)
                //for (int i = 0; i < 1000; i++)
                {
                    Object o = item;    
                    CHECK_EQ(Object, o, *objects[i], pd::to_string(i));
                    ++i;
                }

                TEST_END(ObjectTextStreaming2)
                
            TEST(ObjectBinaryStreaming, true)
				// Creates a vector with 1000 objects
				std::vector<ObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
					objects.push_back(pd::Object::get(i));
				}
                std::ofstream out ("c:\\tmp\\test\\ObjectBinaryStreaming\\test.bin", std::ios::binary);

                for (Id i = 0; i < 1000; i++)
                {
                    out << objects[i];    
                }
				out.close();
                std::ifstream in ("c:\\tmp\\test\\ObjectBinaryStreaming\\test.bin", std::ios::binary);
                for (Id i = 0; i < 1000; i++)
                {
					ObjectPtr o = pd::Object::get();
					in >> o;
                    ObjectPtr o1 = pd::Object::get(i);
                    CHECK_EQ(Object, *o, *o1, pd::to_string(i));
                }
             TEST_END(ObjectBinaryStreaming)


    }
}
