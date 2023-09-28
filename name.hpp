// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef NAME_HPP
#define NAME_HPP

#include "object.hpp"
#include "string_def.hpp"
#include "json_util.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        class Name : public virtual Object
        {
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

                /** Access name;
                 * \return The current value of name;
                 */
                std::string get_name () const { return name;}
                bool equals (const Name& n) const {return name == n.name;}

                bool operator == (const Name& n) const {return equals (n);}

                /** Default constructor */
                Name (const Id aid = NULL_ID, const String& aname = ""): Object(aid), name (aname) {}

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

                // Conversion to json string.
                virtual String json() const noexcept
                {
                    std::stringstream ss (pd::json<Name>(*this));
                    ss << " , \"name\" : \"" << name << "\" }";
                    return ss.str();
                }

                virtual std::istream& read (std::istream& is, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native)
                {
                    if (amode == BINARY)
                    {
                        // todo: implement binary read.
                    }
                    else // json format
                    {
                        Json j;
                        Id id;
                        VersionPtr v;
                        pd::read_json<Name>(is, *this, &id, &v, &j);
                        set_id (id);
                        name = j["name"];

                        if (*VERSION != *v)
						{
							std::stringstream ss;
							ss << "Version mismatch: " << *VERSION << " != " << *v;
							throw std::runtime_error (ss.str());
						}
                    }
                    return is;
                };

                virtual std::ostream& write (std::ostream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const
                {
                    if (amode == BINARY)
                    {
                        // todo: implement binary read.
                    }
                    else // json format
                    {
                        os << json ();
                    }
                    return os;
                };

                //friend void to_json(Json& j, const Name& n);
                //friend void from_json(const Json& j, Name& n);
                friend std::istream& operator >> (std::istream& is, Name& n);
                friend std::ostream& operator << (std::ostream& os, const Name& n);

            private:
                std::string name; //!< Member variable "name"
        };
        extern std::istream& operator >> (std::istream& is,       Name& n);
        extern std::ostream& operator << (std::ostream& os, const Name& n);

        extern void to_json   (      Json& j, const Name& n);
        extern void from_json (const Json& j,       Name& n);
    }
}

#endif // NAME_HPP
