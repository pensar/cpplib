// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef STRING_DEF_HPP_INCLUDED
#define STRING_DEF_HPP_INCLUDED

#include <string>

namespace pensar_digital
{
	namespace cpplib
	{
		template<typename C = char>
		struct EmptyString 
		{
			inline static const C* value = "";
		};

		template<>
		struct EmptyString<wchar_t>
		{
			inline static const wchar_t* value = L"";
		};

		template <typename C = char>
		inline static const C* EMPTY = EmptyString<C>::value;

		template<typename C = char>
		struct CurrentDir
		{
			inline static const C* value = ".";
		};

		template<>
		struct CurrentDir<wchar_t>
		{
			inline static const wchar_t* value = L".";
		};

		// Define constant CURRENT_DIR.
		template <typename C = char>
		inline static const C* CURRENT_DIR = CurrentDir<C>::value;


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
