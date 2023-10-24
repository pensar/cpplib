// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "generator.hpp"
#include "memory_buffer.hpp"

#include <sstream>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(Get, true)
            Generator<int> g;
            Id expected = 1;
            CHECK_EQ(Id, g.value(), expected++, "0");
            CHECK_EQ(Id, g.value(), expected++, "1");

            Generator<> g2(1, 1, 2);
            expected = 3;
            CHECK_EQ(Id, g2.value(), expected, "2");
            expected = 5;
            CHECK_EQ(Id, g2.value(), expected, "3");
        TEST_END(Get)

        TEST(GetNext, true)
            Generator<int> g;
            Id expected = 1;
            CHECK_EQ(Id, g.next(), expected, "0");
            CHECK_EQ(Id, g.next(), expected, "1");

            Generator<> g2(1, 1, 2);
            expected = 3;
            CHECK_EQ(Id, g2.next(), expected, "2");
            CHECK_EQ(Id, g2.next(), expected, "3");
        TEST_END(GetNext)

        TEST(GetCurrent, true)
            Generator<int> g;
            Id expected = 0;
            CHECK_EQ(Id, g.current(),   expected, "0");
            CHECK_EQ(Id, g.value()        , ++expected, "1");
            CHECK_EQ(Id, g.current(),   expected, "2");

            Generator<> g2(1, 1, 2);
            expected = 1;
            CHECK_EQ(Id, g2.current(), expected, "4");
            CHECK_EQ(Id, g2.value()        ,        3, "5");
            CHECK_EQ(Id, g2.current(),        3, "6");
        TEST_END(GetCurrent)
        

        TEST(SetValue, true)
            Generator<int> g;
            g.set_value (10);
            Id expected = 10;
            CHECK_EQ(Id, g.current (),         10, "0");
            CHECK_EQ(Id, g.value ()        , ++expected, "1");
        TEST_END(SetValue)

		TEST(SetStep, true)
            Generator<int> g (1, 0, 2);
            Id expected = 0;
            CHECK_EQ(Id, g.current (),   expected, "0");
            CHECK_EQ(Id, g.value ()        ,          2, "1");
            CHECK_EQ(Id, g.value ()        ,          4, "2");
        TEST_END(SetStep)

        TEST(JsonConversion, true)
            Generator<int> g;
			Id id = g.value();
			CHECK_EQ(String, g.json(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 1, \"mstep\" : 1 }", "0");
			id = g.value();
			CHECK_EQ(String, g.json(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 2, \"mstep\" : 1 }", "1");
        TEST_END(JsonConversion)

        TEST(TextStreaming, true)
            Generator<int> g;
			std::stringstream ss;
            Id id = g.value();
			ss << g;
			CHECK_EQ(String, ss.str(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 1, \"mstep\" : 1 }", "0");
            id = g.value();
            ss.str ("");
			ss << g;
			CHECK_EQ(String, ss.str(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 2, \"mstep\" : 1 }", "1");
            Generator<int> g2;
            std::stringstream ss2;
            ss2 << g.json();
            ss2 >> g2;
            CHECK_EQ(Generator<int>, g2, g, "2");
            TEST_END(TextStreaming)

		TEST(BinaryStreaming, true)
            typedef Generator<std::span<std::byte>> G;
            static_assert (BinaryOutputtableObject<G>);
            G g;
            MemoryBuffer buffer;
            buffer.write<G> (g);
            
            static_assert (FactoryConstructible<G, G::GeneratorFactory::P, G::IdType, G::IdType, G::IdType>);
            G g2; 
            G::GeneratorFactory::P ptr = buffer.fread<G, G::GeneratorFactory::P, G::IdType, G::IdType, G::IdType> (NULL_ID, 0, 1);
            CHECK_EQ(G, g2, g, "0");  
        TEST_END(BinaryStreaming)
    }
}
