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
            CHECK_EQ (Path, path, CURRENT_DIR<wchar_t>, "0");

            path = "\\\\";
            CHECK_EQ (Path, path, path.root_path (), "1");

            path = "c:\\tmp\\test\\path_test\\"; // Final slash indicates directory. If not present it assumes file.
            path.remove();

            // Verifies path does not exist.
            CHECK(!path.exists(), "2");

            path.create_dir ();
            CHECK(path.exists(), "3");

            CHECK(!path.has_filename (), "4");

            path += "\\path_test.txt";
            CHECK(path.has_filename (), "5");

            // Deletes file.
            fs::remove (path);

            // Verifies file does not exist.
            CHECK(!path.exists(), "6");

            Path path2 = path.filename ();

            CHECK_EQ (Path, path2, "path_test.txt", "7");

            path2 = path.parent_path ();
            CHECK_EQ (Path, path2, "c:\\tmp\\test\\path_test", "8");

            path2.remove ();
        TEST_END(Path)
    }
}