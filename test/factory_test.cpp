
// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../factory.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object_factory.hpp"
#include <memory>

namespace pensar_digital
{
    using namespace pensar_digital::cpplib;
    namespace unit_test
    {
        TEST(NewFactory, true)
            {
                std::vector<ObjectPtr> v;
                v.push_back (std::make_shared<Object>(1));      
                CHECK(v[0].use_count() == 1, "0. use_count() should be 1 but is " + pd::to_string((int)v[0].use_count()));

                ObjectPtr ptr = v[0]; 
                CHECK(ptr.use_count() == 2, "1. use_count() should be 2 but is " + pd::to_string((int)ptr.use_count()));


                NewFactory <Object> factory;
                ObjectPtr o  = factory.get ();
                NewFactory <Object, pd::Id> factory1;
                ObjectPtr o1 = factory1.get (1);
                CHECK(*o != *o1, "0. o != o1 should be true");
                o.reset();
                CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
        }
        TEST_END(NewFactory)
        
        TEST(SingletonFactory, true)
        {
				SingletonFactory <Object, pd::Id> factory (1);
				ObjectPtr o  = factory.get (1);
				ObjectPtr o1 = factory.get (1);
				CHECK(*o == *o1, "0. o == o1 should be true.");
				o.reset();
				CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
		}
        TEST_END(SingletonFactory)

        TEST(MockupFactory, true)
		{
            Object* mockup = new Object (1);
            MockupFactory<Object, pd::Id> factory (mockup);
            ObjectPtr o  = factory.get (1);
            CHECK(*o == *mockup, "0. o == mockup should be true.");
        }
        TEST_END(MockupFactory)

        TEST(PoolFactory, true)
        {
			PoolFactory<Object, pd::Id> factory (3, 10, 1);
            {
                size_t count = factory.get_available_count();
                ObjectPtr ptr;
                for (size_t i = 0; i < count; i++)
                {
                    ptr = factory.get(1);
                    CHECK(factory.get_available_count () == factory.get_pool_size () - i - 1, "0.");
                }
                CHECK(factory.get_available_count() == 0, "0.1. available_count should be 0 but is " + pd::to_string((int)factory.get_available_count()));
            }
            factory.reset(3, 10, 0);

            CHECK(factory.get_available_count() == 3, "1. available_count should be 3 but is " + pd::to_string((int)factory.get_available_count ()));
			ObjectPtr o  = factory.get (1);
            CHECK(o->get_id () == 1, "0. o->get_id () should be 1 but is " + pd::to_string((int)o->get_id ()))
            CHECK(factory.get_available_count() == 2, "2. available_count should be 2.");   
			ObjectPtr o1 = factory.get (2);
            CHECK(o1->get_id() == 2, "3. o1->get_id () should be 2 but is " + pd::to_string((int)o->get_id()))
            CHECK(factory.get_available_count() == 1, "4. available_count should be 1.");
            ObjectPtr o2 = factory.get (3);
            CHECK(o2->get_id() == 3, "5. o2->get_id () should be 3 but is " + pd::to_string((int)o->get_id()))
            CHECK(factory.get_available_count() == 0, "6. available_count should be 0.");
            ObjectPtr o3 = factory.get (4);
            CHECK(factory.get_available_count() == 9, "7. available_count should be 9 but is " + pd::to_string((int)factory.get_available_count()));
            CHECK(o3->get_id() == 4, "8. o3->get_id () should be 4 but is " + pd::to_string((int)o->get_id()))
            CHECK(*o != *o1, "9. *o != *o1 should be true.");
			o.reset();
			CHECK(o.get () == nullptr, "10. managed object should have been deleted and assigned to nullptr.");
        }
        TEST_END(PoolFactory)
    }
}
