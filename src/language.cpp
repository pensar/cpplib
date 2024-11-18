#include "pch.h"
#include "language.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        Language::Language(const Language& other)
        {
            //copy ctor
        }

        Language& Language::operator=(const Language& rhs)
        {
            if (this == &rhs) return *this; // handle self assignment
            //assignment operator
            return *this;
        }
    }
}