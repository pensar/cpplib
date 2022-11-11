#ifndef VERSION_HPP_INCLUDED
#define VERSION_HPP_INCLUDED

#include "streamable.hpp"
#include "constants.h"
#include "error.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        /// Inherit from Version to have a read only version atributte.
        /// \tparam T The type used to hold the version attribute.

        class Version : public Streamable
        {
            public:
            typedef VersionType V;

            protected:
            ~Version() {}
            virtual std::istream& ReadFromStream (std::istream& is, const V v) = 0;
            virtual std::ostream& WriteToStream ( std::ostream& os, const V v) const = 0;

            Version (const V v = -1): version(v) {};

            /// Access version
            /// \return The version number.
            VersionType get_version () const { return version;}

            bool operator == (const Version& v) const {return (v.version == version);}
            bool operator != (const Version& v) const {return (v.version != version);}
            bool operator  < (const Version& v) const {return (v.version  < version);}
            bool operator  > (const Version& v) const {return (v.version  > version);}

            virtual std::istream& ReadFromStream (std::istream& is)
            {
                switch (version)
                {
                    case 0:
                        return is >> version;
                        break;
                    default:
                       throw UnsupportedVersion<> (version);
                };
                ReadFromStream (is, version);
                return is;
            };

            virtual std::ostream& WriteToStream ( std::ostream& os) const
            {
                switch (version)
                {
                    case 0:
                        return os << version;
                        break;
                    default:
                       throw UnsupportedVersion<> (version);
                };
                WriteToStream (os, version);
                return os;
            };

            using Streamable::operator <<;
            using Streamable::operator >>;

            private:
            V version; ///< Member variable "id"
        };
    }
}



#endif // VERSION_HPP_INCLUDED
