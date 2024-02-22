// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../s.hpp"
#include "../stop_watch.hpp"
#include "../constraint.hpp"
#include <thread>
#include <chrono>

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(StopWatch, true)
            StopWatch<> sp;
            std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
            sp.mark ();
            sp.stop ();
            StopWatch<>::ELAPSED_TYPE elapsed = sp.elapsed ();   
            CHECK(elapsed >= 10000, "0.");
            StopWatch<>::ELAPSED_TYPE mark_elapsed = sp.elapsed_since_mark ();
            S elapsed_formatted = sp.elapsed_formatted ();
            S elapsed_since_mark_formatted = sp.elapsed_since_mark_formatted ();
            CHECK(mark_elapsed < elapsed, W("1. elapsed = ") + elapsed_formatted + W(" elapsed_since_mark = ") + elapsed_since_mark_formatted);
            //out () << "elapsed = " << elapsed_formatted << " elapsed_since_mark = " << elapsed_since_mark_formatted << std::endl;
        TEST_END(StopWatch)
    }
}
