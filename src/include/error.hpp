#ifndef ERROR_HPP_INCLUDED
#define ERROR_HPP_INCLUDED

#include "streamable.hpp"
#include "string_def.hpp"
#include "constants.h"

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

        template <class String = string, typename IdType = uint32_t>
        class Error : public Streamable
        {
            public:
            typedef IdType IdT;
            Error (const String& error_msg, const unsigned aid = 0): error_message (error_msg), id (aid) {}

            const String& get_error_message () const { return error_message; } ;
            const IdType get_id () const { return id; };

            void set_error_message    (const String& error_msg) { error_message = error_msg; };
            void append_error_message (const String& error_msg) { error_message = error_message + error_msg; };

            protected:
            virtual std::istream& ReadFromStream (std::istream& is)
            {
                is >> id >> error_message;
                return is;
            };
            virtual std::ostream& WriteToStream (std::ostream& os) const
            {
                os << error_message << id;
                return os;
            };

            private:
            String error_message;
            IdType id;
        };

        template <class String = string, typename IdType = uint32_t>
        class UnsupportedVersion : public Error<String, IdType>
        {
            public:
            UnsupportedVersion (const VersionType v): Error<String, IdType> ("Unsupported version number: " + std::to_string(v)) {};
        };
    }
}

#endif // ERROR_HPP_INCLUDED
