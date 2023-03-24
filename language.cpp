#include "Language.hpp"

Language::Language()
{
    //ctor
}

Language::~Language()
{
    //dtor
}

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
