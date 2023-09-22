// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "generator.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(Get, true)
            Generator<int> g;
            int expected = 1;
            CHECK_EQ(int, g.get(), expected++, "0");
            CHECK_EQ(int, g.get(), expected++, "1");

            Generator<> g2(1, 1, 2);
            expected = 3;
            CHECK_EQ(Id, g2.get(), expected, "2");
            expected = 5;
            CHECK_EQ(Id, g2.get(), expected, "3");
        TEST_END(Get)

        TEST(GetNext, true)
            Generator<int> g;
            int expected = 1;
            CHECK_EQ(int, g.get_next(), expected, "0");
            CHECK_EQ(int, g.get_next(), expected, "1");

            Generator<> g2(1, 1, 2);
            expected = 3;
            CHECK_EQ(Id, g2.get_next(), expected, "2");
            CHECK_EQ(Id, g2.get_next(), expected, "3");
        TEST_END(GetNext)

        TEST(GetCurrent, true)
            Generator<int> g;
            int expected = 0;
            CHECK_EQ(int, g.get_current(),   expected, "0");
            CHECK_EQ(int, g.get()        , ++expected, "1");
            CHECK_EQ(int, g.get_current(),   expected, "2");

            Generator<> g2(1, 1, 2);
            expected = 1;
            CHECK_EQ(Id, g2.get_current(), expected, "4");
            CHECK_EQ(Id, g2.get()        ,        3, "5");
            CHECK_EQ(Id, g2.get_current(),        3, "6");
        TEST_END(GetCurrent)
        

        TEST(SetValue, true)
            Generator<int> g;
            g.set_value(10);
            int expected = 10;
            CHECK_EQ(int, g.get_current(),         10, "0");
            CHECK_EQ(int, g.get()        , ++expected, "1");
        TEST_END(SetValue)
    }
}
