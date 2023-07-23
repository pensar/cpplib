// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef constantesHPP
#define constantesHPP
//---------------------------------------------------------------------------
#include <cstdint>
#include "string_def.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        using Id         = int64_t; // istream >> unsigned long was not compiling.
        using Hash       = int64_t;
        using VersionInt = int16_t;
        using FileSize   = int64_t;
        extern String DOUBLE_QUOTES;
        extern const Id NULL_ID;
    }   // namespace cpplib
}       // namespace pensar_digital

#endif
