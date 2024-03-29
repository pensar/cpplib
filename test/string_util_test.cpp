// $Id: string_util_test.cpp 70 2014-01-16 07:04:22Z mgpensar $

#ifdef VISUAL_STUDIO
#include "stdafx.h"
#endif

#define BOOST_TEST_MODULE cpplib_test_module
#include <boost/test/included/unit_test.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#include "string_util.hpp"
#include "my_boost.hpp"

//____________________________________________________________________________//

using namespace std;
namespace cpp = pensar_digital::cpplib;

// If I do not define this BOOST_CHECK_EQUAL_COLLECTIONS fails to compile with vector<wstring> !
namespace std
{
    ostream& operator << (ostream& ostr, wstring const& str)
    {
       ostr << boost::lexical_cast<wstring>(str);
       return ostr;
    }
}

BOOST_AUTO_TEST_SUITE(string_util_suite)

BOOST_AUTO_TEST_CASE(split_test)
{
	typedef map<string, vector<string> > Map;
	vector<string> v0, v1, v2, v3;
	v0.push_back ("");
	v1.push_back ("blah");
	v2.push_back ("a");
	v2.push_back ("1");
	v3.push_back ("2009");
	v3.push_back ("10");
	v3.push_back ("09");
	vector<string> v4 = {"a", "b", "c", "d"};
	Map map;
	map[""      ] = v0;
	map["blah"  ] = v1;
	map["a, 1"  ] = v2;
    map["2009,10,09"] = v3;
    map["a,b,,c,,,d"] = v4;
	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		vector<string> v;
		cpp::split (s, ',', v);

		CHECK_EQUAL_COLLECTIONS(i->second, v);

		//cpp::StringVectorPtr vp = cpp::split (s, ',');
		//CHECK_EQUAL_COLLECTIONS(i->second, *vp);
	}

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		set<string> sset;
		cpp::split<set<string>> (s, ',', sset);
        vector<string> v (sset.size ());
        std::copy (sset.begin(), sset.end (), v.begin ());
        std::sort (v.begin (), v.end ());
        std::sort (i->second.begin (), i->second.end ());
		CHECK_EQUAL_COLLECTIONS(i->second, v);
	}

	v2[1] = " 1";
	vector<string> v;
	cpp::split ("a, 1", ',', v, false);
	CHECK_EQUAL_COLLECTIONS (v2, v);
}

BOOST_AUTO_TEST_CASE(split_test2)
{
    std::string s = "ESPN360 BRASIL, ESPN360";
    vector<string> v;
    cpp::split (s, ',', v);
    BOOST_CHECK_EQUAL (2u, v.size ());

    set<string> sset;
    cpp::split<set<string>> (s, ',', sset);
    BOOST_CHECK_EQUAL (2u, sset.size ());
}

#ifdef UNICODE
BOOST_AUTO_TEST_CASE(split_wstring_test)
{
	typedef map<wstring, vector<wstring> > Map;
	vector<wstring> v0, v1, v2;
	v0.push_back (L"");
	v1.push_back (L"blah");
	v2.push_back (L"a");
	v2.push_back (L"1");
	Map map;
	map[L""    ] = v0;
	map[L"blah"] = v1;
	map[L"a, 1"] = v2;

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		wstring s = i->first;
		vector<wstring> v;
		cpp::split (s, ',', &v);

		CHECK_EQUAL_COLLECTIONS(i->second, v);

		cpp::WStringVectorPtr vp = cpp::split (s, ',');
		CHECK_EQUAL_COLLECTIONS(i->second, *vp);
	}

	v2[1] = L" 1";
	vector<wstring> v;
	cpp::split (L"a, 1", L',', &v, false);
	CHECK_EQUAL_COLLECTIONS (v2, v);
}
#endif

