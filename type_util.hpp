// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef TYPE_UTIL_HPP_INCLUDED
#define TYPE_UTIL_HPP_INCLUDED

#include "constant.hpp"
#include "string_util.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        template <class T>
        static String class_name() { String c = typeid(T).name(); c.erase(0, sizeof("class ") - 1); return c; }

    }
}

#endif // TYPE_UTIL_HPP_INCLUDED#pragma once
