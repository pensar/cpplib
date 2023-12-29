// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef STRING_DEF_HPP_INCLUDED
#define STRING_DEF_HPP_INCLUDED

#include <string>

namespace pensar_digital
{
	namespace cpplib
	{
		using S = std::string;
		using SView = std::string_view;
		using SIter = std::string::iterator;
		using SConstIter = std::string::const_iterator;
		using WS = std::wstring;
		using WSView = std::wstring_view;
		using WSIter = std::wstring::iterator;
		using WSConstIter = std::wstring::const_iterator;
	}
}

#endif // STRING_DEF_HPP_INCLUDED
