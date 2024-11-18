// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DATA_HPP
#define DATA_HPP

#include "constant.hpp"
#include <cstddef> // std::byte

namespace pensar_digital
{
    namespace cpplib
    {
        struct Data {}; // Empty struct to be used as a base for all structs that are only data containers for objects.

        template <class D>
        struct Data2 : Data 
        {
            inline static const size_t size () const noexcept { return sizeof (D); }  

        }; // Struct to be used as a base for all structs that are only data containers for objects.
    }   // namespace cpplib
}       // namespace pensar_digital
#endif  // DATA_HPP

