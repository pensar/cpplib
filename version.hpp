// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constant.hpp"
#include "string_util.hpp"
#include "xml_util.hpp"
#include "iobject.hpp"
#include "json_util.hpp"

#include <iostream> // std::istream, std::ostream
#include <memory>   // std::shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
        class Version;
        
        typedef std::shared_ptr<Version> VersionPtr;
        
        class Version : public IObjectRO
        {
        private:
            VersionInt mpublic;
            VersionInt mprotected;
            VersionInt mprivate;
            Id         mid = 897896785686; // todo: get from generator.
        
            // Setters.
            void set_public(VersionInt v) noexcept { mpublic = v; }
            void set_protected(VersionInt v) noexcept { mprotected = v; }
            void set_private(VersionInt v) noexcept { mprivate = v; }

        public:
            static const VersionInt NULL_VERSION = -1;
            Version(VersionInt pub = NULL_VERSION, VersionInt prot = NULL_VERSION, VersionInt priv = NULL_VERSION)
                : mpublic(pub), mprotected(prot), mprivate(priv) {}

            // Getters.
            VersionInt get_public() const noexcept { return mpublic; }
            VersionInt get_protected() const noexcept { return mprotected; }
            VersionInt get_private() const noexcept { return mprivate; }

            String to_string() const noexcept
            {
                return pd::to_string(mpublic) + "." + pd::to_string(mprotected) + "." + pd::to_string(mprivate);
            }

            // Convertion to xml string.
            virtual String xml() const noexcept
            {
                IObjectRO& o = const_cast<Version&>(*this);
                String xml = pd::ObjXMLPrefix(o) + ">";
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual VersionPtr from_xml(const String& sxml)
            {
                Version* v = new Version(); // todo: get from factory.
                XMLNode node = pd::parse_object_tag<Version>(sxml, &mid);
                // todo: check version.
                XMLNode n = node.getChildNode("mprivate");
                if (!n.isEmpty())
                    v->mprivate = atoi(n.getText());
                n = node.getChildNode("mprotected");
                if (!n.isEmpty())
                    v->mprotected = atoi(n.getText());
                n = node.getChildNode("mpublic");
                if (!n.isEmpty())
                    v->mpublic = atoi(n.getText());

                return std::make_shared<Version>(*v); // todo: get from factory.
            }

            bool operator == (const Version& o) const { return (mpublic == o.mpublic && mprotected == o.mprotected && mprivate == o.mprivate); }
            bool operator != (const Version& o) const { return !(*this == o); }

            // Implicit conversion to string.
            operator String() const noexcept { return to_string(); }

            friend std::istream& operator >> (std::istream& is, Version& v);

            // Inherited via IObjectRO
            String class_name() const override;
            String debug_string() const noexcept override;
            bool equals(const IObjectRO& o) const noexcept override;
            const Id get_id() const noexcept override;
            const Hash get_hash() const noexcept override;
            String json() const noexcept override;
            std::ostream& write(std::ostream& os) const override;
            bool operator==(const IObjectRO& o) const override;
            bool operator!=(const IObjectRO& o) const override;

            virtual std::istream& read(std::istream& is) { return pd::read_json<Version>(is, *this); }
            friend void from_json(const Json& j, Version& o);

        }; // class Version
        // Stream operators.
        extern std::istream& operator >> (std::istream& is, Version& v);
        extern std::ostream& operator << (std::ostream& os, const Version& v);
        extern void to_json(Json& j, const Version& o);
        extern void from_json(const Json& j, Version& o);

        // Versionable concept requires a inline static const Version public member named VERSION.
        template <typename T>
        concept Versionable = requires (T t)
        {
            {t.VERSION} noexcept -> std::convertible_to<Version>;
        };

    } // namespace cpplib
} // namespace pensar_digital

#endif // VERSION_HPP_INCLUDED
