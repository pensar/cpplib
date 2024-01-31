#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "macros.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace pensar_digital
{
    namespace cpplib
    {
        // Comment the line below to exclude logging from the compiled file.
        #define LOG_ON
        #define CODEGEAR_BUG

        #ifdef LOG_ON
        
            // All this structure is just to create a default file name template<typename C = char>
            template <typename C = char>
            struct DefaultLogFileNameStruct
			{
                inline static const C* value = "log.txt";
            };

            template<>
            struct DefaultLogFileNameStruct<wchar_t>
            {
                inline static const wchar_t* value = L"log.txt";
            };

            template <typename C = char>
            inline static const C* default_log_file_name() 
            {
                return DefaultLogFileNameStruct<C>::value;
            }

            template <typename C = char>
            std::basic_fstream<C> _log(default_log_file_name<C>());

            static inline std::wfstream wlog(default_log_file_name<wchar_t>());

            static inline bool log_on = true;
            
            template <typename C = char>
            inline void enable_log  () {if (! _log<>.is_open ()) _log<>.open (default_log_file_name<C> ()); log_on = true;}
            template <typename C = char>
            inline void disable_log () {_log<>.close (); log_on = false;}

            template <>
            inline void enable_log() { if (!wlog.is_open()) wlog.open(default_log_file_name<wchar_t> ()); log_on = true; }
            template <>
            inline void disable_log() { wlog.close(); log_on = false; }


            // Comment the line below if you do not want the log to show where the message was originated.
            #define SHOW_LOCAL

            // The name was changed from log to cpplog to avoid conflict with math.h log function.
            #ifdef SHOW_LOCAL
                #define cpplog if (pensar_digital::cpplib::log_on) pensar_digital::cpplib::_log<> << FILE_LINE
            #else
                #define cpplog if (pensar_digital::cpplib::log_on) pensar_digital::cpplib::_log<>
            #endif
            #define LOG(msg) cpplog << msg << std::endl; pensar_digital::cpplib::_log<>.flush ();
            #define LOG_FLUSH pensar_digital::cpplib::_log<>.flush ();

            #ifdef SHOW_LOCAL
            #define wcpplog if (pensar_digital::cpplib::log_on) pensar_digital::cpplib::wlog << WFILE_LINE
            #else
            #define wcpplog if (pensar_digital::cpplib::log_on) pensar_digital::cpplib::wlog
            #endif
            #define WLOG(msg) wcpplog << msg << std::endl; pensar_digital::cpplib::wlog.flush ();
            #define WLOG_FLUSH pensar_digital::cpplib::wlog.flush ();
        #else
            #ifndef CODEGEAR_BUG
                #define cpplog \/\/
                #define LOG \/\/
                #define LOG_FLUSH \/\/
            #else
                #define cpplog if (0) std::cout
                #define LOG if (0)
                #define LOG_FLUSH ;
            #endif
        #endif
    }   // namespace cpplib
}       // namespace pensar_digital

#endif // LOG_H_INCLUDED

