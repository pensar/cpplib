// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "io_util.hpp"
#include "path.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>


namespace pensar_digital
{
    namespace cpplib
    {
        InStream&  operator >> (InStream&  is,       Path& path) { return path.read  (is); }
        OutStream& operator << (OutStream& os, const Path& path) { return path.write (os); }
    }   // namespace cpplib
} // namespace pensar_digital