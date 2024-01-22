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
            std::fstream fs("c:\\tmp\\test\\file_test\\text-file-test.txt", std::ios::out | std::ios::in | std::ios::trunc);
        try
        {
            TextFile<> file("c:\\tmp\\test\\file_test\\text-file-test.txt", "blah");
            file.close();
            // Checks if file exists.
            
            CHECK(file.exists(), "0");
            std::string s = file.read();
            CHECK_EQ(std::string, s, "blah", "1");

            // Deletes file.
            if (file.remove() != 0)
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