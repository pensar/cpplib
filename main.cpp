#include "..\unit-test\test.hpp"

int main()
{
    namespace test = pensar_digital::unit_test;
    test::all_tests ().run ();
    return 0;
}
