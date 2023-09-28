// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constant.hpp"
#include "string_util.hpp"
#include "header_lib/json.hpp"
#include "factory.hpp"

#include <iostream> // std::istream, std::ostream
#include <memory>   // std::shared_ptr

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
                inline static VersionFactory mfactory = { 3, 10, 1, 1, 1, NULL_ID };
                VersionInt mpublic;
                VersionInt mprotected;
                VersionInt mprivate;
        	    Id         mid;
                
                // Setters.
                void set_public    (VersionInt v) noexcept { mpublic    = v; }
                void set_protected (VersionInt v) noexcept { mprotected = v; }
                void set_private   (VersionInt v) noexcept { mprivate   = v; }

            public:
                inline static const VersionInt NULL_VERSION = -1;
                Version(const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& aid = NULL_ID)
                    : mpublic(pub), mprotected(prot), mprivate(priv), 
                      mid (aid) {}

                inline static VersionFactory::P get(const VersionInt& pub = pd::Version::NULL_VERSION,
                    const VersionInt& pro = Version::NULL_VERSION,
                    const VersionInt& pri = Version::NULL_VERSION,
                    const Id& aid = NULL_ID)
                {
                    return mfactory.get (pub, pro, pri, aid);
                };

                VersionFactory::P clone ()
                {
                    return get (mpublic, mprotected, mprivate, mid);
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

                inline static VersionFactory::P parse_json (const String& sjson)
                {
                    Json j;
                    VersionFactory::P ptr = get();
                    std::stringstream ss(sjson);
                    ss >> *ptr;
                    return ptr;
                };
        
                // Getters.
                VersionInt get_public    () const noexcept { return mpublic   ; }
                VersionInt get_protected () const noexcept { return mprotected; }
                VersionInt get_private   () const noexcept { return mprivate  ; }

                // Setters.
                inline void set_id (const Id& aid) noexcept { mid = aid; }

                bool equals (const Version& v) const noexcept;
	
                inline virtual bool initialize (const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& aid = NULL_ID) noexcept
                {
                    mpublic    = pub ;
				    mprotected = prot;
				    mprivate   = priv;
				    mid        = aid ;

                    return true;
			    }

                inline String to_string() const noexcept
                {
                    return pd::to_string (mpublic) + "." + pd::to_string (mprotected) + "." + pd::to_string (mprivate);
                }

                // Convertion to xml string.
                String xml() const noexcept;
                void from_xml(const String& sxml);
            
                std::istream& read (std::istream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native);

                inline bool operator == (const Version& v) const { return ((mid == v.mid) && (mpublic == v.mpublic) && (mprotected == v.mprotected) && (mprivate == v.mprivate)); }
                inline bool operator != (const Version& v) const { return !(*this == v); }

                // Implicit conversion to string.
                operator String() const noexcept { return to_string(); }

                friend std::istream& operator >> (std::istream& is, Version& v);

                // Inherited via Object
                String class_name() const;
                String debug_string() const noexcept;
                inline const Id get_id() const noexcept { return mid; }
                const Hash get_hash() const noexcept;
                String json() const noexcept;
                std::ostream& write (std::ostream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const;

            
                friend void from_json(const Json& j, Version& o);

        }; // class Version
        // Stream operators.
        extern std::istream& operator >> (std::istream& is, Version& v);
        extern std::ostream& operator << (std::ostream& os, const Version& v);
        extern void to_json(Json& j, const Version& o);
        extern void from_json(const Json& j, Version& o);

    } // namespace cpplib
} // namespace pensar_digital

#endif // VERSION_HPP_INCLUDED
