#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <ro_object.hpp>
#include <iostream>

namespace pensar_digital
{
    namespace cpplib
    {
        class Object : public virtual RO_Object
        {
            public:
                /** Default constructor */
                Object(IdType aid = NULL_ID) : RO_Object() {};

                /** Default destructor */
                virtual ~Object() {}

                /** Copy constructor
                 *  \param other Object to copy from
                 */
                Object(const Object& o) {assign (o);}

                virtual Object& assign (const Object& o) {id = o.get_id (); return *this;}

                /** Assignment operator
                 *  \param o Object to assign from
                 *  \return A reference to this
                 */
                Object& operator=(const Object& o) { return assign (o); }

                /** Set id
                 * \param val New value to set
                 */
                void set_id (IdType value) { id = value; }

           protected:
                virtual std::istream& ReadFromStream (std::istream& is, const VersionType v)
                {
                    switch (v)
                    {
                        case 1:
                            return is >> id;
                    };
                    return is;
                };

                virtual std::ostream& WriteToStream (std::ostream& os, const VersionType v) const
                {
                    switch (v)
                    {
                        case 1:
                            return os << id;
                    };
                    return os;
                };
            public:
                std::istream& operator >> (std::istream& is)       { return ReadFromStream (is, get_version ());};
                std::ostream& operator << (std::ostream& os) const { return WriteToStream  (os, get_version ());};
         };
    }
}
#endif // OBJECT_HPP
