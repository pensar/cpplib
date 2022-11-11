// $Id: sys_user_info.h 70 2014-01-16 07:04:22Z mgpensar $

#ifndef sys_user_infoH
#define sys_user_infoH
//---------------------------------------------------------------------------

#include <string>

namespace pensar_digital
{
    namespace cpplib
    {
        void getUserName (std::string* s);

        /// Gets user home directory.
        std::string get_user_home ();
    }   // namespace cpplib
}       // namespace pensar_digital
#endif
