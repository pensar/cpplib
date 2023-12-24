// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

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
            
        #define WINVALID_ARGUMENT(condition,message) \
        if (condition)\
        {\
            std::wstringstream ss;\
            ss << WFILE << L'\t' << WLINE << L'\t' << message << std::endl;\
            std::wcerr << ss.str ();\
            std::wcerr.flush ();\
            throw std::invalid_argument (ss.str ());\
        };

        template <typename Char = char>
        class Error : public Object
        {
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                Error (const std::basic_string<Char>& error_msg, const Id aid = 0): Object (aid), error_message (error_msg) {}

            const std::basic_string<Char>& get_error_message () const { return error_message; } ;

            void set_error_message    (const std::basic_string<Char>& error_msg) { error_message = error_msg; };
            void append_error_message (const std::basic_string<Char>& error_msg) { error_message = error_message + error_msg; };

            virtual std::basic_istream<Char>& ReadFromStream (std::basic_istream<Char>& is)
            {
                Id id;
                is >> id >> error_message;
                set_id (id);
                return is;
            };

            virtual std::basic_ostream<Char>& WriteToStream (std::basic_ostream<Char>& os) const
            {
                os << error_message << id ();
                return os;
            };

            private:
                std::basic_string<Char> error_message;
        };

        extern std::ostream& operator<< (std::ostream& os, const Error<char>& e);
        extern std::istream& operator>> (std::istream& is,       Error<char>& e);

        extern std::wostream& operator<< (std::wostream& os, const Error<wchar_t>& e);
        extern std::wistream& operator>> (std::wistream& is,       Error<wchar_t>& e);


        template <typename Char = char>
        class UnsupportedVersion : public Error<Char>
        {
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                UnsupportedVersion (const Version v): Error<Char> (L(Char, "Unsupported version number: ") + v.to_string<Char> ()){};
        };
    }
}

#endif // ERROR_HPP_INCLUDED;
