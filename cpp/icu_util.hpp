// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef ICU_UTIL_HPP
#define ICU_UTIL_HPP 

#include "defines.hpp"
#include "error.hpp"
#include "log.hpp"
#include "s.hpp"

#include <stdio.h>
#include <string.h>

#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <string>


namespace pensar_digital
{
    namespace cpplib
    {
        namespace icu
        {
            inline static const size_t BUFFER_SIZE = 1024;
            
            //Gets the max value of int32_t.
            #undef max // This is necessary because of a conflict with the max macro in Windows.h
            static inline constexpr size_t max_int32_t = std::numeric_limits<int32_t>::max();
            
            // Function to convert a UTF-8 string to a UChar string using ICU
            inline void convert_utf8_to_uchar(const char* src, UChar* dest, int32_t destCapacity) {
                UErrorCode status = U_ZERO_ERROR;
                // Calculate the required buffer size
                int32_t destLen = ucnv_toUChars(NULL, NULL, 0, src, -1, &status);
                status = U_ZERO_ERROR; // Reset status for actual conversion

                if (destLen <= destCapacity) {
                    // Perform the conversion
                    ucnv_toUChars(NULL, dest, destLen, src, -1, &status);

                    if (U_FAILURE(status)) 
                    {
                        S msg = W("convert_utf8_to_uchar: Failed to convert error message");
                        LOG(msg);
                        throw Error {msg};
                    }
                }
                else 
                {
                    S msg = W("convert_utf8_to_uchar: destination buffer is too small. Required size = ") + pd::to_string(destLen) + " but it was " + pd::to_string(destCapacity);
                    LOG(msg);
                    throw Error {msg};
                }
            }


            inline void char_ptr_to_uchar_ptr(const char* msg, UChar* out)
            {
                // Gets msg length and throws an error if it is too big.
                if (strlen(msg) > BUFFER_SIZE)
                {
                    S error = W("char_ptr_to_uchar_ptr: Message is too big to be converted to UChar string. Max. size = ") + pd::to_string(BUFFER_SIZE);
                    LOG(error);
                    throw Error {error};
                }
                convert_utf8_to_uchar (msg, out, BUFFER_SIZE); // Convert to UChar string
            }

            inline void utf8_char_ptr_to_wchar_t_ptr (const char* msg, wchar_t* out)
            {
                UChar uMsg[BUFFER_SIZE];
                convert_utf8_to_uchar (msg, uMsg, BUFFER_SIZE); // Convert to UChar string

                UErrorCode status = U_ZERO_ERROR;

                wchar_t wcharMsg[BUFFER_SIZE];
                u_strToWCS(wcharMsg, BUFFER_SIZE, NULL, uMsg, -1, &status);
                if ( !U_SUCCESS(status)) 
                {
                    S error = W("char_ptr_to_wchar_ptr: Error converting UChar to wchar_t");
                    LOG(error);
                    throw Error {error};
                }
            }

            inline std::wstring to_wstring(const std::string& s)
            {
                // Convert a UTF-8 string to a UTF-16 string using ICU library.
                UErrorCode status = U_ZERO_ERROR;
                UConverter* conv = ucnv_open("utf-8", &status);
                if (U_FAILURE(status)) {
					wprintf(L"Failed to open the converter\n");
				}
                // Calculate the required buffer size
				int32_t destLen = ucnv_toUChars(conv, NULL, 0, s.c_str(), -1, &status);
				status = U_ZERO_ERROR; // Reset status for actual conversion

				UChar* dest = new UChar[destLen];
				ucnv_toUChars(conv, dest, destLen, s.c_str(), -1, &status);

                if (U_FAILURE(status)) {
					wprintf(L"Failed to convert error message\n");
				}

				// Convert the UChar string to a wchar_t string
				wchar_t wcharMsg[BUFFER_SIZE];
				u_strToWCS(wcharMsg, BUFFER_SIZE, NULL, dest, -1, &status);
                if (!U_SUCCESS(status))
                {
 				}

				return std::wstring(wcharMsg);

            }

