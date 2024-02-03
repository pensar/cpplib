// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DUMMY_HPP
#define DUMMY_HPP

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "../json_util.hpp"
#include "../clone_util.hpp"
#include "../factory.hpp"
#include "../string_types.hpp"
#include "../person.hpp"

#include <memory>

namespace pensar_digital
{
    namespace cpplib
    {
        using namespace pensar_digital::contact;
        class Dummy;
#pragma warning( disable : 4250) // Disable warning C4250: inherits via dominance.
        typedef std::shared_ptr<Dummy> DummyPtr;


        /// <summary>
        /// Dummy class is streamable and comparable.
        /// </summary>
        class Dummy : public Object
        {
            private:
                struct Data : public pd::Data
                {
                    S<> mname;
                    Data(const S<>& name = "") noexcept : mname(name) {}
                    size_t data_size() const noexcept { return mname.length (); }  

                };
                Data mdata; //!< Member variable mdata contains the object data.
            public:
                    typedef pd::Factory<Dummy, Id, String> Factory;
                    inline static Factory factory = {3, 10, NULL_ID, ""};
                    inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                    virtual const pd::Data* data() const noexcept { return &mdata; }
                    virtual size_t data_size() const noexcept { return mdata.data_size (); }

                    Dummy(const Id& id = NULL_ID, const S<>& name = "") : Object(id), mdata(name) {}
                    Dummy(const Dummy& d) : Object(d), mdata (d.mdata.mname) { }
                    Dummy(Dummy&& d) noexcept : Object(d) { assign (d); }
                
                    //Dummy& assign(const Dummy&  o) noexcept { Object::assign(o); std::memcpy(&mdata, &(o.mdata), sizeof(mdata)); }
                    //Dummy& assign(const Dummy&& o) noexcept { Object::assign(o); std::memmove(&mdata, &(o.mdata), sizeof(mdata)); }

                    inline static Factory::P get(const Id& aid = NULL_ID, const String& aname = "")
                    {
                        return factory.get(aid, aname);
                    };

                    inline Factory::P clone ()
                    {
                        return get(id  (), get_name ());
                    };
                    inline Factory::P clone(const DummyPtr& ptr) { return ptr->clone (); }

                    inline static Factory::P parse_json(const String& sjson)
                    {
                        Json j;
                        Factory::P ptr = get(pd::id<Dummy>(sjson, &j));
                        ptr->set_name(j.at("name"));
                        VersionPtr v = Version::get(j);

                        // todo: check version compatibility.
                        if (*(ptr->VERSION) != *v)
                            throw std::runtime_error("Factory::parse_json: version mismatch.");

                        return ptr;
                    };

                    Dummy& operator = (const Dummy& d) noexcept { assign(d); return *this; }
                    Dummy& operator = (Dummy&& d) noexcept { assign(d); return *this; }
                    using Object::operator ==;// (const Dummy& d) const { return (Object::operator == (d) && (mdata.mname == d.mdata.mname)); }
                    using Object::operator !=;
                    virtual ~Dummy() {}
                    //Dummy& assign(const Dummy& d) noexcept { Object::assign(d); mdata = d.mdata; return *this; }

                    virtual String class_name() const noexcept { return Object::class_name (); }

                    virtual void set_id(const Id& aid) noexcept { Object::set_id(aid); }
            
                    // Implements initialize method from Initializable concept.
                    virtual bool initialize(const Id& aid = NULL_ID, const String& name = "") noexcept
                    {
                        Object::set_id(aid);
                        mdata.mname = name;
                        return true;
                    }

                    DummyPtr clone() const  noexcept { return pd::clone<Dummy>(*this, id (), mdata.mname); }

                    // Conversion to json string.
                    virtual String json() const noexcept
                    {
                        std::stringstream ss;
                        ss << (pd::json<Dummy>(*this));
                        ss << ", \"name\" : \"" << mdata.mname << "\" }";
                        return ss.str();
                    }

                    virtual std::istream& read (std::istream& is, const IO_Mode& amode = TEXT, const std::endian& byte_order = std::endian::native)
                    {
                        if (amode == BINARY)
                        {
                            Object::read(is, BINARY, byte_order);
                            read_bin_version(is, *VERSION, byte_order);
                            is.read((char*)data(), data_size());
                        }
                        else // json format
                        {
                            Json j;
                            Id id;
                            VersionPtr v;
                            pd::read_json<Dummy>(is, *this, &id, &v, &j);
                            set_id (id);
                            mdata.mname = j["name"].get<String>();
                        }
                        return is;
                };

                virtual std::ostream& write (std::ostream& os, const IO_Mode& amode = TEXT, const std::endian& byte_order = std::endian::native) const
                {
                    if (amode == BINARY)
                    {
                        Object::write (os, amode, byte_order);
                        VERSION->write(os, amode, byte_order);
                        os.write((const char*)data(), data_size());
                    }
                    else // json format
                    {
                        os << json ();
                    }
                    return os;
                };

                // Convertion to xml string.
                virtual String xml() const noexcept
                {
                    String xml = ObjXMLPrefix() + ">";
                    xml += "<name>" + mdata.mname.to_string () + "</name>";
                    xml += "</object>";
                    return xml;
                }

                // Convertion from xml string.
                virtual void from_xml(const String& sxml)
                {
                    XMLNode node = parse_object_tag(sxml);
                    XMLNode n = node.getChildNode("name");
                    if (!n.isEmpty()) mdata.mname = n.getText();
                }

                virtual String get_name() const noexcept { return mdata.mname; }
                void   set_name(const String& aname) noexcept { mdata.mname = aname; }
            
                virtual String to_string() const noexcept { return Object::to_string () + " " + mdata.mname.to_string (); }

                /// Implicit conversion to string.
                /// \return A string with the object id.
                operator String () const noexcept { return to_string(); }

                /// Debug string.
                /// \return A string with the object id.
                virtual String debug_string() const noexcept
                {
				    return Object::debug_string() + " name = " + mdata.mname;
                }

                //friend void to_json(Json& j, const Dummy& d);
                //friend void from_json(const Json& j, Dummy& d);
            //protected:
                //virtual bool _equals(const Object& o) const { return Object::_equals(o); }
        };
        //extern void to_json(Json& j, const Dummy& o);
        //extern void from_json(const Json& j, Dummy& o);

        extern std::istream& operator >> (std::istream& is, Dummy& o);
        extern std::ostream& operator << (std::ostream& os, const Dummy& o);

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // DUMMY_HPP