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
#include "s.hpp"
#include "io_util.hpp"
#include "generator.hpp"
#include "type_util.hpp" // pd::class_name 
#include "contact.hpp"   // Contact, ContactLocationType, ContactQualifier.
#include "phone.hpp"	 // PhoneNumber
#include "e-mail.hpp"	 // Email

namespace pensar_digital
{
    namespace cpplib
    {
        namespace contact
        {
            static inline const size_t MAX_FIRST_NAME = 20;
            static inline const size_t MAX_MIDDLE_NAME = 20;
            static inline const size_t MAX_LAST_NAME = 20;
            static inline const size_t MAX_NAME = MAX_FIRST_NAME + MAX_MIDDLE_NAME + MAX_LAST_NAME + 2;


            class PersonName
            {
            public:
                typedef CS<0, MAX_FIRST_NAME> First;
                typedef CS<0, MAX_MIDDLE_NAME> Middle;
                typedef CS<0, MAX_LAST_NAME> Last;

                First  mfirst;
                Middle mmiddle;
                Last   mlast;

                constexpr PersonName(const First& f = EMPTY, const Middle& m = EMPTY, const Last& l = EMPTY)
                    : mfirst(f), mmiddle(m), mlast(l) {}

                const CS<0, MAX_NAME> name() const
                {
                    S s = mfirst.to_string();
                    S s2 = mfirst.to_string() + SPACE + mlast.to_string();
                    S s3 = mlast.to_string();
                    s += mmiddle.empty() ? s3 : s2;
                    return s;
                }

                bool operator==(const PersonName& other) const
                {
                    return mfirst == other.mfirst && mmiddle == other.mmiddle && mlast == other.mlast;
                }
            };

            inline static const PersonName null_person_name()
            {
                return { W(""), W(""), W("") };
            }

            // Make PersonName OutputStreamable.
            inline OutStream& operator<<(OutStream& os, const PersonName& name)
            {
                os << name.name();
                return os;
            }

            // Person class. It must be trivially copyable. Contains fullname, date of birth, and a list of phone numbers and e-mails each qualified by a contact qualifier indicating if it is 
            // a home, work or other. Each phone number and e-mail address must be unique and also have a business or personal qualifier. Class must be serializable to a byte array 
            // and deserializable from a byte array.
            class Person : public pd::Object
            {
                public:
                    inline static const size_t MAX_PHONE_NUMBERS = 5;
                    inline static const size_t MAX_EMAILS = 5;
                private:
                    /// \brief Class Object::Data is compliant with the StdLayoutTriviallyCopyable concept. 
                    /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
                    /// \see https://en.cppreference.com/w/cpp/named_req/StandardLayoutType
                    struct Data : public pd::Data
                    {
                        PersonName mname;
                        pd::Date mdate_of_birth; // Format: YYYY-MM-DD
                        std::array<PhoneNumber, MAX_PHONE_NUMBERS> mphones;
                        size_t phones_count = 0;
                        std::array<Email, MAX_EMAILS> memails;
                        size_t emails_count = 0;

                        Data(const PersonName& name, pd::Date dob = pd::NULL_DATE) : mname(name), mdate_of_birth(dob) {}
                    };
                    Data mdata; //!< Member variable mdata contains the object data.
                    inline static pd::Generator<Person> generator; //!< Static generator object used to generate unique ids for Person objects.
                    inline static Factory mfactory = { 1000, 1000, NULL_DATA }; //!< Member variable "factory"

                    // Set Factory as friend class to allow access to private members.
                    friend class Factory;
                public:
                    inline const static Data NULL_DATA = { null_person_name(), pd::NULL_DATE };
                    typedef Data Datatype;
                    typedef pd::Factory<Person, typename Datatype, const pd::Id> Factory;
                    inline static const pd::VersionPtr VERSION = pd::Version::get(1, 1, 1);
                    typedef Factory FactoryType;

                    Data* data() { return &mdata; }
                    virtual const pd::Data* data() const noexcept { return &mdata; }
                    virtual size_t data_size() const noexcept { return sizeof(mdata); }

                    Person(const Data& data = NULL_DATA, const pd::Id id = pd::NULL_ID) : Object(id == pd::NULL_ID ? generator.get_id() : id), mdata(data)
                    {
                    }

                    void add_phone(const PhoneNumber& phone)
					{
						if (mdata.phones_count < MAX_PHONE_NUMBERS)
						{
							std::memcpy (&mdata.mphones[mdata.phones_count++], &phone, sizeof(phone));
						}
					}

                    void add_email(const Email& email)
                    {
                        if (mdata.emails_count < MAX_EMAILS)
                        {
                            std::memcpy(&mdata.memails[mdata.emails_count++], &email, sizeof(email));
                        }
                    }


                    virtual bool initialize(const Data& data = NULL_DATA, const pd::Id id = pd::NULL_ID)
                    {
                        bool ok = Object::initialize(id == pd::NULL_ID ? generator.get_id() : id);
                        std::memcpy(&mdata, &data, data_size());
                        return true;
                    }

                    const PersonName& name() const { return mdata.mname; }
                    virtual InStream& read (InStream& is, const std::endian& byte_order = std::endian::native)
                    {
                        Object::read(is, byte_order);
                        read_bin_version(is, *VERSION, byte_order);
                        is.read((C*)data(), data_size());
                        return is;
                    };

                    virtual OutStream& write (OutStream& os, const std::endian& byte_order = std::endian::native) const
                    {
                        Object::write(os, byte_order);
                        VERSION->write(os, byte_order);
                        os.write((const C*)data(), data_size());
                        return os;
                    };

                    /*static inline Factory::P  get(T aid = null_value<T>(), T initial_value = 0, T step = 1) noexcept
                    {
                        return mfactory.get(aid, initial_value, step);
                    };
                    Factory::P clone()
                    {
                        return get(get_id(), mdata.minitial_value, mdata.mstep);
                    };


                    */

                protected:
                    bool _equals(const Object& other) const noexcept override
                    {
                        const Person* pother = dynamic_cast<const Person*>(&other);
                        if (pother == nullptr)
                            return false;
                        return (std::memcmp(&mdata, &pother->mdata, sizeof(mdata)) == 0);
                    }
            }; // class Person  
        }       // namespace contact
    }           // namespace cpplib
}               // namespace pensar_digital

#endif // PERSON_HPP
