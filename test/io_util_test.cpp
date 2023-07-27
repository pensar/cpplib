// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "../io_util.hpp"
#include "../sys_user_info.hpp"

#include <memory>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(PathTest, true)
            Path temp_dir = get_user_home<true>() / "test_dir";
            Path file1 = temp_dir / "file_name";
            Path file2 = temp_dir / "dir" / "file_name";
            TextFile tf (file1, "blah");
            CHECK(tf.exists (), "0");
            TextFile tf2 (file2, "blah");
            CHECK(tf2.exists(), "0");
            fs::last_write_time (file2, last_write_time (file1)); // Do timestamp file2 = timestamp file2.
            //CHECK_EQ(Path, file1, file2, "2");

            /*fs::last_write_time(file2, last_write_time(file1) + (time_t)60);
            BOOST_CHECK (! cpp::is_same (file1, file2));

            Path file3 = temp_dir / "file_name3";
            cpp::create_file (file3, "blah");

            BOOST_CHECK (cpp::is_same (file1, file3, cpp::SAME_SIZE + cpp::SAME_TIME));
            */
            TEST_END(PathTest)
    }
}
