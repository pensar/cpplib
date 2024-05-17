// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DUMMY_HPP
#define DUMMY_HPP

#include "../../unit-test/test.hpp"
#include "../cpplib/cpp/s.hpp"
#include "../cpplib/cpp/object.hpp"
#include "../cpplib/cpp/clone_util.hpp"
#include "../cpplib/cpp/factory.hpp"
#include "../cpplib/cpp/string_types.hpp"
#include "../cpplib/cpp/person.hpp"

#include <memory>

namespace pensar_digital
{
    namespace cpplib
    {
        using namespace pensar_digital::cpplib::contact;
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
                    CS<0, 20> mname;
                    Data(const S& name = W("")) noexcept : mname(name) {}
                    size_t data_size() const noexcept { return mname.length (); }  

                };
                Data mdata; //!< Member variable mdata contains the object data.
            public:
                    typedef pd::Factory<Dummy, Id, S> Factory;
                    inline static Factory factory = {3, 10, NULL_ID, EMPTY};
                    inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                    virtual const pd::Data* data() const noexcept { return &mdata; }
                    virtual size_t data_size() const noexcept { return mdata.data_size (); }

                    Dummy(const Id& id = NULL_ID, const S& name = EMPTY) : Object(id), mdata(name) {}
                    Dummy(const Dummy& d) : Object(d), mdata (d.mdata.mname) { }
                    Dummy(Dummy&& d) noexcept : Object(d) { assign (d); }
                
                    //Dummy& assign(const Dummy&  o) noexcept { Object::assign(o); std::memcpy(&mdata, &(o.mdata), sizeof(mdata)); }
                    //Dummy& assign(const Dummy&& o) noexcept { Object::assign(o); std::memmove(&mdata, &(o.mdata), sizeof(mdata)); }

                    inline static Factory::P get(const Id& aid = NULL_ID, const S& aname = EMPTY)
                    {
                        return factory.get(aid, aname);
                    }

                    inline Factory::P clone () const noexcept
                    {
                        return get(id  (), get_name ());
                    }

                    inline Factory::P clone(const DummyPtr& ptr) { return ptr->clone (); }
                    Dummy& operator = (const Dummy& d) noexcept { assign(d); return *this; }
                    Dummy& operator = (Dummy&& d) noexcept { assign(d); return *this; }
                    using Object::operator ==;// (const Dummy& d) const { return (Object::operator == (d) && (mdata.mname == d.mdata.mname)); }
                    using Object::operator !=;
                    virtual ~Dummy() {}
                    //Dummy& assign(const Dummy& d) noexcept { Object::assign(d); mdata = d.mdata; return *this; }

                    virtual S class_name() const noexcept { return Object::class_name (); }

                    virtual void set_id(const Id& aid) noexcept { Object::set_id(aid); }
            
                    // Implements initialize method from Initializable concept.
                    virtual bool initialize(const Id& aid = NULL_ID, const S& name = W("")) noexcept
                    {
                        Object::set_id(aid);
                        mdata.mname = name;
                        return true;
                    }

                    //DummyPtr clone() const  noexcept { return pd::clone<Dummy>(*this, id (), mdata.mname); }

                    virtual std::istream& binary_read(std::istream& is, const std::endian& byte_order = std::endian::native)
                    {
                        Object::binary_read(is, byte_order);
                        read_bin_version(is, *VERSION, byte_order);
                        is.read((char*)data(), data_size());
                        return is;
                    }

                virtual std::ostream& binary_write (std::ostream& os, const std::endian& byte_order = std::endian::native) const
                {
                        Object::binary_write (os, byte_order);
                        VERSION->binary_write(os, byte_order);
                        os.write((const char*)data(), data_size());
                    return os;
                }

                virtual S get_name() const noexcept { return mdata.mname; }
                void   set_name(const S& aname) noexcept { mdata.mname = aname; }
            
                virtual S to_string() const noexcept { return Object::to_string () + W(" ") + mdata.mname.to_string (); }

                /// Implicit conversion to string.
                /// \return A string with the object id.
                operator S () const noexcept { return to_string(); }

                /// Debug string.
                /// \return A string with the object id.
                virtual S debug_string() const noexcept
                {
				    return Object::debug_string() + W(" name = ") + mdata.mname;
                }

            //protected:
                //virtual bool _equals(const Object& o) const { return Object::_equals(o); }
        };

        inline InStream&  operator >> (InStream&  is,       Dummy& o) { return o.read  (is); }
        inline OutStream& operator << (OutStream& os, const Dummy& o) { return o.write (os); }

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // DUMMY_HPP