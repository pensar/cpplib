// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)
#include "../unit-test/test/test.hpp"


int main()
{
    namespace test = pensar_digital::unit_test;
    test::all_tests ().run ();
    return 0;
}
