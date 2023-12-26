// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef TYPE_UTIL_HPP_INCLUDED
#define TYPE_UTIL_HPP_INCLUDED

#include <typeinfo>
    
#include "constant.hpp"
#include "string_util.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        template <class T, typename C = char>
        std::basic_string<C> class_name() noexcept
        {
            std::string s = typeid(T).name();
            s.erase(0, sizeof("class ") - 1); // remove "class " prefix;
            return s;
        }
        
        // specialization for wchar_t
        template <class T>
        std::basic_string<wchar_t> class_name() noexcept
        {
            std::wstring s = typeid(T).name ();
            s.erase(0, sizeof(L"class ") - 1); // remove "class " prefix;
            return s;
        }
        //                return to_wstring(s);
    }   // namespace cpplib
}   // namespace pensar_digital

#endif // TYPE_UTIL_HPP_INCLUDED#pragma once
