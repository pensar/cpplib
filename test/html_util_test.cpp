// $Id: html_util_test.cpp 40 2010-07-16 22:04:49Z mgpensar $

#ifdef VISUAL_STUDIO
#include "stdafx.h"
#endif

#include <boost/test/unit_test.hpp>

#include "html_util.h"
#include <map>
#include <string>

using namespace std;
namespace cpp = pensar_digital::cpplib;

BOOST_AUTO_TEST_SUITE(html_util_suite)

BOOST_AUTO_TEST_CASE(escape_unescapeHtml_test)
{
	typedef map<string, string, less<string> > Map;
	Map map;
	map[""      ] = "";
	map["blah"  ] = "blah";
	map["&"     ] = "&amp;";
	map[">"     ] = "&gt;";
	map["<"     ] = "&lt;";
	map["&amp;" ] = "&amp;amp;";
	map["&gt;"  ] = "&amp;gt;";
	map["&lt;"  ] = "&amp;lt;";
	map["<blah>"] = "&lt;blah&gt;";
	map["<&>"   ] = "&lt;&amp;&gt;";

	for (Map::iterator i = map.begin (); i != map.end (); ++i)
	{
		string s = i->first;
		string escaped   = cpp::escape_html (s);
		string unescaped = cpp::unescape_html (escaped);

		BOOST_CHECK_EQUAL (escaped, i->second);
		BOOST_CHECK_EQUAL (s      , unescaped);
	}
}

BOOST_AUTO_TEST_SUITE_END ()
