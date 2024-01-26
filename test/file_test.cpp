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
            CHECK_EQ(char*, p.parent_path().cstr(), TMP_DIR.remove_trailing_separator ().cstr(), "0");
            std::string filename = p.filename_only().string();
            CHECK_EQ(std::string, p.extension(), ".txt", "1");
            CHECK_EQ(size_t, filename.length (), 8, "2");
            CHECK(filename.find_first_not_of ("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos, "3");
            
            // Check if it does not start with a number.
            CHECK(filename.find_first_of ("0123456789") != 0, "4");


	
        TEST_END(RandomFileNameGenerator)

        TEST(TextFile, true)
        try
        {
            TextFile<> file("c:\\tmp\\test\\file_test\\text-file-test.txt", "blah");
            file.close();
            
            // Checks if file exists.
            CHECK(file.exists(), "0");
            std::string s = file.read();
            CHECK_EQ(std::string, s, "blah", "1");

            // Deletes file.
            if (! file.remove())
            {
				CHECK(false, "2");
			}
        }
        catch (const std::exception& e)
        {
        }

            
            //TextFile<wchar_t> file (L"c:\\tmp\\test\\file_test\\text-file-test.txt", L"blah");
            //file.close();
            //file.open();
            //std::wstring s = file.read();
            //WCHECK_EQ(std::wstring, s, L"blah", L"1");
            
        TEST_END(TextFile)
    }
}