// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constant.hpp"
#include "factory.hpp"
#include "type_util.hpp"
#include "memory_buffer.hpp"

#include <iostream> // std::istream, std::ostream
#include <memory>   // std::shared_ptr
#include <algorithm> // std::min
#include <bit>       // std::endian

namespace pensar_digital
{
    namespace cpplib
    {
        class Version   
        {
            public:
                using Int = int_fast16_t;
                using Ptr = std::shared_ptr<Version>;
            private:

                typedef Factory<Version, const Version::Int&,
                    const Version::Int&,
                    const Version::Int&,
                    const Id&> VersionFactory;
                inline static VersionFactory mfactory = { 3, 10, 1, 1, 1, null_value<Id>() };
                struct Data : public pd::Data
				{
                    Id  mid;
                    Int mpublic;
					Int mprotected;
					Int mprivate;
                    Data (const Int& pub = NULL_VERSION, const Int& prot = NULL_VERSION, const Int& priv = NULL_VERSION, const Id& aid = null_value<Id>())
						: mpublic(pub), mprotected(prot), mprivate(priv), mid(aid == null_value<Id>() ? 0 : aid) {}
				};
                Data mdata;

            public:
                inline static const Int NULL_VERSION = -1;
                
                typedef Data DataType;
                inline virtual const pd::Data* data() const noexcept { return &mdata; }
				inline static const size_t DATA_SIZE = sizeof(mdata);
                inline static const size_t SIZE = sizeof(mdata);

                inline const BytePtr version_data_bytes () const noexcept { return (BytePtr)&mdata; }
                inline virtual const BytePtr data_bytes() const noexcept { return (BytePtr)data(); }
                inline virtual size_t data_size () const noexcept { return sizeof(mdata); }
				inline virtual size_t size() const noexcept { return data_size(); }

                Version(const Int& pub = NULL_VERSION, const Int& prot = NULL_VERSION, const Int& priv = NULL_VERSION, const Id& id = null_value<Id>())
                    : mdata (pub, prot, priv, id) {}

				Version (MemoryBuffer& mb) noexcept
				{
                    assign (mb);
				}

                inline static VersionFactory::P get(const Int& pub = pd::Version::NULL_VERSION,
                    const Int& pro = Version::NULL_VERSION,
                    const Int& pri = Version::NULL_VERSION,
                    const Id& aid = null_value<Id>())
                {
                    return mfactory.get (pub, pro, pri, aid);
                };
                    
                inline virtual ~Version() noexcept = default;

				Version& assign (MemoryBuffer& mb) noexcept
				{
					mb.read_known_size (reinterpret_cast<BytePtr>(&mdata), DATA_SIZE);
					return *this;
				}
                
                inline virtual void write(MemoryBuffer& mb)
                {
                    mb.write((BytePtr)(&mdata), DATA_SIZE);
                }

                /*
                inline virtual void bytes(std::vector<std::byte> v) const noexcept
				{
                    size_t req_size = v.size() + data_size();
                    if (v.capacity() < req_size)
                        v.resize(req_size);
                    // Adds all bytes from mdata to v.
                    std::copy_n(reinterpret_cast<const std::byte*>(&mdata), data_size(), v.end() - data_size());
                }
                */

                inline virtual ByteSpan data_span() const noexcept { return ByteSpan (data_bytes(), data_size()); }

                inline virtual MemoryBuffer::Ptr bytes() const noexcept
                {
                    MemoryBuffer::Ptr mb = std::make_unique<MemoryBuffer>(DATA_SIZE);
					mb->write ((BytePtr)&mdata, DATA_SIZE);
					return mb;
                }
                inline VersionFactory::P clone () const noexcept
                {
                    return get (mdata.mpublic, mdata.mprotected, mdata.mprivate, mdata.mid);
                };

                inline VersionFactory::P clone(const Ptr& ptr) { return ptr->clone (); }
                
                inline static VersionFactory& factory() noexcept { return mfactory; }   

