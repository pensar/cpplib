// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <vector>
#include <utility>
#include <type_traits>
#include <array>
#include <unordered_set>

#include "constant.hpp"
#include "bytes_util.hpp"
#include "concept.hpp"
#include "date.hpp"

namespace pensar_digital
{
	namespace contact
	{
		namespace pd = pensar_digital::cpplib;

        enum class ContactType { Home, Work, Other };
        enum class ContactQualifier { Business, Personal };

        struct Contact 
        {
            std::array<char, 100> value; // Adjust size as needed
            ContactType type;
            ContactQualifier qualifier;
        };


        // Person class. It must be trivially copyable. Contains fullname, date of birth, and a list of phone numbers and e-mails each qualified by a contact qualifier indicating if it is 
        // a home, work or other. Each phone number and e-mail address must be unique and also have a business or personal qualifier. Class must be serializable to a byte array 
        // and deserializable from a byte array.
        class Person
        {
            public:
                std::array<char, 100> fullname; // Adjust size as needed
                pd::Date dateOfBirth; // Format: YYYY-MM-DD
                //std::unordered_set<Contact> phoneNumbers;
                //std::unordered_set<Contact> emails;

        };

        // Ensure the class is trivially copyable
        static_assert (std::is_trivially_copyable_v<std::array<char, 100>>);
        static_assert (std::is_standard_layout_v<std::array<char, 100>>);
        static_assert(pd::StdLayoutTriviallyCopyable<Person>);

	} // namespace contact
} // namespace pensar_digital

#endif // PERSON_HPP
