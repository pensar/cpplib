// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
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

            CompositeConstraint<StringConstraint, StringConstraint> sc3 = (sc || sc2);
            CHECK( sc3.ok ("bola" ), "10");
            CHECK( sc3.ok ("123"  ), "11"); 
            CHECK(!sc3.ok ("123bola" ), "12");
        TEST_END (Constraint);

        TEST(RangeConstraint, true)
			RangeConstraint<int> rc (0, 10);
			CHECK( rc.ok (0  ), "0");
			CHECK( rc.ok (10 ), "1");
			CHECK( rc.ok (5  ), "2");
			CHECK(!rc.ok (-1 ), "3");
			CHECK(!rc.ok (11 ), "4");
            CHECK(!rc.ok (100), "5");

            RangeConstraint<char> rc2 ('A', 'E');
            CHECK( rc2.ok ('A'), "6");  
            CHECK( rc2.ok ('E'), "7");
            CHECK( rc2.ok ('C'), "8");
            CHECK(!rc2.ok ('@'), "9");
            CHECK(!rc2.ok ('F'), "10");
            CHECK(!rc2.ok ('a'), "11");
            CHECK(!rc2.ok ('e'), "12");

            RangeConstraint<int> rc3 (10);
            CHECK( rc3.ok (10), "13");
            CHECK(!rc3.ok (9 ), "14");
            CHECK(!rc3.ok (11), "15");
		TEST_END (RangeConstraint);

        TEST(CompositeConstraints, true)
            RangeConstraint<int> baby (0, 2);
            RangeConstraint<int> kid  (3, 10);
            RangeConstraint<int> teen (11, 19);
            RangeConstraint<int> adult(20, 60);
            RangeConstraint<int> old  (61, 200);

            typedef CompositeConstraint<RangeConstraint<int>, RangeConstraint<int>> CompositeRangeInt;
            CompositeRangeInt babies_or_kids = (baby || kid);
			CHECK( babies_or_kids.ok ( 0), "0");
			CHECK(!babies_or_kids.ok (11), "1");
            CHECK( babies_or_kids.ok ( 3), "2");   
            CHECK( babies_or_kids.ok (10), "3");

            CompositeRangeInt teens_or_adults = (teen || adult);
            CHECK( teens_or_adults.ok ( 11), "4");  
            CHECK( teens_or_adults.ok ( 19), "5");
            CHECK( teens_or_adults.ok ( 20), "6");  
            CHECK( teens_or_adults.ok ( 60), "7");
            CHECK(!teens_or_adults.ok ( 10), "8");
            CHECK(!teens_or_adults.ok ( 61), "9");

            typedef CompositeConstraint<CompositeRangeInt, RangeConstraint<int>> Composite3RangeInt;
            Composite3RangeInt no_adults = (babies_or_kids || teen);
            CHECK( no_adults.ok ( 0), "10");
            CHECK( no_adults.ok ( 3), "11");
            CHECK( no_adults.ok ( 10), "12");
            CHECK( no_adults.ok ( 11), "13");
            CHECK( no_adults.ok ( 19), "14");
            CHECK(!no_adults.ok ( 20), "15");
            CHECK(!no_adults.ok ( 60), "16");
            CHECK(!no_adults.ok ( 61), "17");

            RangeConstraint<int> legal (18, 200);
            CHECK(!legal.ok(0), "10");
            CHECK(!legal.ok(3), "11");
            CHECK(!legal.ok(10), "12");
            CHECK(!legal.ok(11), "13");
            CHECK(legal.ok(19), "14");
            CHECK(legal.ok(20), "15");
            CHECK(legal.ok(60), "16");
            CHECK(legal.ok(61), "17");

        TEST_END (CompositeConstraints)
    }   // cpplib.
}   // pensar_digital.
