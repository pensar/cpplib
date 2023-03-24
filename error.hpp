#ifndef ERROR_HPP_INCLUDED
#define ERROR_HPP_INCLUDED

#include "string_def.hpp"
#include "object.hpp"

#include <stdexcept>
#include <sstream>

namespace pensar_digital
{
    namespace cpplib
    {
        #define INVALID_ARGUMENT(condition,message) \
            if (condition)\
            {\
                std::stringstream ss;\
                ss << __FILE__ << '\t' << __LINE__ << '\t' << message << std::endl;\
                std::cerr << ss.str ();\
                std::cerr.flush ();\
                throw std::invalid_argument (ss.str ());\
            };

        class Error : public Object
        {
            public:
            Error (const String& error_msg, const Id aid = 0): Object (aid), error_message (error_msg) {}

            const String& get_error_message () const { return error_message; } ;

            void set_error_message    (const String& error_msg) { error_message = error_msg; };
            void append_error_message (const String& error_msg) { error_message = error_message + error_msg; };

            protected:
            virtual std::istream& ReadFromStream (std::istream& is)
            {
                Id id;
                is >> id >> error_message;
                set_id (id);
                return is;
            };

            virtual std::ostream& WriteToStream (std::ostream& os) const
            {
                os << error_message << get_id ();
                return os;
            };

            private:
            String error_message;
        };

        class UnsupportedVersion : public Error
        {
            public:
            UnsupportedVersion (const Version v): Error ("Unsupported version number: " + std::to_string(v)) {};
        };
    }
}

#endif // ERROR_HPP_INCLUDED
