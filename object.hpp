// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constant.hpp"
#include "s.hpp"
#include "header_lib/xmlParser.h"
#include "clone_util.hpp"
#include "version.hpp"
#include "json_util.hpp"
#include "factory.hpp"


#include <fstream>
#include <sstream>
#include <iostream>
#include <memory> // for std::shared_ptr
#include <concepts>
#include <exception>
#include <string>
#include <typeinfo> // for typeid
#include <string.h>
#include <utility> // for std::move
#include <vector>

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;
        class Object;
        typedef std::shared_ptr<Object> ObjectPtr;

        template <class T> T& assigns (T& l, const T& r) noexcept 
        { 
            std::memcpy  (l.data (), ((T&)r).data (), sizeof(T::DataType)); 
            return l; 
        }
        template <class T> T& moves (T& l, const T& r) noexcept
        { 
            std::memmove (l.data (), ((T&)r).data (), sizeof(T::DataType));
            return l; 
        }

        class Object
        {
            private:
                /// \brief Class Object::Data is compliant with the StdLayoutTriviallyCopyable concept. 
                /// \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable  
                /// \see https://en.cppreference.com/w/cpp/named_req/StandardLayoutType
                struct Data : public pd::Data
                {
                    Id mid;         //!< Unique id (among same class).
                    Data(const Id& id = NULL_ID) noexcept : 
                        mid(id) {}
                };
                Data mdata; //!< Member variable mdata contains the object data.
            public:
                inline const static Data NULL_DATA = { NULL_ID};
                typedef Data DataType;
                typedef pd::Factory<Object, typename Object::DataType> Factory;
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                typedef Factory FactoryType;

                virtual const pd::Data* data() const noexcept { return &mdata; }
                virtual size_t data_size() const noexcept { return sizeof(mdata); }
                /// Set id
                /// \param val New value to set
                void set_id(const Id& value) { mdata.mid = value; }
            private:
                inline static Factory mfactory = { 3, 10, NULL_DATA }; //!< Member variable "factory"
                
                // Set Factory as friend class to allow access to private members.
                friend class Factory;
            protected:
                S ObjXMLPrefix() const noexcept { return W("<object class_name = \"") + class_name() + W("\" id = \"") + Object::to_string() + W("\""); }

                /// \brief Compare objects.
                ///
                /// \param The object to be compared with.
                /// \return true if they are equal, false otherwise.
                /// \see equals
                ///
                inline virtual bool _equals(const Object& o) const { return ! (std::memcmp (data (), o.data (), data_size ())); }

            public:

                /// Default constructor.
                Object(const Data& data = NULL_DATA) noexcept
                {
                    initialize(data);
                }

                /// Copy constructor
                /// \param other Object to copy from
                Object(const Object& o) { assign(o); }

                /// Move constructor
                Object(Object&& o) noexcept { assign(o); }

                /** Default destructor */
                virtual ~Object() {}

                virtual Object& assign(const Object&  o) noexcept { std::memcpy  ((void*)data(), ((Object&)o).data(), data_size ()); return *this; }
                virtual Object& assign(const Object&& o) noexcept { std::memmove ((void*)data(), ((Object&)o).data(), data_size ()); return *this; }

                inline virtual void bytes (std::vector<std::byte>& v) const noexcept
                {
                    VERSION->bytes(v);
                    size_t req_size = v.size() + data_size();
                    if (v.capacity() < req_size)
						v.resize(req_size);
                    std::copy_n(reinterpret_cast<const std::byte*>(&mdata), data_size(), v.end() - data_size());
                }

                virtual std::span<const std::byte> bytes() const noexcept { return std::span<const std::byte>((const std::byte*)data (), data_size ()); }
                
                /// \brief Uses std::as_writable_bytes to get a span of writable bytes from the object.
                virtual std::span<std::byte> wbytes() noexcept
                {
                    static_assert (sizeof(char) == sizeof(std::byte));

                    auto byte_span = std::span<std::byte>((std::byte*)(data()), data_size());

                    return std::as_writable_bytes(byte_span);
				}
                virtual std::string sclass_name () const
				{
					std::string s = typeid(*this).name();
					s.erase(0, sizeof("class ") - 1);
					return s;
				}

                virtual S class_name() const
                {
                    std::string s = typeid(*this).name();
                    s.erase(0, sizeof("class ") - 1);
                    #ifdef WIDE_CHAR
					    return pd::to_wstring (s);
					#else
                        return s;
                    #endif  
                }

                // Clone method. 
                ObjectPtr clone() const noexcept { return pd::clone<Object>(*this, mdata.mid); }
                
                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                bool equals(const Object& o) const noexcept
                {
                    if (hash() != 
                        o.hash())
                        return false;
                        return _equals(o);
                }

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id id () const noexcept { return mdata.mid; };

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual inline const Hash hash() const noexcept { return this->id(); };

                // Implements initialize method from Initializable concept.
                virtual bool initialize (const Data& data) noexcept
                { 
                    mdata = data;
                    return true; 
                }

                // Conversion to json string.
                inline virtual S json () const noexcept 
                { 
                    return pd::json<Object> (*this) + W(" }");
                }

                virtual void read_bin_obj(InStream& is, const std::endian& byte_order = std::endian::native);

                void read_bin_version(InStream& is, const Version& version, const std::endian& byte_order = std::endian::native);

                virtual InStream& read (InStream& is, const IO_Mode amode = BINARY, const std::endian& byte_order = std::endian::native);

                virtual OutStream& write (OutStream& os, const IO_Mode amode = BINARY, const std::endian& byte_order = std::endian::native) const;

                // Conversion to xml string.
                virtual S xml() const noexcept { return ObjXMLPrefix() + W("/>"); }

                XMLNode parse_object_tag(const std::string& sxml)
                {
                    const char* xml = sxml.c_str();
                    XMLCSTR tag = "object";
                    XMLResults* pResults = 0;
                    XMLNode node = XMLNode::parseString(xml, tag, pResults);
                    std::string xml_class_name = node.getAttribute("class_name");
                    if (xml_class_name == sclass_name())
                    {
                        std::string sid = node.getAttribute("id");
                        mdata.mid = std::stoi(sid);
                    }
                    else
                        throw std::runtime_error("Invalid class name");
                    return node;
                }

                // Conversion from xml string.
                virtual void from_xml(const std::string& sxml)
                {
                    parse_object_tag(sxml);
                }

                bool operator == (const Object& o) const { return   equals(o); }
                bool operator != (const Object& o) const { return !equals(o); }

                Object& from_json(const S& sjson);

                /// Conversion to string.
                /// \return A string with the object id.
                S to_string() const noexcept { return pd::to_string(mdata.mid); }

                /// Implicit conversion to string.
                /// \return A string with the object id.
                operator S () const noexcept { return to_string(); }

                /// Debug string.
                /// \return A string with the object id.
                virtual S debug_string() const noexcept
                {
                    SStream ss;
                    ss << W("id = ") << Object::to_string();
                    return ss.str();

                }

                /// Assignment operator
                /// \param o Object to assign from
                /// \return A reference to this
                Object& operator=(const Object& o) noexcept { return assign(o); }

                /// Move assignment operator
                Object& operator=(Object&& o) noexcept { return assign(o); }

                friend void from_json(const Json& j, Object& o);
                
                static inline Factory::P  get(const Data& data = NULL_DATA)
                {
                    return mfactory.get(data);
                };

                static inline Factory::P  get(const Id& id)
				{
					return mfactory.get(Data (id));
				};

                Factory::P clone ()
                {
                    return get (mdata);
                };

                inline static Factory::P get (const Json& j)
                {
                    std::string json_class = j.at(W("class"));
                    if (json_class != pd::class_name<Object>())
                        throw std::runtime_error(W("Invalid class name: ") + pd::class_name<Object>());
                    Factory::P ptr = get(j.at(W("id")));

                    VersionPtr v = Version::get(j[W("VERSION")]);

                    if (*(ptr->VERSION) != *v)
                        throw std::runtime_error(W("Factory::parse_json: version mismatch."));

                    return ptr;
                }

                inline static Factory::P get (const S& sjson)
                {
                    Json j;
                    Factory::P ptr = get (Data (pd::id<Object>(sjson, &j)));

                    VersionPtr v = Version::get(j);

                    // todo: check version compatibility.
                    if (*(ptr->VERSION) != *v)
                        throw std::runtime_error("Factory::parse_json: version mismatch.");
                    return ptr;
                } // parse_json
        }; // Object
            extern void to_json(Json& j, const Object& o);
            extern void from_json(const Json& j, Object& o);
            //extern Object nlohmann::from_json (const Json& j);

            inline InStream& operator >> (InStream& is, Object& o)
            { 
				return o.read(is, TEXT);
            }
    
            inline OutStream& operator << (OutStream& os, const Object& o)
            { 
                return o.write (os, TEXT);
            }

            inline Object& operator >> (const S& sjson, Object& o) { return o.from_json(sjson); }

            inline InStream&  operator >> ( InStream& is,       ObjectPtr o) { return is >> *o; }
            inline OutStream& operator << (OutStream& os, const ObjectPtr o) { return os << *o; }

            static_assert (std::is_move_constructible_v <Object>);
            static_assert (std::is_move_assignable_v    <Object>);  

            // Dependency class is a Constrainable class used to define dependencies between objects.
            /*template <Versionable MainClass, Versionable RequiredClass>
            class Dependency
            {
            private:
                VersionInt required_public_interface_version;
                VersionInt required_protected_interface_version;
                VersionInt required_private_interface_version;
            public:
                Dependency(Version v) noexcept
                    : required_public_interface_version(v.get_public ()),
                    required_protected_interface_version(v.get_protected ()),
                    required_private_interface_version(v.get_private ()) {}
                virtual ~Dependency() {}
                virtual bool ok() const noexcept = 0;

                // method to set the class dependency. 
            };*/
         } // namespace cpplib
} // namespace pensar_digital
#endif // OBJECT_HPP
