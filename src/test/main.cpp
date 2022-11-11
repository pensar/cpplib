#include "../../unit_test/test.hpp"
#include "id_test.hpp"
#include <iostream>

using namespace pensar_digital::unit_test;
using namespace pensar_digital::cpplib;

int main()
{
    // This is done here to avoid the static initialization order problem.
    TestGetSet test_get_set;
    TestEqOperator test_eq_operator;
    TestDiffOperator test_diff_operator;

    AllTests.run ();
}