BOOST_AUTO_TEST_CASE(pad_copy_test)
{
    string abc = "abc";
	BOOST_CHECK_EQUAL ("abc   ", cpp::pad_copy (abc, ' ', 6));
	BOOST_CHECK_EQUAL ("abc   ", cpp::pad_copy ("abc", ' ', 6));
	BOOST_CHECK_EQUAL ("abc",    cpp::pad_copy ("abc", ' ', 3));

	// Uncomment the lines below to test the assertions.
	// Negative values are not allowed.
	//pad ("abc", ' ', -2);
	//pad (0,  ' ', 6);

	BOOST_CHECK_EQUAL ("abc ",   cpp::pad_copy ("abc", ' ', 4));
	string empty = "";
	BOOST_CHECK_EQUAL ("      ", cpp::pad_copy (empty, ' ', 6));

	BOOST_CHECK_EQUAL ("abc000", cpp::pad_copy ("abc", '0', 6));
	BOOST_CHECK_EQUAL ("abc",    cpp::pad_copy ("abc", '0', 3));
	BOOST_CHECK_EQUAL ("abc0",   cpp::pad_copy ("abc", '0', 4));
	BOOST_CHECK_EQUAL ("000000", cpp::pad_copy (empty, '0', 6));

	BOOST_CHECK_EQUAL ("000abc", cpp::pad_copy ("abc", '0', 6, cpp::PAD_LEFT));
}

BOOST_AUTO_TEST_CASE(only_digits_test)
{
	typedef map<string, string, less<string> > Map;
	Map map;
	map[""       ] = "";
	map["blah"   ] = "";
	map["12 34a5"] = "12345";
	map["123.607.834-07"    ] = "12360783407";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		string result  = cpp::only_digits (s);

		BOOST_CHECK_EQUAL (result, i->second);
	}
}

BOOST_AUTO_TEST_CASE(only_alpha_numeric_test)
{
	typedef map<string, string> Map;
	Map map;
	map[""       ] = "";
	map["blah"   ] = "blah";
	map["12 34a5"] = "1234a5";
	map["123.607.834-07"    ] = "12360783407";
	map["a-1,b;2"] = "a1b2";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		string result  = cpp::only_alpha_numeric (s);

		BOOST_CHECK_EQUAL (result, i->second);
	}
}

BOOST_AUTO_TEST_CASE(remove_blanks_test)
{
	typedef map<string, string, less<string> > Map;
	Map map;
	map[""                   ] = "";
	map["blah"               ] = "blah";
	map[" blah "             ] = "blah";
	map["12 34 5"            ] = "12345";
	map["123  456\n\n789\t01"] = "12345678901";
	map["� � � � � A � � � �"] = "�����A����";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string result = i->first;
		cpp::remove_blanks (result);

		BOOST_CHECK_EQUAL (result, i->second);
	}
}

BOOST_AUTO_TEST_CASE(remove_accents_test)
{
	typedef map<string, string, less<string> > Map;
	Map map;
	map[""      ] = "";
	map["blah"  ] = "blah";
	map["Jos� da Silva"  ] = "Jose da Silva";
	map["����� ���� ���� ����� ���� �"] = "aaaaa eeee iiii ooooo uuuu c";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		string result  = cpp::copy_remove_accents (s);

		BOOST_CHECK_EQUAL (result, i->second);
	}

	string a = "�";
	string s = cpp::copy_remove_accents (a);
	BOOST_CHECK_EQUAL ("�", a);
	BOOST_CHECK_EQUAL ("a", s);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
	typedef map<string, string> Map;
	Map map;

	map[""      ] = "";
	map["blah"  ] = "blah";
	map["AO VIVO Jos� da Silva"  ] = " Jos� da Silva";
	map["Campeonato Paulista: Palmeiras x Corinthians AO VIVO"] = "Campeonato Paulista: Palmeiras x Corinthians ";
    map["AO VIVO"] = "";
    map["1 AO VIVO2"] = "1 2";
    map["1 AO VIVO 2"] = "1  2";
    map["1AO VIVO2"] = "12";
    map["1AO VIVO234"] = "1234";
	map["AO VIVO, Jos� da Silva"  ] = ", Jos� da Silva";
	map["AO VIVO1 Jos� da Silva"  ] = "1 Jos� da Silva";
	map["AO VIVO2 Jos� da Silva"  ] = "2 Jos� da Silva";
	map["1AO VIVO2 Jos� da Silva"  ] = "12 Jos� da Silva";
	map["---AO VIVO---"] = "------";
	map["1AO VIVO2AO VIVO3"] = "123";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		string result  = cpp::copy_remove (string("AO VIVO"), s);

		BOOST_CHECK_EQUAL (result, i->second);
	}
}


