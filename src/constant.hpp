// author : Mauricio Gomes  
// license: MIT (https://opensource.org/licenses/MIT)  
  
#ifndef constantesHPP  
#define constantesHPP  
  
// Defines.  
#define VISUAL_STUDIO  
  
//---------------------------------------------------------------------------  
#define _WINSOCKAPI_  
#include <cstdint>  
#include <vector>  
#include <cstddef> // std::byte  
#include <concepts> // std::signed_integral  
#include <iostream>     // std::cout  
#include <fstream>      // std::ifstream  
#include <string>       // std::string  
#include <sstream>      // std::stringstream  
#include <filesystem>   // std::filesystem  
#include <algorithm>    // std::find  
#include <functional>   // std::function  
#include <memory>       // std::shared_ptr  
#include <map>          // std::map  
#include <set>          // std::set  
#include <list>         // std::list  
#include <queue>        // std::queue  
#include <stack>        // std::stack  
#include <tuple>        // std::tuple  
#include <variant>      // std::variant  
#include <optional>     // std::optional  
#include <any>          // std::any  
#include <chrono>       // std::chrono  
#include <thread>       // std::thread  
#include <mutex>        // std::mutex  
#include <condition_variable> // std::condition_variable  
#include <atomic>       // std::atomic  
#include <future>       // std::future  
#include <type_traits>  // std::is_same  
#include <typeinfo>     // std::type_info  
#include <random>       // std::random_device  
#include <functional>   // std::function  
#include <cctype>       // std::tolower  
#include <span>         // std::span  
  
namespace pensar_digital  
{  
    namespace cpplib  
    {  

#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#endif

        struct Data  
        {  
        }; // Empty struct to be used as a base for all structs that are only data containers for objects.  
  
        using Id         = int_fast64_t; // istream >> unsigned long was not compiling.  
        using Hash       = int_fast64_t;  
        using VersionInt = int_fast16_t;  
        using FileSize   = int_fast64_t;  
          
        extern const Id NULL_ID;  
          
        template <typename T>  
        concept unsigned_integral = std::is_integral<T>::value && std::is_unsigned<T>::value;  
          
        template <unsigned_integral T>  
        constexpr T null_value() { return 0; }  
          
        template <std::signed_integral T>  
        constexpr T null_value() { return -1; }  
  
        enum Alignment : int_fast16_t { NULL_ALIGNMENT = -1, BIT8 = 8, BIT16 = 16, BIT32 = 32, BIT64 = 64, BIT128 = 128, BIT256 = 256, BIT512 = 512, BIT1024 = 1024 };  
          
        using Byte     = std::byte;  
        using Bytes    = std::vector<Byte>;  
        using BytePtr  = Byte*;  
        using ByteSpan = std::span<Byte>;  
  
    }   // namespace cpplib  
}       // namespace pensar_digital  
  
namespace pd = pensar_digital::cpplib;  
  
#endif
