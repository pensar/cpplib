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
        /*
        TEST(S, true)
            static_assert(StdLayoutTriviallyCopyable<S<>>, "S is not compliant with StdLayoutTriviallyCopyable concept.");
            S<> s;
            S<> s1;  
            CHECK_EQ(S<>, s, s1, "0");
            s = "abc";
            CHECK_NOT_EQ(S<>, s, s1, "1");
            CHECK_EQ(S<>, s, "abc", "2");
            CHECK_EQ(size_t, s.length (),  3, "3");
            CHECK_EQ(size_t,   s.size (), S<>::MAX_SIZE, "4");

            S s2 = "abc";
            CHECK_EQ(S<>, s2.to_string (), "abc", "5");
            CHECK (s == s2, "6");

            s2 = "def";
            CHECK_EQ(S<>, s2, "def", "7");
            static_assert (pd::OutputStreamable<S<>>, "S is not OutputStreamable");
            typedef S<wchar_t> W;
            W w = W("abc";
            static_assert (pd::OutputStreamable<W>, "W is not OutputStreamable");
            WCHECK_EQ(W, w, W("abc", W("8");

            std::string str = "abc";
            S<> s3 = str;
            CHECK_EQ(S<>, s3, "abc", "9");

            std::wstring wstr = W("abc";
            W w2 = wstr;    
            WCHECK_EQ(W, w2, W("abc", W("10");

            S<> s4 = "abc";
            S<> s5 = "def";
            S<> s6 = s4 + s5;
            CHECK_EQ(S<>, s6, "abcdef", "11");

        TEST_END(S)
        */

        TEST(CS, true)
            static_assert(StdLayoutTriviallyCopyable<CS<10>>, W("S is not compliant with StdLayoutTriviallyCopyable concept."));
            CS<> s;
            CS<> s1;
            CHECK_EQ(CS<>, s, s1, W("0"));
            s = "abc";
            CHECK_NOT_EQ(CS<>, s, s1, W("1"));
            CHECK_EQ(CS<>, s, "abc", W("2"));
            CHECK_EQ(size_t, s.length(), 3, W("3"));
            CHECK_EQ(size_t, s.size(), CS<>::MAX_SIZE, W("4"));

            CS s2 = "abc";
            CHECK_EQ(CS<>, s2.to_string(), W("abc"), W("5"));
            CHECK(s == s2, W("6"));

            s2 = W("def");
            CHECK_EQ(CS<>, s2, W("def"), W("7"));
            static_assert (pd::OutputStreamable<CS<>>, "S is not OutputStreamable");
            typedef CS<0, 20> WC;
            WC w = W("abc");
            static_assert (pd::OutputStreamable<WC>, "W is not OutputStreamable");

            std::string str = W("abc");
            CS<> s3 = str;
            CHECK_EQ(CS<>, s3, W("abc"), W("9"));


            CS<> s4 = W("abc");
            CS<> s5 = W("def");
            CS<> s6 = s4 + s5;
            CHECK_EQ(CS<>, s6, W("abcdef"), W("11"));

            TEST_END(CS)
    }
}