BOOST_AUTO_TEST_CASE(remove_delimiter_test)
{
	typedef map<string, string, less<string> > Map;
	Map map;
	map[""      ] = "";
	map["blah"  ] = "blah";
	map["\"Jos� da Silva\""  ] = "Jos� da Silva";
	map["\"����� ���� ���� ����� ���� �\""] = "����� ���� ���� ����� ���� �";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		cpp::remove_delimiters ('"', s);

		BOOST_CHECK_EQUAL (s, i->second);
	}
}


BOOST_AUTO_TEST_CASE(reverse_test)
{
	typedef map<string, string> Map;
	Map map;
	map[""       ] = "";
	map["blah"   ] = "halb";
	map["12 34a5"] = "5a43 21";
	map["123.607.834-07"    ] = "70-438.706.321";
	map["a"] = "a";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		string result  = cpp::reverse (s);

		BOOST_CHECK_EQUAL (result, i->second);
	}
}

BOOST_AUTO_TEST_CASE(wreverse_test)
{
	typedef map<wstring, wstring> Map;
	Map map;
	map[L""       ] = L"";
	map[L"blah"   ] = L"halb";
	map[L"12 34a5"] = L"5a43 21";
	map[L"123.607.834-07"    ] = L"70-438.706.321";
	map[L"a"] = L"a";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		wstring s = i->first;
		wstring result  = cpp::reverse (s);

		BOOST_CHECK_EQUAL (result, i->second);
	}
}

BOOST_AUTO_TEST_CASE(to_lower_test)
{
	typedef map<string, string> Map;
	Map map;

	map[""      ] = "";
	map["blah"  ] = "blah";
	map["AO VIVO JOS�"] = "ao vivo jos�";
	map["Campeonato Paulista: Palmeiras x Corinthians AO VIVO"] = "campeonato paulista: palmeiras x corinthians ao vivo";
    map["1 AO VIVO2"] = "1 ao vivo2";
	map["---"] = "---";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		cpp::to_lower (s);

		BOOST_CHECK_EQUAL (s, i->second);
	}
}

BOOST_AUTO_TEST_CASE(to_upper_test)
{
	typedef map<string, string> Map;
	Map map;

	map[""      ] = "";
	map["BLAH"  ] = "BLAH";
	map["ao vivo jos�"] = "AO VIVO JOS�";
	map["Campeonato Paulista: Palmeiras x Corinthians AO VIVO"] = "CAMPEONATO PAULISTA: PALMEIRAS X CORINTHIANS AO VIVO";
    map["1 ao vivo2"] = "1 AO VIVO2";
	map["---"] = "---";

	//locale locBr("portuguese-brazilian");

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		cpp::to_upper (s);
		//cpp::to_upper (s, locBr);

		BOOST_CHECK_EQUAL (s, i->second);
	}
}

/*
BOOST_AUTO_TEST_CASE(to_upperw_test)
{
	typedef map<wstring, wstring> Map;
	Map map;

	map[L""      ] = L"";
	map[L"BLAH"  ] = L"BLAH";
	map[L"ao vivo jos�"] = L"AO VIVO JOS�";
	map[L"Campeonato Paulista: Palmeiras x Corinthians AO VIVO"] = L"CAMPEONATO PAULISTA: PALMEIRAS X CORINTHIANS AO VIVO";
    map[L"1 ao vivo2"] = L"1 AO VIVO2";
	map[L"---"] = L"---";

	//locale locBr("portuguese-brazilian");

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		wstring s = i->first;
		cpp::to_upper (s);
		//cpp::to_upper (s, locBr);

		BOOST_CHECK_EQUAL (s, i->second);
	}
}
*/