                ///
                /// \return  The current value of hash
                virtual inline const Hash hash() const noexcept { return mdata.mid; };

                // Getters.
                inline Int get_public    () const noexcept { return mdata.mpublic   ; }
                inline Int get_protected () const noexcept { return mdata.mprotected; }
                inline Int get_private   () const noexcept { return mdata.mprivate  ; }

                // Setters.
                inline void set_public(Int v) noexcept { mdata.mpublic = v; }
                inline void set_protected(Int v) noexcept { mdata.mprotected = v; }
                inline void set_private(Int v) noexcept { mdata.mprivate = v; }
                inline void set_id (const Id& id) noexcept { mdata.mid = id; }

                inline virtual bool initialize (const Int& pub = NULL_VERSION, const Int& prot = NULL_VERSION, const Int& priv = NULL_VERSION, const Id& aid = null_value<Id>()) noexcept
                {
                    mdata.mpublic    = pub ;
                    mdata.mprotected = prot;
                    mdata.mprivate   = priv;
                    mdata.mid        = (aid == null_value<Id>() ? 0 : aid);

                    return true;
			    }

                inline S to_s (Int number)const noexcept
                {
                    OutStringStream ss;
                    ss << number;
                    S s = ss.str();
                    //if (number < 0) inserts a minus sign at the beginning of the string.
                    if (number < 0)
                        s.insert(s.begin(), W('-'));

                    return s;
                }


                inline S to_string () const noexcept
                {
                    return to_s (mdata.mpublic) + W(".") + to_s (mdata.mprotected) + W(".") + to_s (mdata.mprivate);
                }

                inline bool operator == (const Version& v) const { return ((mdata.mid == v.mdata.mid) && (mdata.mpublic == v.mdata.mpublic) && (mdata.mprotected == v.mdata.mprotected) && (mdata.mprivate == v.mdata.mprivate)); }
                inline bool operator != (const Version& v) const { return !(*this == v); }

                // Implicit conversion to string.
                inline operator S() const noexcept { return to_string (); }

                inline friend std::istream& operator >> (std::istream& is, Version& v);

                inline S class_name() const
                {
                    return pd::class_name<Version> ();
                }
               
                inline const Id id() const noexcept { return mdata.mid; }

                inline S debug_string() const noexcept
                {
                    return S();
                }

                inline bool equals(const Version& v) const noexcept
                {
                    static_assert (Hashable<Version>);
                    return (v.id() == id()) && (v.mdata.mprivate == mdata.mprivate) && (v.mdata.mprotected == mdata.mprotected) && (v.mdata.mpublic == mdata.mpublic);
                }

                inline const Hash get_hash() const noexcept
                {
                    return Hash(897896785686); // todo: implement hash.
                }

                inline std::istream& binary_read(std::istream& is, const std::endian& byte_order)
                {
                    return is.read((char*)(&mdata), DATA_SIZE);
                }

                inline std::ostream& binary_write(std::ostream& os, const std::endian& byte_order) const
                {
                    return os.write((const char*)(&mdata), DATA_SIZE);
                }
        }; // class Version

        // Versionable concept requires a inline static const Version public member named VERSION convertible to Version::Ptr.
        template <typename T>
        concept Versionable = requires (T t)
        {
            { t.VERSION } noexcept -> std::convertible_to<Version::Ptr>;
        };

        // Concept IdentifiableAndVersionable requires T to be Versionable and Identifiable.
        template <typename T>
        concept IdentifiableAndVersionable = Versionable<T> && Identifiable<T>;
        inline InStream& operator >> (InStream& is, Version& v)
        {
            Version::Int pub, pro, pri;
            return is >> pub >> pro >> pri;
            v.set_public(pub);
            v.set_protected(pro);
            v.set_private(pri);

        }

        inline OutStream& operator << (OutStream& os, const Version& v)
        {
            return os << v.get_public() << W(".") << v.get_protected() << W(".") << v.get_private();
        }

    } // namespace cpplib
} // namespace pensar_digital

#endif // VERSION_HPP_INCLUDED
