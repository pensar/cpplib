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
        struct Data {
        }; // Empty struct to be used as a base for all structs that are only data containers for objects.

        #define WFILE W(wchar_t,(__FILE__)) // WFILE is the same as __FILE__ but with an L prefix to indicate a wide string.
        #define WLINE W(wchar_t,(__LINE__)) // WLINE is the same as __LINE__ but with an L prefix to indicate a wide string.

        using Id         = int_fast64_t; // istream >> unsigned long was not compiling.
        using Hash       = int_fast64_t;
        using VersionInt = int_fast16_t;
        using FileSize   = int_fast64_t;
        
        extern       String  DOUBLE_QUOTES;
        extern const Id NULL_ID;
        
        template <std::unsigned_integral T>
        constexpr T null_value() { return 0; }
        
        template <std::signed_integral T>
        constexpr T null_value() { return -1; }

        enum Alignment : int_fast16_t { NULL_ALIGNMENT = -1, BIT8 = 8, BIT16 = 16, BIT32 = 32, BIT64 = 64, BIT128 = 128, BIT256 = 256, BIT512 = 512, BIT1024 = 1024 };
        
        enum IO_Mode : bool { BINARY = true, TEXT = false};

        typedef std::vector<std::byte> Bytes;

        typedef std::byte* BytePtr;

    }   // namespace cpplib
}       // namespace pensar_digital






































namespace pd = pensar_digital::cpplib;

#endif
