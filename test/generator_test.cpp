// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "generator.hpp"
#include "memory_buffer.hpp"
#include "concept.hpp"

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
            CHECK_EQ(Id, g.get_id (), expected++, "0");
            CHECK_EQ(Id, g.get_id (), expected++, "1");

            Generator<> g2(1, 1, 2);
            expected = 3;
            CHECK_EQ(Id, g2.get_id (), expected, "2");
            expected = 5;
            CHECK_EQ(Id, g2.get_id (), expected, "3");
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
            CHECK_EQ(Id, g.get_id ()        , ++expected, "1");
            CHECK_EQ(Id, g.current(),   expected, "2");

            Generator<> g2(1, 1, 2);
            expected = 1;
            CHECK_EQ(Id, g2.current(), expected, "4");
            CHECK_EQ(Id, g2.get_id ()        ,        3, "5");
            CHECK_EQ(Id, g2.current(),        3, "6");
        TEST_END(GetCurrent)
        

        TEST(SetValue, true)
            Generator<int> g;
            g.set_value (10);
            Id expected = 10;
            CHECK_EQ(Id, g.current (),         10, "0");
            CHECK_EQ(Id, g.get_id ()        , ++expected, "1");
        TEST_END(SetValue)

		TEST(SetStep, true)
            Generator<int> g (1, 0, 2);
            Id expected = 0;
            CHECK_EQ(Id, g.current (),   expected, "0");
            CHECK_EQ(Id, g.get_id ()        ,          2, "1");
            CHECK_EQ(Id, g.get_id ()        ,          4, "2");
        TEST_END(SetStep)

        TEST(JsonConversion, true)
            Generator<int> g;
			Id id = g.get_id ();
			CHECK_EQ(S, g.json(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 1, \"mstep\" : 1 }", "0");
			id = g.get_id ();
			CHECK_EQ(S, g.json(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 2, \"mstep\" : 1 }", "1");
        TEST_END(JsonConversion)

        TEST(TextStreaming, true)
            Generator<int> g;
			std::stringstream ss;
            Id id = g.get_id ();
			ss << g;
			CHECK_EQ(S, ss.str(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 1, \"mstep\" : 1 }", "0");
            id = g.get_id ();
            ss.str ("");
			ss << g;
			CHECK_EQ(S, ss.str(), "{ \"class\" : \"pensar_digital::cpplib::Generator<int,__int64>\", \"id\" : 0, \"VERSION\": { \"class\" : \"pensar_digital::cpplib::Version\" , \"id\" : 0, \"mpublic\" : 1, \"mprotected\" : 1, \"mprivate\" : 1 }, \"minitial_value\" : 0, \"mvalue\" : 2, \"mstep\" : 1 }", "1");
            Generator<int> g2;
            std::stringstream ss2;
            ss2 << g.json();
            ss2 >> g2;
            CHECK_EQ(Generator<int>, g2, g, "2");
            TEST_END(TextStreaming)

		TEST(BinaryStreaming, false)
            typedef Generator<Object> G;
            static_assert (BinaryWriteableObject<G>);
            static_assert (FactoryConstructible<G, G::IdType, G::IdType, G::IdType>);
            
            MemoryBuffer buffer;
            //G::Factory::P p = G::get (1, 0, 1);
            G g(1);
            Id id = g.get_id ();
            buffer.write<G> (g);
            
            static_assert (ObjectBinaryWriteable<MemoryBuffer, G>);
            static_assert (BinaryReadable<MemoryBuffer>);
            static_assert (Sizeofable<G>);
            static_assert (ObjectBinaryReadable<MemoryBuffer, G>);
            static_assert (FactoryObjBinaryReadable<MemoryBuffer, G>);

            //G::Factory::P ptr = buffer.read<G, G::IdType, G::IdType, G::IdType>(NULL_ID, 0, 1);

            G::Factory::P p2 = buffer.read<G> (1);
            G g2(1);

            CHECK_NOT_EQ(G, g2, g, "0");
            CHECK_EQ(G, *p2, g, "1");
            
            
            G::Factory::P p3 = buffer.write<G>(3, 2, 1);
            G::Factory::P p4 = buffer.read<G> (3);
            CHECK_EQ(G, *p4, *p3, "2");
        TEST_END(BinaryStreaming)
    }
}
