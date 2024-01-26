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
            #define LINE_FEED W(C,"\r\n")
        #elif __linux__
            #include <unistd.h>
            #include <sys/utsname.h>
            #define MAX_PATH         4096 // Max path length in Linux
            #define MAX_UNC_PATH     4096 // Max UNC path length.
            #define PATH_SEPARATOR '/'
            #define LINE_FEED "\n"
        #endif

        template <typename C = char>
        class System 
        {
            public:
                typedef CS< 2, C> S2;
                typedef CS<10, C> S10;
                typedef CS<20, C> S20;

            static S10 os_name() 
            {
                #ifdef _WIN32   
                    return "Windows";
                #elif __linux__ 
                    return   "Linux";
                #else 
                    return "Other";
                #endif
            }

            static S10 os_version() 
            {
                #ifdef _WIN32
                    OSVERSIONINFO info;
                    ZeroMemory(&info, sizeof(OSVERSIONINFO));
                    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                    GetVersionEx(&info);
                    return std::to_string(info.dwMajorVersion) + "." + std::to_string(info.dwMinorVersion);
                #elif __linux__
                    struct utsname buffer;
                    uname(&buffer);
                    return buffer.release;
                #else
                return "Unknown";
                #endif
            }

            static inline S20 endianess () { return (std::endian::native == std::endian::little) ? "Little Endian" : "Big Endian"; }
            static inline S2 path_separator ()
            {
                #ifdef _WIN32
                return '\\';
                #else
                        return '/';
                #endif
            }
            inline const static S2 LF = LINE_FEED;
            
        };

    } // namespace cpplib
} // namespace pensar_digital
#endif	// SYSTEM_HPP