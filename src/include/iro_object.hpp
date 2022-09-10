#ifndef IROOBJECT_HPP
#define IROOBJECT_HPP

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


        };
    }
}

#endif // IROOBJECT_HPP
