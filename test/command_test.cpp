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
        TEST(Command, true)
		{
			static int value = 0;
			// Arrange
			class CommandInc : public Command
			{
				public:
					~CommandInc () = default;
					virtual void run() override { ++value; }
					virtual void rollback() override { --value; }
			};

			class CommandDec : public Command
			{
				public:
					~CommandDec() = default;
					void run() override { --value; }
					void rollback() override { ++value; }
			};

			class CommandFail : public Command
			{
				public:
					//Destructor
					~CommandFail() = default;
					void run() override { throw "cmd.run () error."; }
					void rollback() override { ; }
			};

			class CommandDoubleFail : public Command
			{
				public:
					~CommandDoubleFail() = default;
					void run () override { throw "Double errors."; }// value *= 2; }
					void rollback () override { value /= 2; }
			};

			CommandInc inc;
			CHECK_EQ(int, value, 0, "0");

			inc.run ();
			CHECK_EQ(int, value, 1, "1");

			inc.rollback ();
			CHECK_EQ(int, value, 0, "2");

			CommandDec dec;

			CHECK_EQ(int, value, 0, "3");

			dec.run ();
			CHECK_EQ(int, value, -1, "4");

			dec.rollback ();
			CHECK_EQ(int, value, 0, "5");
			
			CommandFail fail;
			CompositeCommand cmd;

			cmd.add (std::make_unique <CommandInc> (inc));
			CHECK_EQ(int, value, 0, "6");

			CHECK_EQ(size_t, cmd.count (), 1, "7");

			cmd.run ();
			CHECK_EQ(int, value, 1, "8");

			value = 0;
			cmd.add(std::make_unique <CommandDec>(dec));
			cmd.add(std::make_unique <CommandDec>(dec));
			CHECK_EQ(size_t, cmd.count (), 3, "9");

			CHECK_EQ(int, value, 0, "10");

			cmd.run();
			CHECK_EQ(int, value, -1, "11");

			cmd.rollback();
			CHECK_EQ(int, value, 0, "12");

			cmd.add(std::make_unique <CommandFail>(fail));
			CHECK_EQ(size_t, cmd.count (), 4, "13");

			CHECK_EQ(int, value, 0, "14");

			try
			{
				cmd.run();
			}
			catch (...)
			{
				CHECK_EQ(int, value, 0, "15");
			}

			CommandDoubleFail double_fail;

		}
		TEST_END(Command)
    }
}
