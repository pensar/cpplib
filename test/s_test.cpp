// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../s.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {

        TEST(S, true)
            static_assert(StdLayoutTriviallyCopyable<S<10>>, "S is not compliant with StdLayoutTriviallyCopyable concept.");

            typedef S<10> S1;
            typedef S<10> S2;
            S1 s;
            S1 s1;  
            CHECK_EQ(S1, s, s1, "0");
            s = "abc";
            CHECK_NOT_EQ(S1, s, s1, "1");
            CHECK_EQ(S1, s, "abc", "2");
            CHECK_EQ(size_t, s.length (),  3, "3");
            CHECK_EQ(size_t,   s.size (), 10, "4");

            S2 s2 = "abc";
            CHECK_EQ(S2, s2, "abc", "5");
            CHECK (s == s2, "6");

            s2 = "def";
            CHECK_EQ(S2, s2, "def", "7");
            static_assert (pd::OutputStreamable<S1>, "S is not OutputStreamable");
            typedef S<10, wchar_t> W;
            W w = L"abc";
            static_assert (pd::OutputStreamable<W>, "W is not OutputStreamable");
            WCHECK_EQ(W, w, L"abc", L"8");

            std::string str = "abc";
            S1 s3 = str;
            CHECK_EQ(S1, s3, "abc", "9");

            std::wstring wstr = L"abc";
            W w2 = wstr;    
            WCHECK_EQ(W, w2, L"abc", L"10");

            S1 s4 = "abc";
            S1 s5 = "def";
            S1 s6 = s4 + s5;
            CHECK_EQ(S1, s6, "abcdef", "11");

        TEST_END(S)
    }
}
