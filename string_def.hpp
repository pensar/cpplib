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
		struct PathSeparatorChar
		{
			inline static const C value = '\\';
		};

		template<>
		struct PathSeparatorChar<wchar_t>
		{
			inline static const wchar_t value = L'\\';
		};

		template <typename C = char>
		inline static const C PATH_SEPARATOR = PathSeparatorChar<C>::value;

		
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
		template <typename C = char>
		inline static C* EMPTY_CSTR = const_cast<char*>(EMPTY<C>);

		template<typename C = char>
		struct SpaceStruct
		{
			inline static const C value = ' ';
		};

		template<>
		struct SpaceStruct<wchar_t>
		{
			inline static const wchar_t value = L' ';
		};

		template <typename C = char>
		inline static const C SPACE = SpaceStruct<C>::value;

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

		template<typename C = char>
		struct DefaultTextFileExtension
		{
			inline static const C* value = ".txt";
		};

		template<>
		struct DefaultTextFileExtension<wchar_t>
		{
			inline static const wchar_t* value = L".txt";
		};

		template <typename C = char>
		inline static const C* DEFAULT_TXT_FILE_EXTENSION = DefaultTextFileExtension<C>::value;

		template<typename C = char>
		struct DefaultBinaryFileExtension
		{
			inline static const C* value = ".bin";
		};

		template<>
		struct DefaultBinaryFileExtension<wchar_t>
		{
			inline static const wchar_t* value = L".bin";
		};

		template <typename C = char>
		inline static const C* DEFAULT_BIN_FILE_EXTENSION = DefaultBinaryFileExtension<C>::value;
		
		template <typename C = char>
		struct NullCharStruct
		{
			inline static const C value = '\0';
		};

		template<>
		struct NullCharStruct<wchar_t>
		{
			inline static const wchar_t value = L'\0';
		};

		template <typename C = char>
		inline static const C null_char() {
			return NullCharStruct<C>::value;
		}

		using String = std::string;
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
