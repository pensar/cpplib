// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef constantesHPP
#define constantesHPP
//---------------------------------------------------------------------------
#include <cstdint>

namespace pensar_digital
{
    namespace cpplib
    {
        using Id                        = int64_t; // istream >> unsigned long was not compiling.
        using Hash                      = int64_t;
        using Version                   = int16_t;
        using FileSize                  = int64_t;
        extern const Id NULL_ID;
    }   // namespace cpplib
}       // namespace pensar_digital

#endif
