// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constant.hpp"
#include "string_util.hpp"

#include <iostream> // std::istream, std::ostream

namespace pensar_digital
{
    namespace cpplib
    {
        class Version
        {
            private:
                VersionInt mpublic;
                VersionInt mprotected;
                VersionInt mprivate;

                // Setters.
                void set_public    (VersionInt v) noexcept { mpublic    = v; }
                void set_protected (VersionInt v) noexcept { mprotected = v; }
                void set_private   (VersionInt v) noexcept { mprivate   = v; }

            public:
                static const VersionInt NULL_VERSION = -1;
                Version (VersionInt pub = NULL_VERSION, VersionInt prot = NULL_VERSION, VersionInt priv = NULL_VERSION) 
                          : mpublic(pub), mprotected(prot), mprivate(priv) {}

                // Getters.
                VersionInt get_public    () const noexcept { return mpublic   ; }
                VersionInt get_protected () const noexcept { return mprotected; }
                VersionInt get_private   () const noexcept { return mprivate  ; }

                String to_string() const noexcept
                {
                    return pd::to_string(mpublic) + "." + pd::to_string(mprotected) + "." + pd::to_string(mprivate);
                }

                bool operator == (const Version& o) const { return (mpublic == o.mpublic && mprotected == o.mprotected && mprivate == o.mprivate); }
                bool operator != (const Version& o) const { return !(*this == o); }

                // Implicit conversion to string.
                operator String() const noexcept { return to_string(); }

                friend std::istream& operator >> (std::istream& is, Version& v);
        };

        // Stream operators.
        extern std::istream& operator >> (std::istream& is, Version& v);
        extern std::ostream& operator << (std::ostream& os, const Version& v);

    } // namespace cpplib
} // namespace pensar_digital

#endif // VERSION_HPP_INCLUDED
