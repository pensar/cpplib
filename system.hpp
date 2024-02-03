// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef  SYSTEM_HPP
#define SYSTEM_HPP

#include "s.hpp"

#include <iostream>
#include <type_traits>
#include <string>
#include <bit>  // std::endian

namespace pensar_digital
{
	namespace cpplib
	{
        #ifdef _WIN32 
            #include <windows.h>
            #define LINE_FEED "\r\n"
        #elif __linux__
            #include <unistd.h>
            #include <sys/utsname.h>
            #define MAX_PATH         4096 // Max path length in Linux
            #define MAX_UNC_PATH     4096 // Max UNC path length.
            #define PATH_SEPARATOR '/'
            #define LINE_FEED "\n"
        #endif

        class System 
        {
            public:

            static std::string os_name() 
            {
                #ifdef _WIN32   
                    return "Windows";
                #elif __linux__ 
                    return   "Linux";
                #else 
                    return "Other";
                #endif
            }

            static std::string os_version() 
            {
                #ifdef _WIN32
                    OSVERSIONINFO info;
                    ZeroMemory(&info, sizeof(OSVERSIONINFO));
                    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                    return std::to_string(info.dwMajorVersion) + "." + std::to_string(info.dwMinorVersion);

                #elif __linux__
                    struct utsname buffer;
                    uname(&buffer);
                    return buffer.release;
                #else
                return "Unknown";
                #endif
            }

            static inline std::string endianess () { return (std::endian::native == std::endian::little) ? "Little Endian" : "Big Endian"; }
            static inline char path_separator ()
            {
                #ifdef _WIN32
                return '\\';
                #else
                        return '/';
                #endif
            }
            inline const static std::string LF = LINE_FEED;
            
        };

    } // namespace cpplib
} // namespace pensar_digital
#endif	// SYSTEM_HPP