#ifndef TRANSCODER_HPP_INCLUDED
#define TRANSCODER_HPP_INCLUDED

#include <unicode/ucnv.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "s.hpp"


namespace pensar_digital 
{
    namespace cpplib 
    {
        class Encoding 
        {
            public:
                Encoding (const S& name) : name(name) {}

            operator S() const { return name; }

            const S& s() const noexcept { return name; }

            friend OutStream& operator<<(OutStream& os, const Encoding& encoding);

            static const Encoding UTF8;
            static const Encoding UTF16;
            static const Encoding ISO_8859_1;
            static const Encoding WINDOWS_1252;
            // Add more encodings as needed

        private:
            S name;
        };

        inline OutStream& operator<<(OutStream& os, const Encoding& encoding) {
            os << encoding.s ();
            return os;
        }


        const Encoding Encoding::UTF8         (W("UTF-8"       ));
        const Encoding Encoding::UTF16        (W("UTF-16"      ));
        const Encoding Encoding::ISO_8859_1   (W("ISO-8859-1"  ));
        const Encoding Encoding::WINDOWS_1252 (W("windows-1252"));
        
        #ifdef WIDE_CHAR
            const Encoding DefaultEncoding = Encoding::UTF16;
        #else
            const Encoding DefaultEncoding = Encoding::UTF8;
        #endif

        class Transcoder 
        {
            public:
                Transcoder(const Encoding& in, const Encoding& out)
                : min(in), mout(out) {}

            void transcode(InStream& in_stream, OutStream& out_stream) 
            {
                UErrorCode error = U_ZERO_ERROR;

                UConverter* in_converter = ucnv_open(min.s ().c_str (), &error);
                if (U_FAILURE(error)) {
                    std::cerr << "Failed to open input converter for " << min << std::endl;
                    return;
                }

                UConverter* out_converter = ucnv_open (mout.s ().c_str (), &error);
                if (U_FAILURE(error)) 
                {
                    std::cerr << "Failed to open output converter for " << mout << std::endl;
                    return;
                }

                S line;
                while (std::getline (in_stream, line)) 
                {
                    const C* source = line.c_str();
                    const C* sourceLimit = source + line.length ();
                    const size_t size = 1024;
                    UChar target[size];
                    UChar* targetStart = target;
                    ucnv_toUnicode (in_converter, &targetStart, &target[size - 1], &source, sourceLimit, NULL, true, &error);
                    if (U_FAILURE (error)) 
                    {
                        std::cerr << "Failed to convert line to Unicode" << std::endl;
                        return;
                    }

                    C output[1024];
                    C* out_ptr = output;
                    ucnv_fromUnicode(out_converter, &out_ptr, output + sizeof(output), (const UChar**)&target, targetStart, NULL, true, &error);
                    if (U_FAILURE(error)) 
                    {
                        std::cerr << "Failed to convert line from Unicode" << std::endl;
                        return;
                    }

                    out_stream << output << std::endl;
                }

                ucnv_close (in_converter);
                ucnv_close (out_converter);
            }

        private:
            Encoding min;
            Encoding mout;
        };

    }  // namespace cpplib
}  // namespace pensar_digital
#endif // TRANSCODER_HPP_INCLUDED