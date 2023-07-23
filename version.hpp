#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "constants.hpp"
#include "string_util.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        namespace pd = pensar_digital::cpplib;

        struct structVersion
        {
            VersionInt PUBLIC;
            VersionInt PROTECTED;
            VersionInt PRIVATE;

            structVersion(VersionInt pub, VersionInt prot, VersionInt priv) : PUBLIC(pub), PROTECTED(prot), PRIVATE(priv) {}

            String to_string() const noexcept
            {
                return pd::to_string<String, false>(PUBLIC) + "." + pd::to_string<String, false>(PROTECTED) + "." + pd::to_string<String, false>(PRIVATE);
            }

            bool operator == (const structVersion& o) const { return (PUBLIC == o.PUBLIC && PROTECTED == o.PROTECTED && PRIVATE == o.PRIVATE); }
            bool operator != (const structVersion& o) const { return !(*this == o); }

            // Implicit conversion to string.
            operator String() const noexcept { return to_string(); }
        };
    }
}

#endif // VERSION_HPP_INCLUDED
