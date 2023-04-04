// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"

#include <iostream>
#include <memory>

namespace pensar_digital
{
    namespace cpplib
    {
        class Object
        {
            public:
                const static Version VERSION           = 1;
                const static Version INTERFACE_VERSION = 1;

                /// Check if passed object equals self.
                /// Derived classes must implement the _equals method. The hash compare logic is made on equals.
                /// _equals is called from template method equals and should only implement the specific comparison.
                /// \see _equals
                /// \return true if objects have the same id, false otherwise.
                const bool equals (const Object& o) const { return (get_hash () != o.get_hash () ? false : _equals (o)); }

                /// Access object id
                /// \return The current value of id
                ///
                virtual const Id get_id() const {return id;};

                /// \brief Access hash
                ///
                /// \return  The current value of hash
                virtual const Hash get_hash() const {return id;};

                bool operator == (const Object& o) const {return   equals (o);}
                bool operator != (const Object& o) const {return ! equals (o);}
            protected:
                /// \brief Compare objects.
                ///
                /// \param The object to be compared with.
                /// \return true if they are equal, false otherwise.
                /// \see equals
                ///
                virtual bool _equals (const Object& o) const { return (id == o.id);}
            private:

                Id id; //!< Member variable "id"
           public:
                /** Default constructor */
                Object (): id (NULL_ID) {};
                Object(Id aid): id (aid) {};

                /** Default destructor */
                virtual ~Object() {}

                /** Copy constructor
                 *  \param other Object to copy from
                 */
                Object(const Object& o) {assign (o);}

                virtual Object& assign (const Object& o) {id = o.get_id (); return *this;}

                template <typename T = Object>
                T& clone (const T& o) {return T(o);}

                /** Assignment operator
                 *  \param o Object to assign from
                 *  \return A reference to this
                 */
                Object& operator=(const Object& o) { return assign (o); }

                /** Set id
                 * \param val New value to set
                 */
                void set_id (Id value) { id = value; }

           protected:
                virtual std::istream& ReadFromStream (std::istream& is, const Version v)
                {
                    switch (v)
                    {
                        case 1:
                            return is >> id;
                    };
                    return is;
                };

                virtual std::ostream& WriteToStream (std::ostream& os, const Version v) const
                {
                    switch (v)
                    {
                        case 1:
                            return os << id;
                    };
                    return os;
                };
            public:
                std::istream& operator >> (std::istream& is)       { return ReadFromStream (is, VERSION);};
                std::ostream& operator << (std::ostream& os) const { return WriteToStream  (os, VERSION);};
         };
    }
}
#endif // OBJECT_HPP
