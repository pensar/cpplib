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
