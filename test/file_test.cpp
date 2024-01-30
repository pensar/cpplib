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
        TEST(RandomFileNameGenerator, true)
            RandomFileNameGenerator<char> r;
            Path p = r ();
            CHECK_EQ(Path, p.parent_path(), TMP_PATH.copy_without_trailing_separator(), "0");
            std::string filename = p.filename_only().string();
            CHECK_EQ(std::string, p.extension(), ".txt", "1");
            CHECK_EQ(size_t, filename.length (), 8, "2");
            CHECK(filename.find_first_not_of ("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos, "3");
            
            // Check if it does not start with a number.
            CHECK(filename.find_first_of ("0123456789") != 0, "4");


	
        TEST_END(RandomFileNameGenerator)

        TEST(TextFile, true)
            Path p;
            {
                TmpTextFile<char> file("text-file-test.txt", "blah");
                p = file.fullpath ();
            
                // Checks if file exists.
                CHECK(file.exists(), "0");
                std::string s = file.read();
                CHECK_EQ(std::string, s, "blah", "1");
			}   
            CHECK(! p.exists(), "2");

            
            {
                TmpTextFile<wchar_t> file(L"wtext-file-test.txt", L"blah");
                p = file.fullpath ();
                std::wstring ws = file.read();
                WCHECK_EQ(std::wstring, ws, L"blah", L"3");
            }
            CHECK(! p.exists(), "4");
            
        TEST_END(TextFile)
    }
}