            // Converts from std::wstring to std::string.
            inline std::string utf16_to_utf8 (const std::wstring& s)
            {
                // Convert a UTF-16 string to a UTF-8 string using ICU library.
                UErrorCode status = U_ZERO_ERROR;
                UConverter* conv = ucnv_open("utf-8", &status);
                if (U_FAILURE(status))
                {
                    S error = W("pd::icu::utf16_to_utf8: Error getting converter.");
                    LOG(error);
                    throw Error{ error };
                }

                // Convert std::wstring to UChar*
                const UChar* source = reinterpret_cast<const UChar*>(s.c_str());
                
                size_t slen = s.length();
                if (slen > max_int32_t)
                {
                    S error = W("pd::icu::utf16_to_utf8: Error converting string. Buffer size is too big.");
                    LOG(error);
                    throw Error{ error };
                }
                uint32_t len = static_cast<uint32_t>(slen); 
                // Calculate the required buffer size for the UTF-8 string
                int32_t destLen = ucnv_fromUChars(conv, NULL, 0, source, len, &status);
                status = U_ZERO_ERROR; // Reset status for actual conversion

                // Allocate buffer for the UTF-8 string
                char* dest = new char[destLen + 1]; // +1 for null terminator

                // Perform the conversion
                ucnv_fromUChars(conv, dest, destLen + 1, source, len, &status);

                if (U_FAILURE(status))
                {
                    S error = W("pd::icu::utf16_to_utf8: Error converting string.");
                    LOG(error);
                    throw Error{ error };
                }

                std::string result(dest, destLen); // Construct the result string

                delete[] dest; // Clean up the allocated buffer
                ucnv_close(conv); // Close the converter

                return result;
            }

            #ifdef Windows
                // Converts from std::wstring to std::string.
                std::string to_string(const std::wstring& s)
                {
                    return utf16_to_utf8(s);
                }
            #endif

            inline std::string utf32_to_utf8(const std::wstring& s) 
            {
                UErrorCode status = U_ZERO_ERROR;

                // Open a converter for UTF-32 to UTF-8 conversion
                UConverter* convToUTF8 = ucnv_open("UTF-8", &status);
                if (U_FAILURE(status)) 
                {
                    S error = W("1. pd::icu::utf32_to_utf8: Failed to open UTF-8 converter.");
                    LOG(error);
                    throw Error{ error };
                }

                UConverter* convFromUTF32 = ucnv_open("UTF-32", &status);
                if (U_FAILURE(status)) 
                {
                    ucnv_close(convToUTF8); // Ensure the first converter is closed before throwing
                    S error = W("2. pd::icu::utf32_to_utf8: Failed to open UTF-32 converter.");
                    LOG(error);
                    throw Error{ error };
                }
                
                size_t slen = s.length();
                if (slen > max_int32_t)
                {
                    S error = W("pd::icu::utf32_to_utf8: Error converting string. Buffer size is too big.");
                    LOG(error);
                    throw Error{ error };
                }
                int32_t srcLength = static_cast<int32_t>(slen);

                // Convert std::wstring (UTF-32) to UChar*
                const UChar* source = reinterpret_cast<const UChar*>(s.c_str());

                // Calculate the required buffer size for the UTF-8 string
                int32_t utf8Length = ucnv_fromUChars(convToUTF8, NULL, 0, source, srcLength, &status);
                if (U_FAILURE(status)) 
                {
                    ucnv_close(convToUTF8);
                    ucnv_close(convFromUTF32);
                    S error = W("pd::icu::utf32_to_utf8: Failed to calculate buffer size for UTF-8 string.");
                    LOG(error);
                    throw Error{ error };
                }

                // Allocate buffer for the UTF-8 string
                char* utf8String = new char[utf8Length + 1]; // +1 for null terminator

                // Reset status for actual conversion
                status = U_ZERO_ERROR;

                // Perform the conversion
                ucnv_fromUChars(convToUTF8, utf8String, utf8Length + 1, source, srcLength, &status);
                if (U_FAILURE(status)) {
                    delete[] utf8String;
                    ucnv_close(convToUTF8);
                    ucnv_close(convFromUTF32);
                    S error = W("pd::icu::to_string: Failed to convert string from UTF-32 to UTF-8.");
                    LOG(error);
                    throw Error{ error };
                }

                std::string result(utf8String, utf8Length); // Construct the result string

                delete[] utf8String; // Clean up the allocated buffer
                ucnv_close(convToUTF8);
                ucnv_close(convFromUTF32);

                return result;
            }

        }
    }
}


#endif // ICU_UTIL_HPP