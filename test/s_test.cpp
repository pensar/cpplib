// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../s.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {

        TEST(S, true)
            static_assert(StdLayoutTriviallyCopyable<S<10>>, "S is not compliant with StdLayoutTriviallyCopyable concept.");
        TEST_END(S)
    }
}
