// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "../object_factory.hpp"
#include "dummy_factory.hpp"
#include "../io_util.hpp"
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
        ObjectFactory objectf;
        DummyFactory dummyf;
        
        static_assert(Assignable<Dummy>);

        TEST(ObjectClone, true)
            IObjectPtr  o = pd::objectf.get(42);
            IObjectPtr o1 = pd::objectf.clone (*o);
            CHECK(*o == *o1, "0. o == o1 should be true");

            IDummyPtr d = dummyf.get (42, "d");
            IDummyPtr d1 = dummyf.clone(d);
            static_assert(OutputStreamable<IDummy>);
            CHECK_EQ(IDummy, *d1, *d, "1. d != d1");
            TEST_END(ObjectClone)

        TEST(ObjectStreaming, true)
            IObjectPtr o = objectf.get(42);
			std::stringstream ss;
			ss << *o;
            String expected = "{\"class\":\"pensar_digital::cpplib::Object\",\"id\":42,\"mprivate\":1,\"mprotected\":1,\"mpublic\":1}";
            CHECK_EQ(String, ss.str(), expected, "0");

            ObjectPtr o2 = objectf.get();
            ss >> *o2;
            CHECK(*o == *o2, "0. o == o2 should be true");

            std::stringstream ss2;

			IDummyPtr d = dummyf.get (42, "d");
            ss2 << *d;
            expected =  "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"mprivate\":1,\"mprotected\":1,\"mpublic\":1,\"name\":\"d\"}";
            CHECK_EQ(String, ss2.str(), expected, "1");  
			DummyPtr d2 = dummyf.get ();
			ss2 >> *d2;
			CHECK_EQ(IDummy, *d, *d2, "1. d == d2 should be true");
        TEST_END(ObjectStreaming)

        TEST(ObjectJsonConversion, true)
            IObjectPtr o = objectf.get(42);
            String expected = "{ \"class\" : pensar_digital::cpplib::Object, \"id\" : 42, { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 } }";
            CHECK_EQ(String, o->json (), expected, "0.");

            IObjectPtr o1 = objectf.parse_json (o->json());
            CHECK(*o == *o1, "1. o should be equal to o1");

           IDummyPtr d = dummyf.get (42, "d");
           expected = "{\"class\":\"pensar_digital::cpplib::Dummy\",\"id\":42,\"mprivate\":1,\"mprotected\":1,\"mpublic\":1,\"name\":\"d\"}";
           CHECK_EQ(String, d->json (), expected, "2");

           IDummyPtr d1 = dummyf.parse_json (d->json());
           CHECK_EQ(IDummy, *d, *d1, "3. d should be equal to d1");
        TEST_END(ObjectJsonConversion)
        
        TEST(ObjectXMLConversion, true)
            IObjectPtr o = objectf.get(42);
            String xml = o->xml ();
            String expected = "<object class_name = \"pensar_digital::cpplib::Object\" id = \"42\"/>";
            CHECK_EQ(String, xml, expected, "0. xml should be equal to " + expected + " but was " + xml + ".");

            IObjectPtr o1 = objectf.get ();
            CHECK_NOT_EQ(IObject, *o, *o1, "1. o == o1");
            o1->from_xml(xml);
            CHECK_EQ(IObject, *o, *o1, "2. o != o1");

            IDummyPtr d = dummyf.get(42, "d");
            xml = d->xml ();
            expected = "<object class_name = \"pensar_digital::cpplib::Dummy\" id = \"42\"><name>d</name></object>";
            CHECK_EQ(String, xml, expected, "2. xml should be equal to " + expected + " but was " + xml + ".");

            IDummyPtr d1 = dummyf.get();
            CHECK(*d != *d1, "3. d == d1");
            d1->from_xml(xml);
            CHECK(*d == *d1, "4. d != d1");
            TEST_END(ObjectXMLConversion) 

            TEST(ObjectTextStreaming, true)
                // Creates a vector with 1000 objects
                std::vector<IObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
			        objects.push_back(objectf.get(i));
				}
                for (Id i = 0; i < 1000; i++)
                {
                    String si = pd::to_string(i);
                    IObjectPtr o = objectf.get(i);
                    String expected = "{\"class\":\"pensar_digital::cpplib::Object\",\"id\":" + si;
                    expected += ",\"mprivate\":1,\"mprotected\":1,\"mpublic\":1}";
                    CHECK_EQ(String, o->json(), expected, si);
                }
            TEST_END(ObjectTextStreaming)

            TEST(ObjectTextStreaming2, true)
                // Creates a vector with 1000 objects
                std::vector<IObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
                    objects.push_back(objectf.get(i));
                }
                //pd::TextFile f("c:\\tmp\\test\\ObjectTextStreaming\\test.txt", "blah");
                std::ofstream out("c:\\tmp\\test\\ObjectTextStreaming\\test.txt");

                for (Id i = 0; i < 1000; i++)
                {
                    out << *objects[i] << std::endl;
                }
                out.flush ();
                out.close();

                std::ifstream in("c:\\tmp\\test\\ObjectTextStreaming\\test.txt");
                for (Id i = 0; i < 1000; i++)
                {
                    ObjectPtr o = objectf.get();
                    in >> o;
                    IObjectPtr o1 = objectf.get(i);
                    CHECK_EQ(IObject, *o, *o1, pd::to_string(i));
                }
                TEST_END(ObjectTextStreaming2)
                
            TEST(ObjectBinaryStreaming, false)
				// Creates a vector with 1000 objects
				std::vector<IObjectPtr> objects;
                for (Id i = 0; i < 1000; i++)
                {
					objects.push_back(objectf.get(i));
				}
                std::ofstream out ("c:\\tmp\\test\\ObjectBinaryStreaming\\test.bin", std::ios::binary);

                for (Id i = 0; i < 1000; i++)
                {
                    out << objects[i] << std::endl;    
                }
				out.close();
                std::ifstream in ("c:\\tmp\\test\\ObjectBinaryStreaming\\test.bin", std::ios::binary);
                for (Id i = 0; i < 1000; i++)
                {
					ObjectPtr o = objectf.get();
					in >> o;
                    IObjectPtr o1 = objectf.get(i);
                    CHECK_EQ(IObject, *o, *o1, pd::to_string(i));
                }
             TEST_END(ObjectBinaryStreaming)


    }
}
