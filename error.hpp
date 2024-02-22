// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef ERROR_HPP_INCLUDED
#define ERROR_HPP_INCLUDED

#include "s.hpp"
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
            SStream ss;\
            ss << __FILE__ << '\t' << __LINE__ << '\t' << message << std::endl;\
            std::cerr << ss.str ();\
            std::cerr.flush ();\
            throw std::invalid_argument (ss.str ());\
        };
            
        #define WINVALID_ARGUMENT(condition,message) \
        if (condition)\
        {\
            std::wstringstream ss;\
            ss << WFILE << L'\t' << WLINE << L'\t' << message << std::endl;\
            std::wcerr << ss.str ();\
            std::wcerr.flush ();\
            throw std::invalid_argument (ss.str ());\
        };

        class Error : public Object
        {
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                Error (const S& error_msg, const Id aid = 0): Object (aid), error_message (error_msg) {}

            const S& get_error_message () const { return error_message; } ;

            void set_error_message    (const S& error_msg) { error_message = error_msg; };
            void append_error_message (const S& error_msg) { error_message = error_message + error_msg; };

            virtual InStream& ReadFromStream (InStream& is)
            {
                Id id;
                is >> id >> error_message;
                set_id (id);
                return is;
            };

            virtual OutStream& WriteToStream (OutStream& os) const
            {
                os << error_message << id ();
                return os;
            };

            private:
                S error_message;
        };

        extern OutStream& operator << (OutStream& os, const Error& e);
        extern InStream&  operator >> (InStream&  is,       Error& e);

        class UnsupportedVersion : public Error
        {
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                UnsupportedVersion (const Version v): Error (W("Unsupported version number: ") + v.to_string ()){};
        };
    }
}

#endif // ERROR_HPP_INCLUDED;
