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
#include "json_util.hpp"
#include "io_util.hpp"
#include "generator.hpp"


namespace pensar_digital
{
	namespace contact
	{
		namespace pd = pensar_digital::cpplib;
        
        enum class LocationType { ltHome, ltWork, ltOther };
        enum class ContactQualifier { cqBusiness, cqPersonal, cqOther };

        struct Contact 
        {
            LocationType type;
            ContactQualifier qualifier;
        };

        static inline const size_t MAX_FIRST_NAME  = 20;
        static inline const size_t MAX_MIDDLE_NAME = 20;
        static inline const size_t MAX_LAST_NAME   = 20;
        static inline const size_t MAX_NAME        = MAX_FIRST_NAME + MAX_MIDDLE_NAME + MAX_LAST_NAME + 2;
        
        
        template <typename C = char>
        class PersonName
        {
			public:
                typedef pd::CS<MAX_FIRST_NAME , C> First;
                typedef pd::CS<MAX_MIDDLE_NAME, C> Middle;
                typedef pd::CS<MAX_LAST_NAME  , C> Last;

                First  mfirst ;
                Middle mmiddle;
                Last   mlast  ;
              
                PersonName (const First& f = pd::EMPTY<C>, const Middle& m = pd::EMPTY<C>, const Last& l = pd::EMPTY<C>) : mfirst(f), mmiddle(m), mlast(l) {}

                const pd::CS<MAX_NAME, C> name() const
				{
                    std::basic_string<C> s  = mfirst.to_string ();
                    std::basic_string<C> s2 = mfirst.to_string () + pd::SPACE<C> + mlast.to_string();
                    std::basic_string<C> s3 = mlast.to_string ();
                    s += mmiddle.empty() ? s3 : s2;
                    return s;       
                }
               
				bool operator==(const PersonName<C>& other) const
				{
					return mfirst == other.mfirst && mmiddle == other.mmiddle && mlast == other.mlast;
				}

                inline std::string json() const noexcept
                {
                    std::stringstream ss;
                    ss << pd::json<PersonName<C>>(*this);
                    ss << ", \"first\" : " << mfirst << ", \"middle\" : " <<  mmiddle << ", \"last\" : " << mlast << " }";
                    return ss.str();
                }

        };

        template <typename C = char>
        struct NullNameStruct
        {
            inline static const PersonName<C> value = { "", "", "" };
        };

        template<>
        struct NullNameStruct<wchar_t>
        {
            inline static const PersonName<wchar_t> value = {L"", L"", L""};
        };
        
        template <typename C = char>
        inline static const PersonName<C> null_person_name ()
        {
            return NullNameStruct<C>::value;
        }

        // Make PersonName OutputStreamable.
        inline std::ostream& operator<<(std::ostream& os, const PersonName<char>& name)
		{
			os << name.name (); 
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
                    Data (const PersonName<>& name, pd::Date dob = pd::NULL_DATE): mname(name), mdate_of_birth(dob) {}   
                    //std::unordered_set<Contact> phoneNumbers;
                    //std::unordered_set<Contact> emails;

                };
                Data mdata; //!< Member variable mdata contains the object data.
                inline static pd::Generator<Person> generator; //!< Static generator object used to generate unique ids for Person objects.
                inline static Factory mfactory = { 1000, 1000, NULL_DATA }; //!< Member variable "factory"

                // Set Factory as friend class to allow access to private members.
                friend class Factory;
        public:
            inline const static Data NULL_DATA = { null_person_name<>(), pd::NULL_DATE };
            typedef Data Datatype;
            typedef pd::Factory<Person, typename Datatype, const pd::Id> Factory;
            inline static const pd::VersionPtr VERSION = pd::Version::get(1, 1, 1);
            typedef Factory FactoryType;

            Data* data() { return &mdata; }
            virtual const pd::Data* data() const noexcept { return &mdata; }
            virtual size_t data_size() const noexcept { return sizeof(mdata); }

            Person (const Data& data = NULL_DATA, const pd::Id id = pd::NULL_ID) : Object (id == pd::NULL_ID ? generator.get_id () : id), mdata(data)
            {
            }

            virtual bool initialize (const Data& data = NULL_DATA, const pd::Id id = pd::NULL_ID)
            {
                bool ok = Object::initialize (id == pd::NULL_ID ? generator.get_id() : id);
                std::memcpy (&mdata, &data, data_size ());
				return true;
			}

            const PersonName<>& name() const { return mdata.mname; }
            inline virtual std::string json() const noexcept
            {
                std::stringstream ss;
                ss << pd::json<Person>(*this);
                //ss << ", \"minitial_value\" : " << mdata.minitial_value << ", \"mvalue\" : " << mdata.mvalue << ", \"mstep\" : " << mdata.mstep << " }";
                return ss.str();
            }

            virtual std::istream& read(std::istream& is, const pd::IO_Mode amode = pd::TEXT, const std::endian& byte_order = std::endian::native)
            {
                if (amode == pd::BINARY)
                {
                    Object::read(is, pd::BINARY, byte_order);
                    read_bin_version(is, *VERSION, byte_order);
                    is.read((char*)data(), data_size());
                }
                else // json format
                {
                    pd::Json j;
                    pd::Id id = pd::NULL_ID;
                    pd::VersionPtr v;
                    pd::read_json<Person>(is, *this, &id, &v, &j);
                    std::string s = j["first"];
                    mdata.mname.mfirst = s;
                    s = j["middle"];
                    mdata.mname.mmiddle = s;
					s = j["last"];
                    mdata.mname.mlast = s;
                }
                return is;
            };

