#ifndef IROOBJECT_HPP
#define IROOBJECT_HPP

#include <cstdint>

namespace pensar_digital
{
    namespace cpplib
    {
        class IROObject
        {
            public:
                const int64_t NULL_ID = 0;

                /** Access object id
                 * \return The current value of id
                 */
                int64_t get_id() const = 0;

                /** Access hash
                 * \return The current value of hash
                 */
                int64_t get_hash() const = 0;

                unsigned int16_t get_version           () const = 0;
                unsigned int16_t get_interface_version () const = 0;
                const VersionType get_version           () const { return VERSION          ; };
                const VersionType get_interface_version () const { return INTERFACE_VERSION; };

                bool operator == (const IROObject<T>& o) const {return (o.id == id);}
                bool operator != (const IROObject<T>& o) const {return (o.id != id);}
                std::istream& operator >> (std::istream& is)       { return ReadFromStream (is);};


        };
    }
}

#endif // IROOBJECT_HPP
