// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../cpplib/cpp//person.hpp"
#include "../cpplib/cpp/memory_buffer.hpp"
#include <chrono>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    using namespace pensar_digital::contact;
    namespace cpplib
    {
        namespace contact
        {
            TEST(Person, true)
                static_assert(StandardLayout            <Contact>, "PersonName<> must be a standard layout type");
                static_assert(TriviallyCopyable         <Contact>, "PersonName<> must be a trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<Contact>, "PersonName<> must be a standard layout trivially copyable type");

                static_assert(StandardLayout            <PhoneNumber>, "PhoneNumber<> must be a standard layout type");
                static_assert(TriviallyCopyable         <PhoneNumber>, "PhoneNumber<> must be a trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<PhoneNumber>, "PhoneNumber<> must be a standard layout trivially copyable type");

                static_assert(StandardLayout            <PersonName>, "PersonName<> must be a standard layout type");
                static_assert(TriviallyCopyable         <PersonName>, "PersonName<> must be a trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<PersonName>, "PersonName<> must be a standard layout trivially copyable type");

                static_assert(StandardLayout            <Date>, "Date must be a standard layout type");
                static_assert(TriviallyCopyable         <Date>, "Date must be a trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<Date>, "Date must be a standard layout trivially copyable type");


                static_assert(StdLayoutTriviallyCopyable<Email::LocalPart>, "Email<>::LocalPart must be a standard layout trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<Email::Domain   >, "Email<>::Domain must be a standard layout trivially copyable type");

                static_assert(StandardLayout            <Email>, "Email<> must be a standard layout type");
                static_assert(TriviallyCopyable         <Email>, "Email<> must be a trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<Email>, "Email<> must be a standard layout trivially copyable type");

                static_assert(StandardLayout<Person::Datatype>, "Person::DataType must be a standard layout type");
                static_assert(TriviallyCopyable<Person::Datatype>, "Person::DataType must be a trivially copyable type");
                static_assert(StdLayoutTriviallyCopyable<Person::Datatype>, "Person must be a standard layout trivially copyable type");
                static_assert(Identifiable<Person>, "Person must be a Identifiable type");
                static_assert(Hashable<Person>, "Person must be a Hashable type");

                static_assert(Persistable<Person>, "Person must be a Persistable type");

                PersonName name = {W("Mauricio"), EMPTY, W("Gomes")};
                //std::chrono::year year(1980);
                //std::chrono::month month(1);
                //std::chrono::day day(1);            
                Date dob(1980, 1, 1);
                Person::Datatype data = { name, dob };
                Person p(data);
                CHECK_EQ(Id, p.id(), 1, W("0"));

                PersonName expected = { W("Mauricio"), EMPTY, W("Gomes") };
                CHECK_EQ(PersonName, p.name(), expected, W("1"));

                p.add_email(W("local_part@domain"));
                Email expected_email = { W("local_part"), W("domain") };
                Email actual_email = p.data()->memails[0];
                CHECK_EQ(Email, actual_email, expected_email, W("4"));

                pd::ObjMemoryBuffer<Person> buffer;
                buffer.write(p);

                Person p2;
                CHECK_NOT_EQ(Person, p2, p, W("2"));
                /*

                buffer.read(&p2);
                CHECK_EQ(Person, p2, p, W("3"));

                Email expected_email2 = { W("local_part"), W("domain") };
                Email actual_email2 = p2.data()->memails[0];
                CHECK_EQ(Email,actual_email2, expected_email2, W("4"));
                */

                TEST_END(Person)
        }
    } // namespace cpplib
} // namespace pensar_digital