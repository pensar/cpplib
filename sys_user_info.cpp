// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "sys_user_info.hpp"

//---------------------------------------------------------------------------

#ifdef CODE_GEAR
#pragma package(smart_init)
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        #ifdef WIN32
        void get_user_name (std::string* s)
        {
            unsigned long max = 100;
            char buf[100];
            bool ok = GetUserName(buf, &max) != 0;
            if (!ok) throw "get_user_name: Não foi possível pegar o nome do usuário.";
            *s = buf;
        }
        #endif

        Path get_user_home ()
        {
            String home;

            #ifdef WINDOWS
            // Get home dir from environment variables
            char*  s;
            s = getenv("USERPROFILE");
            if (s)
            {
                if (s)
                    home = s;
            }
            else
            {
                s = getenv("HOMEPATH");
                char* home_drive = getenv ("HOMEDRIVE");
                if (home_drive)
                    home = home_drive;
                home += s;
            }
            #elif defined(UNIX)
            // Try get home dir from environment variable
            s = getenv("HOME");
            if (s)
                home = str;
            else
            {
                // Try to retrieve the home dir -- first use user's ID,
                //  and if failed, then use user's login name.
                if (!s_GetHomeByUID(home))
                {
                    s_GetHomeByLOGIN(home);
                }
            }
            #endif

         return home;
        }
    }   // namespace cpplib
}       // namespace pensar_digital
