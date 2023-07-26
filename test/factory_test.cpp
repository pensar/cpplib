
// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../factory.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include <memory>

namespace pensar_digital
{
    namespace cpp = pensar_digital::cpplib;
    namespace obj = pensar_digital::cpplib::obj;
    namespace unit_test
    {
        TEST(NewFactory)
            {
                NewFactory <obj::Object> factory;
                std::shared_ptr<obj::Object> o  = factory.get ();
                NewFactory <obj::Object, pd::Id> factory1;
                std::shared_ptr<obj::Object> o1 = factory1.get (1);
                CHECK(*o != *o1, "0. o != o1 should be true");
                o.reset();
                CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
        }
        TEST_END(NewFactory)
        
        TEST(SingletonFactory)
        {
				SingletonFactory <obj::Object, pd::Id> factory (1);
				std::shared_ptr<obj::Object> o  = factory.get (1);
				std::shared_ptr<obj::Object> o1 = factory.get (1);
				CHECK(*o == *o1, "0. o == o1 should be true.");
				o.reset();
				CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
		}
        TEST_END(SingletonFactory)

        TEST(MockupFactory)
		{
            obj::Object* mockup = new obj::Object (1);
            MockupFactory<obj::Object, pd::Id> factory (mockup);
            std::shared_ptr<obj::Object> o  = factory.get (1);
            CHECK(*o == *mockup, "0. o == mockup should be true.");
        }
        TEST_END(MockupFactory)

        TEST(PoolFactory)
        {
			PoolFactory<obj::Object, pd::Id> factory (3, 10, 1);
            CHECK(factory.get_available_count() == 3, "0. available_count should be 3 but is " + pd::to_string((int)factory.get_available_count ()));
			std::shared_ptr<obj::Object> o  = factory.get (1);
            CHECK(o->get_id () == 1, "0. o->get_id () should be 1 but is " + pd::to_string((int)o->get_id ()))
            CHECK(factory.get_available_count() == 2, "1. available_count should be 2.");   
			std::shared_ptr<obj::Object> o1 = factory.get (2);
            CHECK(o1->get_id() == 2, "0. o1->get_id () should be 2 but is " + pd::to_string((int)o->get_id()))
            CHECK(factory.get_available_count() == 1, "2. available_count should be 1.");
            std::shared_ptr<obj::Object> o2 = factory.get (3);
            CHECK(o2->get_id() == 3, "0. o2->get_id () should be 3 but is " + pd::to_string((int)o->get_id()))
            CHECK(factory.get_available_count() == 0, "3. available_count should be 0.");
            std::shared_ptr<obj::Object> o3 = factory.get (4);
            CHECK(factory.get_available_count() == 9, "4. available_count should be 9 but is " + pd::to_string((int)factory.get_available_count()));
            CHECK(o3->get_id() == 4, "0. o3->get_id () should be 2 but is " + pd::to_string((int)o->get_id()))
            CHECK(*o != *o1, "0. *o != *o1 should be true.");
			o.reset();
			CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
		}
        TEST_END(PoolFactory)
    }
}
