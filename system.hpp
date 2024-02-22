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
            #define LINE_FEED W("\r\n")
        #elif __linux__
            #include <unistd.h>
            #include <sys/utsname.h>
            #define MAX_PATH         4096 // Max path length in Linux
            #define MAX_UNC_PATH     4096 // Max UNC path length.
            #define PATH_SEPARATOR W('/')
            #define LINE_FEED W("\n")
        #endif

        class System 
        {
            public:

            static S os_name() 
            {
                #ifdef _WIN32   
                    return W("Windows");
                #elif __linux__ 
                    return   W("Linux");
                #else 
                    return W(Other);
                #endif
            }

            static S os_version() 
            {
                #ifdef _WIN32
                    OSVERSIONINFO info;
                    ZeroMemory(&info, sizeof(OSVERSIONINFO));
                    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                    std::string s = to_string(info.dwMajorVersion) + "." + to_string(info.dwMinorVersion);
                    S r(s.c_str ());
                return r;

                #elif __linux__
                    struct utsname buffer;
                    uname(&buffer);
                    return buffer.release;
                #else
                return W("Unknown");
                #endif
            }

            static inline S endianess () { return (std::endian::native == std::endian::little) ? W("Little Endian") : W("Big Endian"); }
            static inline C path_separator ()
            {
                #ifdef _WIN32
                return W('\\');
                #else
                        return W('/');
                #endif
            }
            inline const static S LF = LINE_FEED;
            
        };

    } // namespace cpplib
} // namespace pensar_digital
#endif	// SYSTEM_HPP