
// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../factory.hpp"
#include "../string_def.hpp"
#include <memory>

namespace pensar_digital
{
    namespace pd = pensar_digital::cpplib;
    namespace unit_test
    {
        TEST(NewFactory)
            {
                pd::NewFactory <pd::Object> factory;
                std::shared_ptr<pd::Object> o  = factory.get ( );
                pd::NewFactory <pd::Object, pd::Id> factory1;
                std::shared_ptr<pd::Object> o1 = factory1.get (1);
                CHECK(*o != *o1, "0. o != o1 should be true");
                o.reset();
                CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
        }
        TEST_END(NewFactory)
        
        TEST(SingletonFactory)
        {
				pd::SingletonFactory <pd::Object, pd::Id> factory (1);
				std::shared_ptr<pd::Object> o  = factory.get (2);
				std::shared_ptr<pd::Object> o1 = factory.get (3);
				CHECK(*o == *o1, "0. o == o1 should be true.");
				o.reset();
				CHECK(o.get () == nullptr, "1. managed object should have been deleted and assigned to nullptr.");
		}
        TEST_END(SingletonFactory)
        
        TEST(MockupFactory)
		{
            Object* mockup = new pd::Object (1);
            pd::MockupFactory<pd::Object, pd::Id> factory (mockup);
            std::shared_ptr<pd::Object> o  = factory.get (2);
            CHECK(*o == *mockup, "0. o == mockup should be true.");
        }
        TEST_END(MockupFactory)
    }
}
