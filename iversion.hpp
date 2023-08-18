// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef IVERSION_HPP_INCLUDED
#define IVERSION_HPP_INCLUDED

#include "constant.hpp"
#include "iobject.hpp"

#include <iostream> // std::istream, std::ostream
#include <memory>   // std::shared_ptr

namespace pensar_digital
{
    namespace cpplib
    {
        // Forward declarations.
        class IVersion;
        class IVersionRO;
        class VersionFactory;

        typedef std::shared_ptr <IVersionRO> IVersionROPtr;
        typedef std::shared_ptr <IVersion  > IVersionPtr  ;

        extern VersionFactory versionf;

		// Interface for read-only access to Version.
        class IVersionRO : public virtual IObjectRO
		{
            public:
                virtual VersionInt get_public    () const noexcept = 0;
                virtual VersionInt get_protected () const noexcept = 0;
                virtual VersionInt get_private   () const noexcept = 0;

        }; // class IVersionRO

        class IVersion : public virtual IVersionRO, public virtual IObject
        {
		    public:
                // Setters.
                virtual void set_public    (VersionInt v) noexcept = 0;
                virtual void set_protected (VersionInt v) noexcept = 0;
                virtual void set_private   (VersionInt v) noexcept = 0;
        }; // class Version
    } // namespace cpplib
} // namespace pensar_digital

#endif // IVERSION_HPP_INCLUDED
