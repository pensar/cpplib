// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constant.hpp"
#include "string_util.hpp"
#include "xml_util.hpp"
#include "iobject.hpp"
#include "iversion.hpp"  // IVersion, IVersionRO
#include "json_util.hpp" // Json

#include <iostream> // std::istream, std::ostream
#include <memory>   // std::shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
        class Version;
        
        typedef std::shared_ptr<Version> VersionPtr;
        
        class Version : public IVersion  
        {
        private:
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
            typedef IVersion    I;  // Interface type.
            typedef IVersionRO IRO; // Read only interface type.
            Version(const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& aid = NULL_ID)
                : mpublic(pub), mprotected(prot), mprivate(priv), mid (aid) {}

            // Getters.
            VersionInt get_public() const noexcept { return mpublic; }
            VersionInt get_protected() const noexcept { return mprotected; }
            VersionInt get_private() const noexcept { return mprivate; }

            // Setters.
            inline void set_id (const Id& aid) noexcept override { mid = aid; }

            bool equals (const Version& v) const noexcept;
	
            inline virtual bool initialize(const VersionInt& pub = NULL_VERSION, const VersionInt& prot = NULL_VERSION, const VersionInt& priv = NULL_VERSION, const Id& aid = NULL_ID) noexcept
            {
                mpublic = pub;
				mprotected = prot;
				mprivate = priv;
				mid = aid;

                return true;
			}

            inline String to_string() const noexcept
            {
                return pd::to_string (mpublic) + "." + pd::to_string (mprotected) + "." + pd::to_string (mprivate);
            }

            // Convertion to xml string.
            String xml() const noexcept override;
            void from_xml(const String& sxml) override;
            
            std::istream& read(std::istream& is) override;

            inline bool operator == (const Version& v) const { return ((mid == v.mid) && (mpublic == v.mpublic) && (mprotected == v.mprotected) && (mprivate == v.mprivate)); }
            inline bool operator != (const Version& v) const { return !(*this == v); }

            // Implicit conversion to string.
            operator String() const noexcept { return to_string(); }

            friend std::istream& operator >> (std::istream& is, Version& v);

            // Inherited via IObjectRO
            String class_name() const override;
            String debug_string() const noexcept override;
            inline const Id get_id() const noexcept override { return mid; }
            const Hash get_hash() const noexcept override;
            String json() const noexcept override;
            std::ostream& write(std::ostream& os) const override;
            bool operator == (const IObjectRO& o) const override { return (mid == o.get_id()); }
            bool operator != (const IObjectRO& o) const override { return (mid != o.get_id()); }

            
            friend void from_json(const Json& j, Version& o);

        }; // class Version
        // Stream operators.
        extern std::istream& operator >> (std::istream& is, Version& v);
        extern std::ostream& operator << (std::ostream& os, const Version& v);
        extern void to_json(Json& j, const Version& o);
        extern void from_json(const Json& j, Version& o);

        // Versionable concept requires a inline static const Version public member named VERSION->
        template <typename T>
        concept Versionable = requires (T t)
        {
            {t.VERSION} noexcept -> std::convertible_to<IVersionPtr>;
        };

    } // namespace cpplib
} // namespace pensar_digital

#endif // VERSION_HPP_INCLUDED
