// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../stop_watch.hpp"
#include "../constraint.hpp"


namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(StopWatch, true)
            StopWatch<> sp;
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
            sp.mark ();
            sp.stop ();
            StopWatch<>::ELAPSED_TYPE elapsed = sp.elapsed ();   
            CHECK(elapsed >= 1000000, "0.");
            CHECK(elapsed  < 30000000, "1.");
            StopWatch<>::ELAPSED_TYPE mark_elapsed = sp.elapsed_since_mark ();
            String elapsed_formatted = sp.elapsed_formatted ();
            String elapsed_since_mark_formatted = sp.elapsed_since_mark_formatted ();
            CHECK(mark_elapsed < elapsed, "2. elapsed = " + elapsed_formatted + " elapsed_since_mark = " + elapsed_since_mark_formatted);
            std::cout << "elapsed = " << elapsed_formatted << " elapsed_since_mark = " << elapsed_since_mark_formatted << std::endl;
        TEST_END(StopWatch)
    }
}
