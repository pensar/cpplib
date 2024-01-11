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

            path = "\\\\";
            CHECK_EQ (Path, path, path.root_path (), "1");

            path = "c:\\tmp\\test\\path_test\\"; // Final slash indicates directory. If not present it assumes file.

            // Verifies path does not exist.
            CHECK(!path.exists(), "2");

            path.create_dir ();
            CHECK(path.exists(), "3");

            path.remove ();
            // Verifies path does not exist.
            CHECK(!path.exists(), "4");

        TEST_END(Path)
    }
}