// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include "../object_factory.hpp"
#include "constraint.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        TEST(Constraint, true)
            StringConstraint sc (ONLY_DIGITS_REGEX);

            CHECK( sc.ok ("123" ), "0"); 
            CHECK(!sc.ok ("123a"), "1");
            CHECK(!sc.ok ("a123"), "2");
            CHECK( sc.ok (""    ), "3");
            CHECK(!sc.ok (" "   ), "4");
            CHECK(!sc.ok (" 123"), "5");
            CHECK( sc.ok ("0"   ), "6");

            StringConstraint sc2("^bola$");
            CHECK( sc2.ok ("bola" ), "7");
            CHECK(!sc2.ok (" bola"), "8");
            CHECK(!sc2.ok (""     ), "9");

            CompositeConstraint<StringConstraint, StringConstraint, OrOperand<StringConstraint, StringConstraint>> sc3 (sc, sc2, * new OrOperand<StringConstraint, StringConstraint> ());
            CHECK( sc3.ok ("bola" ), "10");
            CHECK( sc3.ok ("123"  ), "11"); 
            CHECK(!sc3.ok ("123bola" ), "12");
        TEST_END (Constraint)
    }
}
