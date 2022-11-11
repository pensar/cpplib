// $Id

#define BOOST_TEST_MODULE addres_book_test_module
#include <boost/test/included/unit_test.hpp>
#include <boost/lexical_cast.hpp>

#include "my_boost.hpp"
#include "Generator.hpp"

namespace pd  = pensar_digital;

// If I do not define this BOOST_CHECK_EQUAL_COLLECTIONS fails to compile with vector<wstring> !
namespace std
{
    ostream& operator << (ostream& ostr, wstring const& str)
    {
       ostr << boost::lexical_cast<wstring>(str);
       return ostr;
    }
}

BOOST_AUTO_TEST_SUITE(Generator_suite)

BOOST_AUTO_TEST_CASE(get_test)
{
    pd::Generator<int> g;
    int expected = 1;
    BOOST_CHECK_EQUAL (g.get (), expected++);
    BOOST_CHECK_EQUAL (g.get (), expected++);

    pd::Generator<> g2(1, 2);
    expected = 3;
    BOOST_CHECK_EQUAL (g2.get (), expected);
    expected = 5;
    BOOST_CHECK_EQUAL (g2.get (), expected);
}

BOOST_AUTO_TEST_CASE(get_next_test)
{
    pd::Generator<int> g;
    int expected = 1;
    BOOST_CHECK_EQUAL (g.get_next (), expected);
    BOOST_CHECK_EQUAL (g.get_next (), expected);

    pd::Generator<> g2(1, 2);
    expected = 3;
    BOOST_CHECK_EQUAL (g2.get_next (), expected);
    BOOST_CHECK_EQUAL (g2.get_next (), expected);
}

BOOST_AUTO_TEST_CASE(get_current_test)
{
    pd::Generator<int> g;
    int expected = 0;
    BOOST_CHECK_EQUAL (g.get_current (), expected);
    BOOST_CHECK_EQUAL (g.get (), ++expected);
    BOOST_CHECK_EQUAL (g.get_current (), expected);

    pd::Generator<> g2(1, 2);
    expected = 1;
    BOOST_CHECK_EQUAL (g2.get_current (), expected);
    BOOST_CHECK_EQUAL (g2.get (), 3);
    BOOST_CHECK_EQUAL (g2.get_current (), 3);
}

BOOST_AUTO_TEST_CASE(set_value_test)
{
    pd::Generator<int> g;
    g.set_value (10);
    int expected = 10;
    BOOST_CHECK_EQUAL (g.get_current (), 10);
    BOOST_CHECK_EQUAL (g.get (), ++expected);
}

BOOST_AUTO_TEST_SUITE_END()
