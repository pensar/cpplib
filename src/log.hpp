#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "macros.hpp"
#include "s.hpp"
#include "string_def.hpp"

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
        
            inline static const C* default_log_file_name() 
            {
                return W("log.txt");
            }


            static inline FStream log(default_log_file_name());

            static inline bool log_on = true;
            
            inline void enable_log  () {if (! log.is_open ()) log.open (default_log_file_name ()); log_on = true;}
            inline void disable_log () {log.close (); log_on = false;}

            // Comment the line below if you do not want the log to show where the message was originated.
            #define SHOW_LOCAL

            // The name was changed from log to cpplog to avoid conflict with math.h log function.
                #ifdef SHOW_LOCAL
                    #define cpplog if (pensar_digital::cpplib::log_on) pensar_digital::cpplib::log << __LINE__
                #else
                    #define cpplog if (pensar_digital::cpplib::log_on) pensar_digital::cpplib::log
                #endif
                #define LOG(msg) cpplog << msg << std::endl; pensar_digital::cpplib::log.flush ();
                #define LOG_FLUSH pensar_digital::cpplib::log.flush ();
        #else
            #ifndef CODEGEAR_BUG
                #define cpplog \/\/
                #define LOG \/\/
                #define LOG_FLUSH \/\/
            #else
                #define cpplog if (0) out ()
                #define LOG if (0)
                #define LOG_FLUSH ;
            #endif
        #endif
    }   // namespace cpplib
}       // namespace pensar_digital

#endif // LOG_H_INCLUDED

