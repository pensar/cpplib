// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit_test/src/test.hpp"


#include "../src/path.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(Path, true)
            static_assert (TriviallyCopyable<CPath>);
            Path path;
            CHECK_EQ (Path, path, CURRENT_DIR, W("0"));

            path = W("\\\\");
            CHECK_EQ (Path, path, path.root_path (), W("1"));

            path = W("c:\\tmp\\test\\path_test\\"); // Final slash indicates directory. If not present it assumes file.
            path.remove();

            // Verifies path does not exist.
            CHECK(!path.exists(), W("2"));

            path.create_dir ();
            CHECK(path.exists(), W("3"));

            CHECK(!path.has_filename (), W("4"));

            path += W("\\path_test.txt");
            CHECK(path.has_filename (), W("5"));

            // Deletes file.
            fs::remove (path);

            // Verifies file does not exist.
            CHECK(!path.exists(), W("6"));

            Path path2 = path.filename ();

            CHECK_EQ (Path, path2, W("path_test.txt"), W("7"));

            path2 = path.parent_path ();
            CHECK_EQ (Path, path2, W("c:\\tmp\\test\\path_test"), W("8"));

            path2.remove ();
        TEST_END(Path)
    }
}