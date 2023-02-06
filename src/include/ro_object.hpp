#ifndef RO_OBJECT_HPP
#define RO_OBJECT_HPP

#include <cstdint>

namespace pensar_digital
{
    namespace cpplib
    {
        class RO_Object
        {
            public:
                using IdType                        = int64_t; // istream >> unsigned long was not compiling.
                using HashType                      = int64_t;
                using VersionType                   = int16_t;
                const static VersionType VERSION           = 1;
                const static VersionType INTERFACE_VERSION = 1;
                const static IdType      NULL_ID           = 0;

                const bool equals (const RO_Object& o) const {return id == o.id;}

                /** Access object id
                 * \return The current value of id
                 */
                virtual const IdType get_id() const {return id;};

                /** Access hash
                 * \return The current value of hash
                 */
                virtual const HashType get_hash() const {return id;};

                virtual const VersionType get_version           () const { return VERSION          ; };
                virtual const VersionType get_interface_version () const { return INTERFACE_VERSION; };

                bool operator == (const RO_Object& o) const {return   equals (o);}
                bool operator != (const RO_Object& o) const {return ! equals (o);}

            protected:
                RO_Object () {};

                IdType id; //!< Member variable "id"
        };
    }
}

#endif // RO_OBJECT_HPP
