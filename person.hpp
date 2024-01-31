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
#include <cstring> // std::memcpy

#include "constant.hpp"
#include "bytes_util.hpp"
#include "concept.hpp"
#include "date.hpp"
#include "object.hpp"
#include "s.hpp"
#include "string_types.hpp"
#include "version.hpp"
#include "string_def.hpp"


namespace pensar_digital
{
	namespace contact
	{
		namespace pd = pensar_digital::cpplib;
        using S20 = pd::S20;
        using WS20 = pd::WS20;
        
        enum class LocationType { ltHome, ltWork, ltOther };
        enum class ContactQualifier { cqBusiness, cqPersonal, cqOther };

        struct Contact 
        {
            LocationType type;
            ContactQualifier qualifier;
        };

        static inline const size_t  MAX_FIRST_NAME = 20;
        static inline const size_t MAX_MIDDLE_NAME = 20;
        static inline const size_t   MAX_LAST_NAME = 20;
        static inline const size_t        MAX_NAME = MAX_FIRST_NAME + MAX_MIDDLE_NAME + MAX_LAST_NAME;
        
        
        /*
        template <typename C = char>
        struct FirstName : pd::CS<MAX_FIRST_NAME, C> 
        {
            FirstName(const pd::CS<MAX_FIRST_NAME, C>& first = pd::EMPTY<C>) : pd::CS<MAX_FIRST_NAME, C>(first) {}

            // conversion from std::basic_string
            FirstName(const std::basic_string<C>& s) : pd::CS<MAX_FIRST_NAME, C>(s) {}

            // Conversion from C string.
            FirstName(const C* s) : pd::CS<MAX_FIRST_NAME, C>(s) {}

            using pd::CS<MAX_FIRST_NAME, C>::operator=;
        };

        template <typename C = char>
        struct MiddleName : pd::CS<MAX_MIDDLE_NAME, C> 
        {
            MiddleName(const pd::CS<MAX_MIDDLE_NAME, C>& middle = pd::EMPTY<C>) : pd::CS<MAX_MIDDLE_NAME, C>(middle) {}

            // conversion from std::basic_string
            MiddleName(const std::basic_string<C>& s) : pd::CS<MAX_MIDDLE_NAME, C>(s) {}

            // Conversion from C string.
            MiddleName(const C* s) : pd::CS<MAX_MIDDLE_NAME, C>(s) {}
            using pd::CS<MAX_MIDDLE_NAME, C>::operator=;
        };

        template <typename C = char>
        struct LastName : pd::CS<MAX_LAST_NAME, C> 
        {
			LastName(const pd::CS<MAX_LAST_NAME, C>& last = pd::EMPTY<C>) : pd::CS<MAX_LAST_NAME, C>(last) {}

			// conversion from std::basic_string
			LastName(const std::basic_string<C>& s) : pd::CS<MAX_LAST_NAME, C>(s) {}
        
            // Conversion from C string.
            LastName(const C* s) : pd::CS<MAX_LAST_NAME, C>(s) {}
            using pd::CS<MAX_LAST_NAME, C>::operator=;
        };

    	template <typename C = char>
		struct Name : pd::CS<MAX_NAME_LENGTH, C> {};
        template <typename C = char>
        struct NullNameStruct
        {
			inline static const Name<C> value = { "NULL" };
		};  

        template<>
        struct NullNameStruct<wchar_t>
        {
            inline static const Name<wchar_t> value = { L"NULL" };
        };

        template <typename C = char>
        inline static const Name<C> null_name()
        {
            return NullNameStruct<C>::value;
        }


        template <typename C = char>
        struct EmptyNameStruct
        {
            inline static const Name<C> value = { "" };
        };

        template<>
        struct EmptyNameStruct<wchar_t>
        {
            inline static const Name<wchar_t> value = { L"" };
        };

        template <typename C = char>
        inline static const Name<C> empty_name()
        {
            return EmptyNameStruct<C>::value;
        }
*/
        template <typename C = char>
        class PersonName
        {
			public:
				pd::CS<MAX_FIRST_NAME , C> first;
                pd::CS<MAX_MIDDLE_NAME, C> middle;
                pd::CS<MAX_LAST_NAME , C>   last;

                PersonName (pd::CS<MAX_FIRST_NAME, C> f = pd::EMPTY<C>, pd::CS<MAX_MIDDLE_NAME, C> m = pd::EMPTY<C>, pd::CS<MAX_LAST_NAME, C> l = pd::EMPTY<C>) : first(f), middle(m), last(l) {}

                const pd::CS<MAX_NAME, C> name() const
				{
                    std::basic_string<C> s = first.to_string();
                    std::basic_string<C> s2 = middle.to_string() + pd::SPACE<C> + last.to_string();
                    std::basic_string<C> s3 = last.to_string();
                    s += middle.empty() ? s3 : s2;
                    return s;       
                }
               
				bool operator==(const PersonName<C>& other) const
				{
					return first == other.first && middle == other.middle && last == other.last;
				}
        };

        // Make PersonName OutputStreamable.
        inline std::ostream& operator<<(std::ostream& os, const PersonName<char>& name)
		{
			os << name.name(); 
			return os;
		}

        inline std::wostream& operator<<(std::wostream& os, const PersonName<wchar_t>& name)
        {
            os << name.name();
            return os;
        }

        // Person class. It must be trivially copyable. Contains fullname, date of birth, and a list of phone numbers and e-mails each qualified by a contact qualifier indicating if it is 
        // a home, work or other. Each phone number and e-mail address must be unique and also have a business or personal qualifier. Class must be serializable to a byte array 
        // and deserializable from a byte array.
        class Person : public pd::Object
        {
        private:
            /// \brief Class Object::Data is compliant with the StdLayoutTriviallyCopyable concept. 
            /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
            /// \see https://en.cppreference.com/w/cpp/named_req/StandardLayoutType
            struct Data : public pd::Data
            {
                PersonName<> mname;       
                pd::Date mdate_of_birth; // Format: YYYY-MM-DD
                Data (const PersonName<>& name, pd::Date dob): mname(name), mdate_of_birth(dob) {}   
                //std::unordered_set<Contact> phoneNumbers;
                //std::unordered_set<Contact> emails;

            };
            Data mdata; //!< Member variable mdata contains the object data.
        public:
            //inline const static Data NULL_DATA = { NULL_ID };
            typedef Data DataType;
            typedef pd::Factory<Person, typename DataType> Factory;
            inline static const pd::VersionPtr VERSION = pd::Version::get(1, 1, 1);
            typedef Factory FactoryType;

            virtual const pd::Data* data() const noexcept { return &mdata; }
            virtual size_t data_size() const noexcept { return sizeof(mdata); }

            Person (const Data& data) : mdata(data)
            {
            }

            const PersonName<>& name() const { return mdata.mname; }
        };

	} // namespace contact
} // namespace pensar_digital

#endif // PERSON_HPP
