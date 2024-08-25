// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../unit-test/test/test.hpp"
#include "../cpp/command.hpp"
#include "../cpp/memory_buffer.hpp"

namespace pensar_digital
{
	namespace test = pensar_digital::unit_test;
	using namespace pensar_digital::unit_test;
	using namespace pensar_digital::cpplib;
	namespace cpplib
	{
		inline static int value = 0;
		
		class IncCmd : public Command<IncCmd>
		{
		public:
			IncCmd (const Id aid = NULL_ID) : Command(aid) { }	
			~IncCmd() = default;
			void _run() { ++value; }
			void _undo() const { --value; }
		};

		class DecCmd : public Command<DecCmd>
		{
		public:
			DecCmd (const Id aid = NULL_ID) : Command(aid) { }
			~DecCmd() = default;
			void _run  ()       { --value; }
			void _undo () const { ++value; }
		};

		class IncFailCmd : public Command<IncFailCmd>
		{
		public:
			IncFailCmd (const Id aid = NULL_ID) : Command(aid) { }
			~IncFailCmd() = default;
			void _run  ()       { throw "cmdAddFail.run () error."; }
			void _undo () const { --value; }
		};


		class DoubleCmd : public Command<DoubleCmd>
		{
			public:
			DoubleCmd (const Id aid = NULL_ID) : Command(aid) { }
			~DoubleCmd() = default;
			void _run  ()       { value *= 2; }
			void _undo () const { value /= 2; }
		};

		class DoubleFailCmd : public Command<DoubleFailCmd>
		{
			public:
				DoubleFailCmd (const Id aid = NULL_ID) : Command(aid) { }
			~DoubleFailCmd() = default;
			void _run  ()       { throw "Double errors."; }
			void _undo () const { value /= 2; }
		};

		TEST(Command, true)
		{
			value = 0;

			IncCmd inc;
			CHECK_EQ(int, value, 0, "0");

			inc.run();
			CHECK_EQ(int, value, 1, "1");

			inc.undo();
			CHECK_EQ(int, value, 0, "2");

			DecCmd dec;

			CHECK_EQ(int, value, 0, "3");

			dec.run();
			CHECK_EQ(int, value, -1, "4");

			dec.undo();
			CHECK_EQ(int, value, 0, "5");

			IncFailCmd inc_fail;
			try
			{
				inc_fail.run();
			}
			catch (...)
			{
				CHECK_EQ(int, value, 0, "6");
			}

		}
		TEST_END(Command)

			TEST(CompositeCommand, true)
			{
			
				using Cmd = CompositeCommand <IncCmd, DecCmd>;
				static_assert (Identifiable<Cmd>);
				Cmd cmd;

				value = 0;

				CHECK_EQ(int, value, 0, "0");

				cmd.run();
				CHECK_EQ(int, value, 0, "1");

				CompositeCommand <IncCmd, DecCmd, DecCmd, DecCmd> cmd2;
				CHECK_EQ(int, value, 0, "2");

				cmd2.run();
				CHECK_EQ(int, value, -2, "3");

				cmd2.undo();
				CHECK_EQ(int, value, 0, "4");


				CompositeCommand <IncCmd, DecCmd, IncFailCmd> cmd3;
				CHECK_EQ(int, value, 0, "5");

				try
				{
					cmd3.run();
				}
				catch (...)
				{
					CHECK_EQ(int, value, 0, "6");
				}

				CHECK_EQ(int, value, 0, "7");
		}
		TEST_END(CompositeCommand)
	}


	TEST(CommandBinaryStreaming, true)
	{
		using Cmd = IncCmd;



		MemoryBuffer buffer;
		Cmd cmd;
		/*
		buffer.write(cmd.to);

		Cmd::Factory::P p = buffer.read();
		Cmd p2;

		CHECK_NOT_EQ(Cmd, p2, cmd, W("0"));
		//CHECK_EQ(Cmd, *p, cmd, "1");


		Cmd::Factory::P p3 = buffer.write();
		Cmd::Factory::P p4 = nullptr;
		buffer.read(&p4);
		//CHECK_EQ(Cmd, *p4, *p3, "2");
		*/
	}
	TEST_END(CommandBinaryStreaming)

}
