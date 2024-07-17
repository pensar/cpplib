// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../unit-test/test/test.hpp"
#include "../cpp/command.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
	using namespace pensar_digital::cpplib;
    namespace cpplib
    {
		inline static int value = 0;
		class IncCmd : public Command
		{
		public:
			~IncCmd() = default;
			void _run  () override { ++value; }
			void _undo () const override { --value; }
		};

		class DecCmd : public Command
		{
		public:
			~DecCmd() = default;
			void _run  () override { --value; }
			void _undo () const override { ++value; }
		};

		class IncFailCmd : public Command
		{
		public:
			~IncFailCmd () = default;
			void _run  () override { throw "cmdAddFail.run () error."; }
			void _undo () const override { --value; }
		};

		class DoubleCmd : public Command
		{
		public:
			~DoubleCmd() = default;
			void _run  () override { value *= 2; }
			void _undo () const override { value /= 2; }
		};

		class DoubleFailCmd : public Command
		{
		public:
			~DoubleFailCmd() = default;
			void _run  () override { throw "Double errors."; }
			void _undo () const override { value /= 2            ; }
		};

		TEST(Command, true)
		{
			value = 0;

			IncCmd inc;
			CHECK_EQ(int, value, 0, "0");

			inc.run ();
			CHECK_EQ(int, value, 1, "1");

			inc.undo ();
			CHECK_EQ(int, value, 0, "2");

			DecCmd dec;

			CHECK_EQ(int, value, 0, "3");

			dec.run ();
			CHECK_EQ(int, value, -1, "4");

			dec.undo ();
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
			CompositeCommand cmd;

			cmd.add <IncCmd>(IncCmd ());
			CHECK_EQ(int, value, 0, "0");
			CHECK_EQ(size_t, cmd.count(), 1, "1");

			cmd.run();
			CHECK_EQ(int, value, 1, "2");

			value = 0;
			cmd.add <DecCmd>(DecCmd ());
			cmd.add <DecCmd>(DecCmd ());
			CHECK_EQ(size_t, cmd.count(), 3, "3");

			CHECK_EQ(int, value, 0, "4");

			cmd.run();
			CHECK_EQ(int, value, -1, "5");

			cmd.undo ();
			CHECK_EQ(int, value, 0, "6");


			auto inc_fail_cmd_id = cmd.add <IncFailCmd> (IncFailCmd ());
			CHECK_EQ(size_t, cmd.count(), 4, "7");
			CHECK_EQ(int, value, 0, "8");

			try
			{
				cmd.run();
			}
			catch (...)
			{
				CHECK_EQ(int, value, 0, "9");
			}

			CHECK_EQ(int, value, 0, "9.1");
			cmd.remove(inc_fail_cmd_id);
			CHECK_EQ(size_t, cmd.count(), 3, "10");
			CHECK_EQ(int, value, 0, "10.1");

			CHECK_EQ(int, value, 0, "11");
			cmd.clear ();
			CHECK_EQ(size_t, cmd.count(), 0, "12");
			CHECK_EQ(int, value, 0, "13");
			cmd.run ();
			CHECK_EQ(int, value, 0, "14");

			cmd.add <IncCmd> (IncCmd ());
			cmd.add <DoubleFailCmd> (DoubleFailCmd ());
			cmd.add <DoubleCmd>(DoubleCmd());
			CHECK_EQ(size_t, cmd.count(), 3, "15");
			CHECK_EQ(int, value, 0, "16");

			try
			{
				cmd.run();
			}
			catch (...)
			{
				CHECK_EQ(int, value, 0, "17");
			}
		}
		TEST_END(CompositeCommand)
	}
}
