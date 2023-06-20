// $Id: sys_user_info.h 70 2014-01-16 07:04:22Z mgpensar $

#ifndef sys_user_infoH
#define sys_user_infoH
//---------------------------------------------------------------------------

#include "string_def.hpp"
#include "io_util.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        void get_user_name (std::string* s);

        /// Gets user home directory.
        Path get_user_home ();
    }   // namespace cpplib
}       // namespace pensar_digital
#endif
