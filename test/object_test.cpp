// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../s.hpp"
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
        //static_assert(Assignable<Dummy>);

        TEST(ObjectClone, true)
            Factory::P o = pd::Object::get(42);
            Factory::P o1 = o->clone ();
            CHECK(*o == *o1,W("0. o == o1 should be true"));

            DummyPtr d = Dummy::get (42,W("d"));
            DummyPtr d1 = d->clone();
            static_assert(OutputStreamable<Dummy>);
            CHECK_EQ(Dummy, *d1, *d,W("1. d != d1"));
        TEST_END(ObjectClone)

        TEST(ObjectStreaming, true)
            ObjectPtr o = pd::Object::get(42);
			SStream ss;
			ss << *o;
            S expected = W("{ \"class\" : \"pensar_digital::cpplib::Object\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }");
            CHECK_EQ(S, ss.str(), expected, W("0"));

            ObjectPtr o2 = pd::Object::get();
            o->json () >> *o2;
            CHECK(*o == *o2, W("0. o == o2 should be true"));

			DummyPtr d = Dummy::get (42, W("d"));
            SStream ss3;
            ss3 << *d;
            S expected2 = W("{ \"class\" : \"pensar_digital::cpplib::Dummy\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"name\" : \"d\" }");
            CHECK_EQ(S, ss3.str(), expected2, W("1"));
			DummyPtr d2 = Dummy::get ();
			ss3 >> *d2;
			CHECK_EQ(Dummy, *d, *d2, W("1. d == d2 should be true"));
        TEST_END(ObjectStreaming)

        TEST(ObjectJsonConversion, true)
            ObjectPtr o = pd::Object::get(42);
            S expected = W("{ \"class\" : \"pensar_digital::cpplib::Object\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }");
            CHECK_EQ(S, o->json (), expected, W("0."));

            ObjectPtr o1 = pd::Object::get (o->json());
            CHECK(*o == *o1, W("1. o should be equal to o1"));

           DummyPtr d = Dummy::get (42, W("d"));
           expected = W("{ \"class\" : \"pensar_digital::cpplib::Dummy\", \"id\" : 42, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"name\" : \"")
                        + d->get_name() + W("\" }");
           CHECK_EQ(S, d->json (), expected, W("2"));

           DummyPtr d1 = Dummy::parse_json (d->json());
           CHECK_EQ(Dummy, *d, *d1, W("3. d should be equal to d1"));
        TEST_END(ObjectJsonConversion)
        
        TEST(ObjectXMLConversion, true)
            ObjectPtr o = pd::Object::get(42);
            S xml = o->xml ();
            S expected = W("<object class_name = \"pensar_digital::cpplib::Object\" id = \"42\"/>");
            CHECK_EQ(S, xml, expected, W("0. xml should be equal to ") + expected + W(" but was ") + xml + W("."));

            ObjectPtr o1 = pd::Object::get ();
            CHECK_NOT_EQ(Object, *o, *o1, W("1. o == o1"));
            o1->from_xml(xml);
            CHECK_EQ(Object, *o, *o1, W("2. o != o1"));

            DummyPtr d = Dummy::get(42, W("d"));
            xml = d->xml ();
            expected = W("<object class_name = \"pensar_digital::cpplib::Dummy\" id = \"42\"><name>d</name></object>");
            CHECK_EQ(S, xml, expected, W("2. xml should be equal to ") + expected + W(" but was ") + xml + W("."));

            DummyPtr d1 = Dummy::get();
            CHECK(*d != *d1, W("3. d == d1"));
            d1->from_xml(xml);
            CHECK(*d == *d1, W("4. d != d1"));
            TEST_END(ObjectXMLConversion) 

            TEST(ObjectJsonString, true)
                // Creates a vector with 1000 objects
                std::vector<ObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
			        objects.push_back(pd::Object::get(i));
				}
                for (Id i = 0; i < 1000; i++)
                {
                    S si = pd::to_string(i);
                    ObjectPtr o = pd::Object::get(i);
                    S expected = W("{ \"class\" : \"pensar_digital::cpplib::Object\", \"id\" : ") + si;
                    expected += W(", \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }");
                    CHECK_EQ(S, o->json(), expected, si);
                }
            TEST_END(ObjectJsonString)

            TEST(ObjectTextFileStreaming, true)
                // Creates a vector with 1000 objects
                std::vector<ObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
                    ObjectPtr p = pd::Object::get(i);
                    objects.push_back(p);
                }
                //pd::TextFile f("c:\\tmp\\test\\ObjectTextFileStreaming\\test.txt", W("blah"));
                std::ofstream out("c:\\tmp\\test\\ObjectTextFileStreaming\\test.txt", File::OUTPUT);
                out << W("[");
                for (Id i = 0; i < 999; i++)
                {
                    out << *objects[i] << W(", ") << std::endl;
                }
                out << *objects[999] << W("]");
                out.flush ();
                out.close();

               InFileStream f (W("c:\\tmp\\test\\ObjectTextFileStreaming\\test.txt"));
               S s;
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

                TEST_END(ObjectTextFileStreaming)
                
            TEST(ObjectBinaryFileStreaming, true)
				// Creates a vector with 1000 objects
				std::vector<ObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
					objects.push_back(pd::Object::get(i));
				}
                OutFStream out (W("c:\\tmp\\test\\ObjectBinaryFileStreaming\\test.bin"), std::ios::binary);

                for (Id i = 0; i < 1000; i++)
                {
                    objects[i]->write (out) ;    
                }
				out.close();
                InFileStream in (W("c:\\tmp\\test\\ObjectBinaryFileStreaming\\test.bin"), std::ios::binary);
                for (Id i = 0; i < 1000; i++)
                {
					ObjectPtr o = pd::Object::get();
					o->read (in);
                    ObjectPtr o1 = pd::Object::get(i);
                    CHECK_EQ(Object, *o, *o1, pd::to_string(i));
                }
             TEST_END(ObjectBinaryFileStreaming)


    }
}
