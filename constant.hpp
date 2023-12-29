// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef constantesHPP
#define constantesHPP
//---------------------------------------------------------------------------
#include <cstdint>
#include "string_def.hpp"
#include <vector>  
#include <cstddef> // std::byte

namespace pensar_digital
{
    namespace cpplib
    {
        #define WIDE2(x) L##x
        #define WIDE1(x) WIDE2(x)
        #define WFILE WIDE1(__FILE__)
        #define WLINE WIDE1(__LINE__)

        using Id         = int_fast64_t; // istream >> unsigned long was not compiling.
        using Hash       = int_fast64_t;
        using VersionInt = int_fast16_t;
        using FileSize   = int_fast64_t;
        
        extern       S DOUBLE_QUOTES;
        extern const     Id NULL_ID;
        
        
        enum Alignment : int_fast16_t { NULL_ALIGNMENT = -1, BIT8 = 8, BIT16 = 16, BIT32 = 32, BIT64 = 64, BIT128 = 128, BIT256 = 256, BIT512 = 512, BIT1024 = 1024 };
        
        enum IO_Mode : bool { BINARY = true, TEXT = false};

        typedef std::vector<std::byte> Bytes;

    }   // namespace cpplib
}       // namespace pensar_digital






































namespace pd = pensar_digital::cpplib;

#endif
