// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "constants.hpp"
#include "string_def.hpp"
#include <sstream>
#include <iostream>
#include <memory>

namespace pensar_digital
{
    namespace cpplib
    {
        

        // Concept for a class with a noexcept initialize method returning something convertible to bool.
        template <typename T, typename... Args>
        concept Initializable = requires (T t, Args&& ... args) 
        {
            {T(std::forward<Args>(args) ...)} noexcept;
            {t.initialize (std::forward<Args>(args) ...)} noexcept -> std::convertible_to<bool>;
        };

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
                Object () noexcept : id (NULL_ID) {};
                Object(Id aid) noexcept : id (aid) {};
                
                /// Move constructor
                Object(Object&& o) noexcept {assign (o);}

                /// Move assignment operator
                Object& operator=(Object&& o) noexcept {return assign (o);}

                /// Conversion to string.
                /// \return A string with the object id.
                virtual String to_string () const {return std::to_string (id);}

                /// Implicit conversion to string.
                /// \return A string with the object id.
                operator String () const {return to_string ();}

                /// Debug string.
                /// \return A string with the object id.
                virtual String debug_string() const 
                { 
                    std::stringstream ss;
                    ss << "id = " << to_string(); 
                    return ss.str ();

                }

                /** Default destructor */
                virtual ~Object() {}

                /** Copy constructor
                 *  \param other Object to copy from
                 */
                Object(const Object& o) {assign (o);}


                virtual Object& assign (const Object& o) {id = o.get_id (); return *this;}

                virtual Object clone(const Object& o) { return Object(o); }

                /** Assignment operator
                 *  \param o Object to assign from
                 *  \return A reference to this
                 */
                Object& operator=(const Object& o) { return assign (o); }

                /** Set id
                 * \param val New value to set
                 */
                void set_id (Id value) { id = value; }
                virtual bool initialize(Id aid = NULL_ID) noexcept { id = aid; return true; }
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

        static_assert(Initializable<Object, Id>);
    }
}
#endif // OBJECT_HPP
