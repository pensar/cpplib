#include "dummy.hpp"
#include "s.hpp" 

namespace pensar_digital
{
    namespace cpplib
    {
        /// Makes Dummy Streamable.
        OutStream& operator << (OutStream& os, const Dummy& d) { return d.write(os); }
        InStream& operator  >> ( InStream& is,       Dummy& d) { return d.read (is); }
    }
}