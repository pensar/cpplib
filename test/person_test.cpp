// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../person.hpp"
#include <chrono>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    using namespace pensar_digital::contact;
    namespace cpplib
    {
        TEST(Person, true)
            PersonName<> name = { "Mauricio", "", "Gomes" };
            std::chrono::year year(1980);
            std::chrono::month month(1);
            std::chrono::day day(1);            
            Date dob(year, month, day);
            Person::DataType data = { name, dob };
            Person p(data);
            static_assert(StdLayoutTriviallyCopyable<Person::DataType>, "Person must be a standard layout trivially copyable type");
            PersonName<> expected = { "Mauricio", "", "Gomes" };
            CHECK_EQ(PersonName<>, p.name(), expected, "0");
        TEST_END(Person)
    }
}