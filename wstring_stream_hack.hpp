#ifndef WSTRING_STREAM_HACK_HPP_INCLUDED
#define WSTRING_STREAM_HACK_HPP_INCLUDED

// If I do not define this compilation fails with wstring !
// Declared here and defined in GeneratorTest.cpp.
namespace std
{
    extern ostream& operator << (ostream& ostr, wstring const& str);
}


#endif // WSTRING_STREAM_HACK_HPP_INCLUDED
