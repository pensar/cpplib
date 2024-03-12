#include "file.hpp"
#include "path.hpp"


namespace pensar_digital
{
    namespace cpplib
    { 
        InStream&  operator >> (InStream&  is,       File& file) { return file.read  (is); }
        OutStream& operator << (OutStream& os, const File& file) { return file.write (os); }
    }   // namespace cpplib
} // namespace pensar_digital