            virtual std::wistream& read(std::wistream& is, const pd::IO_Mode amode = pd::TEXT, const std::endian& byte_order = std::endian::native)
            {
                return is;
            }

            virtual std::ostream& write(std::ostream& os, const pd::IO_Mode amode = pd::TEXT, const std::endian& byte_order = std::endian::native) const
            {
                if (amode == pd::BINARY)
                {
                    Object::write(os, amode, byte_order);
                    VERSION->write(os, amode, byte_order);
                    os.write((const char*)data(), data_size());
                }
                else // json format
                {
                    os << json();
                }
                return os;
            };

            virtual std::wostream& write(std::wostream& os, const pd::IO_Mode amode = pd::TEXT, const std::endian& byte_order = std::endian::native) const
            {
                return os;
            }

            // Convertion to xml string.
            virtual std::string xml() const noexcept
            {
                std::string xml = ObjXMLPrefix() + ">";
                //xml += VERSION->xml(); //todo.
                /*
                xml += "<initial_value>" + pd::to_string<Id>(mdata.minitial_value, '.') + "</initial_value>";
                xml += "<value>" + pd::to_string<Id>(mdata.mvalue, '.') + "</value>";
                xml += "<step>" + pd::to_string<Id>(mdata.mstep, '.') + "</step>";
                */
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual void from_xml(const std::string& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                // todo: check version.

                XMLNode n = node.getChildNode("initial_value");
                if (!n.isEmpty())
                    ;// mdata.minitial_value = atoi(n.getText());

                n = node.getChildNode("value");
                if (!n.isEmpty())
                    ;// mdata.mvalue = atoi(n.getText());

                n = node.getChildNode("step");
                if (!n.isEmpty())
                    ;// mdata.mstep = atoi(n.getText());
            }

            Person& parse_json(const std::string& s)
            {
                pd::Json j = pd::Json::parse(s);
                pd::from_json(j, *this);
                return *this;
            }

            /*static inline Factory::P  get(T aid = null_value<T>(), T initial_value = 0, T step = 1) noexcept
            {
                return mfactory.get(aid, initial_value, step);
            };
            Factory::P clone()
            {
                return get(get_id(), mdata.minitial_value, mdata.mstep);
            };

            inline static Factory::P get(const Json& j)
            {
                String json_class = j.at("class");
                if (json_class != pd::class_name<Generator<Type, T>>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<Generator<Type, T>>());

                typename Factory::P ptr = get(j.at("mid"), j.at("initial_value"), j.at("step"));

                VersionPtr v = Version::get(j["VERSION"]);

                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("Generator::Factory::parse_json: version mismatch.");

                return ptr;
            }

            inline static Factory::P get(const String& sjson)
            {
                Json j;
                T id = pd::id<Generator<Type, Id>>(sjson, &j);
                T initial_value = j.at("initial_value");
                T step = j.at("step");
                typename Factory::P ptr = get(id, initial_value, step);

                VersionPtr v = Version::get(j);

                // todo: check version compatibility.
                if (*(ptr->VERSION) != *v)
                    throw std::runtime_error("Factory::parse_json: version mismatch.");
                return ptr;
            } // parse_json
            */


            //friend void from_json(const Json& j, Person& p);

            protected:
            bool _equals(const Object& other) const noexcept override
            {
                const Person* pother = dynamic_cast<const Person*>(&other);
                if (pother == nullptr)
                    return false;
                return (std::memcmp (&mdata, &pother->mdata, sizeof(mdata)) == 0);
            }

            friend void to_json(pd::Json& j, const Person& p);
            friend void from_json(const pd::Json& j, Person& p);

        };
       
        inline void to_json(pd::Json& j, const Person& p)
        {
            j["class" ] = p.class_name();
            j["id"    ] = p.id();
            j["first" ] = p.mdata.mname.mfirst;
            j["middle"] = p.mdata.mname.mmiddle;
            j["last"  ] = p.mdata.mname.mlast;

            to_json(j, *(p.VERSION));
        }

        inline void from_json(const pd::Json& j, Person& p)
        {
            std::string class_name = p.class_name();
            std::string  json_class = j.at("class");
            if (class_name == json_class)
            {
                p.Object::set_id(j.at("id"));
                std::string s = j["mfirst"];
                p.mdata.mname.mfirst = s;
                s = j["mmiddle"];
				p.mdata.mname.mmiddle = s;
                s = j["mlast"];
				p.mdata.mname.mlast = s;


                pd::VersionPtr vp = pd::Version::get(j);
                if (*(p.VERSION) != *vp)
                    throw std::runtime_error("Person::from_json : version mismatch.");
            }
            else 
                throw new std::runtime_error("Person expected class = " + class_name + " but json has " + json_class);
        }
	} // namespace contact
} // namespace pensar_digital

#endif // PERSON_HPP
