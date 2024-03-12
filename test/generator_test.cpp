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

	
        TEST(GeneratorFileBinaryStreaming, true)
            std::ofstream out("c:\\tmp\\test\\GeneratorFileBinaryStreaming\\file_binary_streaming_test.bin", std::ios::binary);
            typedef Generator<Object> G;
            typedef std::shared_ptr<G> GP;
            G g(1);
            g.write(out);
            out.close();
            out.flush();

            std::ifstream in("c:\\tmp\\test\\GeneratorFileBinaryStreaming\\file_binary_streaming_test.bin", std::ios::binary);
            GP pg2 = G::get(1);
            pg2->read(in);
            in.close();
            G g3(3);

            CHECK_NOT_EQ(G, g3, g, "0");
            CHECK_EQ(G, *pg2, g, "1");

        TEST_END(GeneratorFileBinaryStreaming)

        TEST(GeneratorBinaryStreaming, true)
            typedef Generator<Object> G;
        static_assert (Identifiable <G>);
        static_assert (Hashable<G>);
        static_assert (TriviallyCopyable <G::DataType>);
        static_assert (StandardLayout <G::DataType>);
        static_assert (StdLayoutTriviallyCopyable <G::DataType>);
        static_assert (Persistable<G>);

            ObjMemoryBuffer<G> buffer;
            //G::Factory::P p = G::get (1, 0, 1);
            G g(1);
            Id id = g.get_id ();
            buffer.write (g);
            Hash h = g.hash();

    
            //G::Factory::P ptr = buffer.read<G, G::IdType, G::IdType, G::IdType>(null_value<T>(), 0, 1);

            G::Factory::P p2 = buffer.read (1);
            G g2(1);
            Hash h2 = g2.hash();

            CHECK_NOT_EQ(G, g2, g, "0");
            //CHECK_EQ(G, *p2, g, "1");
            
            
            G::Factory::P p3 = buffer.write<Id, Id, Id>(3, 2, 1);
            //G::Factory::P p4 = buffer.read<G> (3);
            //CHECK_EQ(G, *p4, *p3, "2");
        TEST_END(GeneratorBinaryStreaming)
    }
}
