// $Id

#include <boost/test/unit_test.hpp>
#include "my_boost.hpp"
#include "Name.hpp"
#include "wstring_stream_hack.hpp"

namespace pd  = pensar_digital;

template <class String = std::wstring>
class Dummy: public pd::Name<String>
{
    public:
      Dummy(const String& aname = L""): pd::Name<String>(aname){};
};

template <>
class Dummy<std::string>: public pd::Name<std::string>
{
    public:
      Dummy(const std::string& aname = ""): pd::Name<std::string>(aname){};
};

BOOST_AUTO_TEST_SUITE(Name_suite)

BOOST_AUTO_TEST_CASE(get_set_test)
{
    std::string expected = "aName";
    Dummy<std::string> n(expected);
    BOOST_CHECK_EQUAL (n.get_name (), expected);

    expected = "another name";
    n.set_name(expected);
    BOOST_CHECK_EQUAL (n.get_name (), expected);

    expected = "";
    Dummy<std::string> n2;
    BOOST_CHECK_EQUAL (n2.get_name (), expected);
}

BOOST_AUTO_TEST_CASE(eq_operator_test)
{
    Dummy<std::string> n;
    Dummy<std::string> n2;
    BOOST_CHECK (n == n2);

    n.set_name ("a");
    BOOST_CHECK (! (n == n2));
}

BOOST_AUTO_TEST_CASE(diff_operator_test)
{
    Dummy<std::string> n;
    Dummy<std::string> n2;
    BOOST_CHECK (!(n != n2));

    n.set_name ("a");
    BOOST_CHECK (n != n2);
}

BOOST_AUTO_TEST_CASE(wget_set_test)
{
    std::wstring expected = L"aName";
    Dummy<std::wstring> n(expected);
    BOOST_CHECK_EQUAL (n.get_name (), expected);

    expected = L"another name";
    n.set_name(expected);
    BOOST_CHECK_EQUAL (n.get_name (), expected);

    expected = L"";
    Dummy<> n2;
    BOOST_CHECK_EQUAL (n2.get_name (), expected);
}

BOOST_AUTO_TEST_CASE(weq_operator_test)
{
    Dummy<> n;
    Dummy<> n2;
    BOOST_CHECK (n == n2);

    n.set_name (L"a");
    BOOST_CHECK (! (n == n2));
}

BOOST_AUTO_TEST_CASE(wdiff_operator_test)
{
    Dummy<> n;
    Dummy<> n2;
    BOOST_CHECK (!(n != n2));

    n.set_name (L"a");
    BOOST_CHECK (n != n2);
}

BOOST_AUTO_TEST_SUITE_END()
