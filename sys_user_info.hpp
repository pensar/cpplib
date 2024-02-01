// $Id: sys_user_info.h 70 2014-01-16 07:04:22Z mgpensar $

#ifndef sys_user_infoH
#define sys_user_infoH
//---------------------------------------------------------------------------

#include "string_def.hpp"
#include "io_util.hpp"
#include "path.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        #ifdef _WIN32   
        template <bool use_exceptions = true>
        Path get_user_home_windows()
        {
            String home = "";
            char* s;
            size_t len;
            errno_t err = _dupenv_s(&s, &len, "USERPROFILE");
            if (err != 0)
            {
                err = _dupenv_s(&s, &len, "HOMEDRIVE");
                if (err != 0)
                {
                    err = _dupenv_s(&s, &len, "HOMESHARE");
                }
                else
                {
                    home = s;
                    free(s);
                    err = _dupenv_s(&s, &len, "HOMEPATH");
                    home = (err ? (home + s) : "");
                }
            }
            home = (err ? "" : s);
            free(s);

            if (err and use_exceptions)
            {
                throw std::runtime_error("get_user_home_windows: Failed to get user home dir.");
            }
            return home;
        }
        #endif

        #if defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
        template <bool use_exceptions = true>
        Path get_user_home_unix()
        {
            S home = "";
            // Get home dir from environment variables
            char* s;
            size_t len;
            errno_t err = _dupenv_s(&s, &len, "USERPROFILE");
            if (!err)
            {
                home = s;
                free(s);
            }
            else
            {
                // Try to retrieve the home dir -- first use user's ID,
                //  and if failed, then use user's login name.
                if (!s_GetHomeByUID(home))
                {
                    s_GetHomeByLOGIN(home);
                }
            }
            if (err and use_exceptions)
            {
                throw std::runtime_error("get_user_home_unix: Failed to get user home dir.");
            }
        }
        #endif  

        template <bool use_exceptions = true>
        Path get_user_home()
        {
            String home;

            #ifdef _WIN32
                        return get_user_home_windows<use_exceptions>();
            #elif defined(__unix__ || __APPLE__ || __linux__)
                        return get_user_home_unix<use_exceptions>();
            #else      
                        if (use_exceptions)
                            throw std::runtime_error("get_user_home: Não foi possível pegar o diretório home do usuário pois o sistema operacional não foi identificado.");
                        else
                            return "";
            #endif
        }
    }   // namespace cpplib
}       // namespace pensar_digital
#endif