BOOST_AUTO_TEST_CASE(replace_test)
{
	typedef map<string, string> Map;
	Map map;

	map[""      ] = "";
	map["BLAH"  ] = "BLAH";
	map["ao orig jose"] = "ao replaced jose";
	map["Palmeiras x orig"] = "Palmeiras x replaced";
    map["orig2orig"] = "replaced2replaced";
	map["orig"] = "replaced";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		cpp::replace_substr (s, std::string("orig"), std::string("replaced"));

		BOOST_CHECK_EQUAL (s, i->second);
	}
	std::string s = "a  a";
	BOOST_CHECK(cpp::replace_substr (s, "  ", " "));
	BOOST_CHECK_EQUAL(s, "a a");

	s = "55 11  123-4567";
	while (cpp::replace_substr (s, "  ", " "));
	BOOST_CHECK_EQUAL(s, "55 11 123-4567");
}

BOOST_AUTO_TEST_CASE(to_string_unsigned_test)
{
	typedef map<unsigned, string> Map;
	Map map;

	map[0      ] = "0";
	map[1      ] = "1";
	map[123    ] = "123";
	map[1234   ] = "1,234";
    map[12345  ] = "12,345";
	map[123456 ] = "123,456";
	map[1234567] = "1,234,567";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		unsigned u = i->first;
		std::string s = cpp::to_string (u);

		BOOST_CHECK_EQUAL (s, i->second);
	}

    BOOST_CHECK_EQUAL ("1.234.567", cpp::to_string((unsigned)1234567, '.'));
}

BOOST_AUTO_TEST_CASE(to_string_int_test)
{
	typedef map<int, string> Map;
	Map map;

	map[0      ] = "0";
	map[-1     ] = "-1";
	map[123    ] = "123";
	map[-1234  ] = "-1,234";
    map[12345  ] = "12,345";
	map[-123456] = "-123,456";
	map[1234567] = "1,234,567";

	for (Map::iterator it = map.begin (); it != map.end (); ++it)
	{
		int i = it->first;
		std::string s = cpp::to_string ((int)i);

		BOOST_CHECK_EQUAL (s, it->second);
	}

    BOOST_CHECK_EQUAL ("1.234.567", cpp::to_string(1234567, '.'));
}

BOOST_AUTO_TEST_CASE(to_string_double_test)
{
	typedef map<double, string> Map;
	Map map;

	map[0            ] = "0.00";
	map[-1           ] = "-1.00";
	map[123.01       ] = "123.01";
	map[-1234.02     ] = "-1,234.02";
    map[12345.123    ] = "12,345.12";
	//map[-123456.1234 ] = "-123,456.1234";
	//map[1234567.00001] = "1,234,567.00001";

	for (Map::iterator it = map.begin (); it != map.end (); ++it)
	{
		double d = it->first;
		std::string s = cpp::to_string (d);

		BOOST_CHECK_EQUAL (s, it->second);
	}

	std::string s = cpp::to_string (12345.123, 3);
	BOOST_CHECK_EQUAL(s, "12,345.123");

    //BOOST_CHECK_EQUAL ("1.234.567,0000", cpp::to_string((double)1234567, 4, true, '.', ','));
}

BOOST_AUTO_TEST_CASE(remove_ext_test)
{
	typedef map<string, string> Map;
	Map map;

	map[""               ] = ""         ;
	map["blah"           ] = "blah"     ;
	map["fname.txt"      ] = "fname"    ;
	map["fname.txt.txt"  ] = "fname.txt";
	map["fname.extension"] = "fname"    ;
	map[".ext"           ] = ""         ;

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		cpp::remove_ext (s);

		BOOST_CHECK_EQUAL (s, i->second);
	}
}

BOOST_AUTO_TEST_SUITE_END()
