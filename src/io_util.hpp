// $Id: io_util.h 70 2014-01-16 07:04:22Z mgpensar $

#ifndef io_utilH
#define io_utilH

#include <string>
#ifdef WINDOWS
#include <io.h>
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        using LINE_HANDLER = void(*)(const int64_t line_number, const std::string& line);
        extern uintmax_t read_file(const std::string& fname, LINE_HANDLER f);

        // Constantes usadas em is_same ().
        extern const int SAME_NAME;
        extern const int SAME_SIZE;
        extern const int SAME_TIME;
        extern const int SAME_ALL;

        #ifdef BORLAND
        extern bool operator == (const std::ftime& left, const std::ftime& right);
        extern bool operator != (const std::ftime& left, const std::ftime& right);
        extern bool operator <  (const std::ftime& left, const std::ftime& right);
        extern std::ftime fileTimestamp (const char* gridFilePath);
        extern std::string fileTimestampString (const char* gridFilePath);
        #endif

        //extern long long get_file_size(const char* sFileName);
        extern bool can_read (const char* file_name);

        #ifdef WIN32
        extern __int64 get_file_size (const std::string& file_name);
        #endif

        extern bool file_exists (std::string filename);
    }   // namespace cpplib
}       // namespace pensar_digital
#endif
