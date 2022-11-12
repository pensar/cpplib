#ifndef NAME_HPP
#define NAME_HPP

#include "object.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        class RO_Name: public virtual RO_Object
        {
            public:
                /** Access name;
                 * \return The current value of name;
                 */
                std::string get_name () const { return name;}
                bool equals (const RO_Name& n) const {return name == n.name;}

                bool operator == (const RO_Name& n) const {return equals (n);}

            protected:
                RO_Name () {}

            private:
                std::string name;
        };

        class Name : public RO_Name, public virtual Object
        {
            public:
                /** Default constructor */
                Name(): RO_Name(), Object() {}

                /** Default destructor */
                virtual ~Name() {}

                /** Copy constructor
                 *  \param n Object to copy from
                 */
                Name(const Name& n) {assign (n);};

                using RO_Name::operator ==;

                Name& assign (const Name& n) {Object::assign(n); name = n.name; return *this;}

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

            private:
                std::string name; //!< Member variable "name"
        };
    }
}

#endif // NAME_HPP
