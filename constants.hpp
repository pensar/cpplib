// $Id: constantes.h 40 2010-07-16 22:04:49Z mgpensar $

#ifndef constantesH
#define constantesH
//---------------------------------------------------------------------------
#include <cstdint>

namespace pensar_digital
{
    namespace cpplib
    {
        using Id                        = int64_t; // istream >> unsigned long was not compiling.
        using Hash                      = int64_t;
        using Version                   = int16_t;
        extern const Id NULL_ID;
    }   // namespace cpplib
}       // namespace pensar_digital

#endif
