#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

#include "object.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        template <class T, typename Id = int64_t>
        class IdGenerator
        {
            public:
                IdGenerator(): id(NULL_ID) {}
                virtual ~IdGenerator() {}

                Id get_id () { return ++id;}
            private:
                Id id;
        };
    }
}

#endif // IDGENERATOR_HPP
