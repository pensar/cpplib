// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef NAME_HPP
#define NAME_HPP

#include "object.hpp"
#include "string_def.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        class Name : public Object
        {
            public:
                const static Version VERSION           = 1;
                const static Version INTERFACE_VERSION = 1;
                /** Access name;
                 * \return The current value of name;
                 */
                std::string get_name () const { return name;}
                bool equals (const Name& n) const {return name == n.name;}

                bool operator == (const Name& n) const {return equals (n);}

                /** Default constructor */
                Name(const Id aid = NULL_ID, const String& aname = ""): Object(aid), name (aname) {}

                /** Default destructor */
                virtual ~Name() {}

                /** Copy constructor
                 *  \param n Object to copy from
                 */
                Name (const Name&   n) {assign (n);};
                Name (const String& s) {name = s;}

                Name& assign (const Name& n)
                {
                    Object::assign(n);
                    name = n.name;
                    return *this;
                }

                Name& assign (const String& s) { Name n(s); Object::assign(n); name = s; return *this;}
                /** Assignment operator
                 *  \param n Object to assign from
                 *  \return A reference to this
                 */
                Name& operator=(const Name& n) { return assign (n); }

                /** Set name;
                 * \param val New value to set
                 */
                void set_name (const std::string& n) { name = n; }

            protected:
                virtual std::istream& ReadFromStream (std::istream& is)
                {
                    Object::ReadFromStream (is);
                    switch (get_public_interface_version ())
                    {
                        case 1:
                            is >> name;
                    };
                    return is;
                };

                virtual std::ostream& WriteToStream (std::ostream& os) const
                {
                    Object::WriteToStream (os);
                    switch (get_public_interface_version ())
                    {
                        case 1:
                            os << name;
                    };
                    return os;
                };
            public:
                std::istream& operator >> (std::istream& is)       { return ReadFromStream (is);};
                std::ostream& operator << (std::ostream& os) const { return WriteToStream  (os);};

            private:
                std::string name; //!< Member variable "name"
        };
    }
}

#endif // NAME_HPP
