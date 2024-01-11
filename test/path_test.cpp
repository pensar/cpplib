// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../path.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(Path, true)
            static_assert (StdLayoutTriviallyCopyable<CPath<>>);
            Path path;
            CHECK_EQ (Path, path, CURRENT_DIR, "0");
        TEST_END(Path)
    }
}