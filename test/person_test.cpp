// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../person.hpp"
#include "../memory_buffer.hpp"
#include <chrono>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    using namespace pensar_digital::contact;
    namespace cpplib
    {
        TEST(Person, true)
            static_assert(StandardLayout<Person::Datatype>, "Person::DataType must be a standard layout type");
            static_assert(TriviallyCopyable<Person::Datatype>, "Person::DataType must be a trivially copyable type");   
            static_assert(StdLayoutTriviallyCopyable<Person::Datatype>, "Person must be a standard layout trivially copyable type");
            static_assert(Identifiable<Person>, "Person must be a Identifiable type");
            static_assert(Hashable<Person>, "Person must be a Hashable type");
 
            static_assert(Persistable<Person>, "Person must be a Persistable type");

            PersonName<> name = { "Mauricio", "", "Gomes" };
            //std::chrono::year year(1980);
            //std::chrono::month month(1);
            //std::chrono::day day(1);            
            Date dob(1980, 1, 1);
            Person::Datatype data = { name, dob };
            Person p(data);
            CHECK_EQ(Id, p.id (), 1, "0");

            PersonName<> expected = { "Mauricio", "", "Gomes" };
            CHECK_EQ(PersonName<>, p.name(), expected, "1");

            pd::ObjMemoryBuffer<Person> buffer;
            buffer.write(p);

            Person p2;
            CHECK_NOT_EQ(Person, p2, p, "2");

            buffer.read(&p2);
			CHECK_EQ(Person, p2, p, "3");

        TEST_END(Person)
    } // namespace cpplib
} // namespace pensar_digital