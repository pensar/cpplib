// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include <memory>

namespace pensar_digital
{
    namespace pd = pensar_digital::cpplib;
    namespace unit_test
    {
        TEST(ObjectClone)
        {
            pd::Object o(42);
            pd::Object o1 = o.clone(o);
            CHECK(o == o1, "0. o == o1 should be true");
        }
        TEST_END(ObjectClone)
    }
}
