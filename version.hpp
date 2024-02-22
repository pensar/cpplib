// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constant.hpp"
#include "s.hpp"
#include "header_lib/json.hpp"
#include "factory.hpp"
#include "type_util.hpp"

#include <iostream> // std::istream, std::ostream
#include <memory>   // std::shared_ptr
#include <algorithm> // std::min

namespace pensar_digital
{
    namespace cpplib
    {
        using Json = nlohmann::json; 
        class Version;

        typedef std::shared_ptr<Version> VersionPtr;
        
        // Versionable concept requires a inline static const Version public member named VERSION convertible to VersionPtr.
        template <typename T>
        concept Versionable = requires (T t)
        {
            {t.VERSION} noexcept -> std::convertible_to<VersionPtr>;
        };

        typedef Factory<Version, const VersionInt&,
            const VersionInt&,
            const VersionInt&,
            const Id&> VersionFactory;

        class Version   
        {
            private:
                inline static VersionFactory mfactory = { 3, 10, 1, 1, 1, null_value<Id>() };
                struct Data : public pd::Data
				{
					VersionInt mpublic;
					VersionInt mprotected;
					VersionInt mprivate;
					Id         mid;
                    Data (const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& aid = null_value<Id>())
						: mpublic(pub), mprotected(prot), mprivate(priv), mid(aid == null_value<Id>() ? 0 : aid) {}
				};
                Data mdata;

                // Setters.
                void set_public    (VersionInt v) noexcept { mdata.mpublic    = v; }
                void set_protected (VersionInt v) noexcept { mdata.mprotected = v; }
                void set_private   (VersionInt v) noexcept { mdata.mprivate   = v; }

            public:
                inline static const VersionInt NULL_VERSION = -1;
                
                typedef Data DataType;
                virtual const pd::Data* data() const noexcept { return &mdata; }
                virtual size_t data_size() const noexcept { return sizeof(mdata); }

                Version(const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& id = null_value<Id>())
                    : mdata (pub, prot, priv, id) {}

                inline static VersionFactory::P get(const VersionInt& pub = pd::Version::NULL_VERSION,
                    const VersionInt& pro = Version::NULL_VERSION,
                    const VersionInt& pri = Version::NULL_VERSION,
                    const Id& aid = null_value<Id>())
                {
                    return mfactory.get (pub, pro, pri, aid);
                };
                    
                inline virtual ~Version() noexcept = default;

                inline virtual void bytes(std::vector<std::byte> v) const noexcept
				{
                    size_t req_size = v.size() + data_size();
                    if (v.capacity() < req_size)
                        v.resize(req_size);
                    // Adds all bytes from mdata to v.
                    std::copy_n(reinterpret_cast<const std::byte*>(&mdata), data_size(), v.end() - data_size());
                }

                VersionFactory::P clone ()
                {
                    return get (mdata.mpublic, mdata.mprotected, mdata.mprivate, mdata.mid);
                };

                VersionFactory::P clone(const VersionPtr& ptr) { return ptr->clone (); }

                /// <summary>
                /// To be called when parsing a json object with an embedded VersionPtr object.
                /// </summary>
                /// <param name="j"></param>
                /// <returns></returns>
                inline static VersionFactory::P get (const Json& j)
                {
                    Id         vid        = j["VERSION"]["id"        ].get<VersionInt> ();
                    VersionInt vpublic    = j["VERSION"]["mpublic"   ].get<VersionInt> ();
                    VersionInt vprotected = j["VERSION"]["mprotected"].get<VersionInt> ();
                    VersionInt vprivate   = j["VERSION"]["mprivate"  ].get<VersionInt> ();

                    return get (vpublic, vprotected, vprivate, vid);
                };
                
                inline static VersionFactory& factory() noexcept { return mfactory; }   

                ///
                /// \return  The current value of hash
                virtual inline const Hash hash() const noexcept { return mdata.mid; };

                inline static VersionFactory::P parse_json (const S& sjson)
                {
                    Json j;
                    VersionFactory::P ptr = get();
                    SStream ss(sjson);
                    ss >> *ptr;
                    return ptr;
                };
        
                // Getters.
                VersionInt get_public    () const noexcept { return mdata.mpublic   ; }
                VersionInt get_protected () const noexcept { return mdata.mprotected; }
                VersionInt get_private   () const noexcept { return mdata.mprivate  ; }

                // Setters.
                inline void set_id (const Id& id) noexcept { mdata.mid = id; }

                bool equals (const Version& v) const noexcept;
	
                inline virtual bool initialize (const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& aid = null_value<Id>()) noexcept
                {
                    mdata.mpublic    = pub ;
                    mdata.mprotected = prot;
                    mdata.mprivate   = priv;
                    mdata.mid        = (aid == null_value<Id>() ? 0 : aid);

                    return true;
			    }

                S to_string () const noexcept
                {
                    return pd::to_string (mdata.mpublic) + W(".") + pd::to_string (mdata.mprotected) + W(".") + pd::to_string (mdata.mprivate);
                }

                // Convertion to xml string.
                S xml() const noexcept;
                void from_xml(const S& sxml);
            
                InStream& read (InStream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native);

                inline bool operator == (const Version& v) const { return ((mdata.mid == v.mdata.mid) && (mdata.mpublic == v.mdata.mpublic) && (mdata.mprotected == v.mdata.mprotected) && (mdata.mprivate == v.mdata.mprivate)); }
                inline bool operator != (const Version& v) const { return !(*this == v); }

                // Implicit conversion to string.
                operator S() const noexcept { return to_string (); }

                friend std::istream& operator >> (std::istream& is, Version& v);

                S class_name() const
                {
                    return pd::class_name<Version> ();
                }
                
                S debug_string() const noexcept;
                inline const Id id() const noexcept { return mdata.mid; }
                const Hash get_hash() const noexcept;
                S json() const noexcept;
                OutStream& write (OutStream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const;

            
                friend void from_json(const Json& j, Version& o);

        }; // class Version
        // Stream operators.
        extern InStream& operator >> (InStream & is, Version& v);
        extern OutStream& operator << (OutStream & os, const Version& v);
        extern void to_json(Json& j, const Version& o);
        extern void from_json(const Json& j, Version& o);

    } // namespace cpplib
} // namespace pensar_digital

#endif // VERSION_HPP_INCLUDED
