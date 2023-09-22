// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "generator.hpp"

#include <sstream>

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

		TEST(SetStep, true)
            Generator<int> g (1, 0, 2);
            int expected = 0;
            CHECK_EQ(int, g.get_current(),   expected, "0");
            CHECK_EQ(int, g.get()        ,          2, "1");
            CHECK_EQ(int, g.get()        ,          4, "2");
        TEST_END(SetStep)

        TEST(JsonConversion, true)
            Generator<int> g;
			Id id = g.get();
			CHECK_EQ(String, g.json(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"mvalue\" : 1, \"mstep\" : 1 }", "0");
			id = g.get();
			CHECK_EQ(String, g.json(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"mvalue\" : 2, \"mstep\" : 1 }", "1");
        TEST_END(JsonConversion)

        TEST(TextStreaming, true)
            Generator<int> g;
			std::stringstream ss;
            Id id = g.get();
			ss << g;
			CHECK_EQ(String, ss.str(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"mvalue\" : 1, \"mstep\" : 1 }", "0");
            id = g.get();
            ss.str ("");
			ss << g;
			CHECK_EQ(String, ss.str(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"mvalue\" : 2, \"mstep\" : 1 }", "1");
            Generator<int> g2;
            std::stringstream ss2;
            ss2 << g.json();
            ss2 >> g2;
            CHECK_EQ(Generator<int>, g2, g, "2");
            TEST_END(TextStreaming)

		TEST(BinaryStreaming, false)
			Generator<int> g;
            std::stringstream ss(std::ios_base::out | std::ios_base::in | std::ios_base::binary);
            
        TEST_END(BinaryStreaming)
    }
}
