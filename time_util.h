// $Id: time_util.h 55 2010-10-08 19:32:41Z mgpensar $

#ifndef time_utilH
#define time_utilH

#include <ctime>
#ifdef WINDOWS
#include <io.h>
#endif

#include "posix_util.h"

namespace pensar_digital
{
    namespace cpplib
    {
        #ifdef BORLAND
        extern std::time_t today ();
        extern ftime string2ftime (std::string date);
        #endif

        extern std::time_t hour2time_t      (const std::string& hora);
        extern std::time_t date2time_t      (const std::string& data);
        extern std::time_t strdate2time_t   (const std::string& data);
        extern std::time_t sdatetime2time_t (const std::string& datahora);

        extern std::string time_t2strdate (std::time_t time);
        extern std::string time_t2hour (std::time_t time);
        extern std::string time_t2hhmmss (std::time_t time);
        extern std::string time_t2date (std::time_t time);
        extern std::string stimestamp (std::time_t = 0);
        extern std::string now ();
        extern std::string today (const char* pattern = "%Y-%m-%d");

        extern const char* time_t2weekday (std::time_t t);
        extern POSIX::ALT_FILETIME strdate2ALT_FILE_TIME (const string& date);

    }   // namespace cpplib
}       // namespace pensar_digital
#endif
