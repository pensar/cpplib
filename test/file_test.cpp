// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../file.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(TextFile, true)
            TextFile file ("c:\\tmp\\test\\path_test\\text-file-test.txt");
        TEST_END(TextFile)
    }
}