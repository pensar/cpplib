#ifndef STREAMABLE_HPP_INCLUDED
#define STREAMABLE_HPP_INCLUDED

#include <iostream>

namespace pensar_digital
{
    namespace cpplib
    {
        /// Child class has to implement ReadFromStream and WriteToStream methods.
        /// \tparam VersionType The type of the version info.
        class Streamable
        {
            protected:
            virtual std::istream& ReadFromStream (std::istream& is) = 0;
            virtual std::ostream& WriteToStream ( std::ostream& os) const = 0;

            public:
            std::istream& operator >> (std::istream& is)       { return ReadFromStream (is);};
            std::ostream& operator << (std::ostream& os) const { return WriteToStream  (os);};
        };
    }
}

#endif // STREAMABLE_HPP_INCLUDED
