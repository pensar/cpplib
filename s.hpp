// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef S_HPP
#define S_HPP

#include "constant.hpp"
#include "string_util.hpp"
#include "concept.hpp"
#include <array>
#include <algorithm>
//#include <unicode/unistr.h> // ICU library

namespace pensar_digital
{
    namespace cpplib
    {
        extern void remove_accent (char* c) noexcept;
        extern void remove_accent (wchar_t* c) noexcept;

        template <typename C = char>
        C remove_accent(const C c) noexcept
        {
            C ch = c;
            remove_accent (&ch);
            return ch;
        }

        template <typename C = char>
        bool equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            if (case_sensitive)
            {
                if (accent_sensitive)
                {
                    return c == c2;
                }
                else
                {
                    return remove_accent<C>(c) == remove_accent<C>(c2);
                }
            }
            else
            {
                if (accent_sensitive)
                {
                    return std::tolower(c) == std::tolower(c2);
                }
                else
                {
                    return std::tolower(remove_accent<C>(c)) == std::tolower(remove_accent<C>(c2));
                }
            }
        }

        template <typename C = char>
        bool less (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            if (case_sensitive)
            {
                if (accent_sensitive)
                {
                    return c < c2;
                }
                else
                {
                    return remove_accent(c) < remove_accent(c2);
                }
            }
            else
            {
                if (accent_sensitive)
                {
                    return std::tolower(c) < std::tolower(c2);
                }
                else
                {
                    return std::tolower(remove_accent(c)) < std::tolower(remove_accent(c2));
                }
            }
        }

        template <typename C = char>
        bool not_equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return !equal(c, c2, case_sensitive, accent_sensitive);
        }

        template <typename C = char>
        bool greater (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return !less (c, c2, case_sensitive, accent_sensitive) && !equal(c, c2, case_sensitive, accent_sensitive);
        }

        template <typename C = char>
        bool less_equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return less (c, c2, case_sensitive, accent_sensitive) || equal(c, c2, case_sensitive, accent_sensitive);
        }

        template <typename C = char>
        bool greater_equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return greater (c, c2, case_sensitive, accent_sensitive) || equal (c, c2, case_sensitive, accent_sensitive);
        }

        template<int N, typename Char = char> //, typename Encoding = icu::UnicodeString>
        class S
        {
        public:
            std::array<Char, N> data;
            bool case_sensitive = false;
            bool accent_sensitive = false;

            // Default constructor
            S()
            {
                data.fill ('\0');
            }

            S (const Char* str)
			{
				std::copy (str, str + std::strlen (str), data.begin ());
			}

            // Converts to Char*. Must be null terminated.
            operator const Char* () const noexcept
			{
				return data.data();
			}

            // Comparison operators
            bool operator== (const S& other) const noexcept
            {
                return std::equal(data.begin(), data.end(), other.data.begin(),
                    [&](const Char a, const Char b) { return equal(a, b, case_sensitive, accent_sensitive); }
                );
            }

            bool operator!=(const S& other) const noexcept
            {
                return !(*this == other);
            }

            bool operator<(const S& other) const noexcept
            {
                return std::lexicographical_compare(data.begin(), data.end(), other.data.begin(),
                    [&](const Char a, const Char b) { return less(a, b, case_sensitive, accent_sensitive); }
                );
            }

            bool operator>(const S& other) const noexcept
            {
                return other < *this;
            }

            bool operator<=(const S& other) const noexcept
            {
                return !(other < *this);
            }

            bool operator>=(const S& other) const noexcept
            {
                return !(*this < other);
            }

            // Assigns a std::basic_string.
            S& operator= (const std::basic_string<Char>& str) noexcept
            {
                std::copy(str.begin(), str.end(), data.begin());
                return *this;
            }

            // Assigns a null terminated string.
            S& operator= (const Char* str) noexcept
            {
                std::copy(str, str + N, data.begin());
                return *this;
            }

            // Assigns a null terminated wstring.
            S& operator= (const wchar_t* str) noexcept
			{
				std::copy(str, str + N, data.begin());
				return *this;
			}

            // Assigns a std::array.
            S& operator= (const std::array<Char, N>& arr) noexcept
			{
				std::copy(arr.begin(), arr.end(), data.begin());
				return *this;
			}
        };

        // Concatenates two S objects. Must be of same char type.
        template<int N, int N2, typename Char = char>
        S<N + N2, Char> operator+ (const S<N, Char>& lhs, const S<N2, Char>& rhs) noexcept
		{
            static_assert (N > 0, "S<N, Char> operator+ (const S<N, Char>& lhs, const S<N, Char>& rhs) - lhs must be of size N > 0");

			S<N + N2, Char> result;
			std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
			std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
			return result;
		}

		// Concatenates a S object and a std::basic_string. Must be of same char type.
		template<int N, int N2, typename Char = char>
        S<N + N2, Char> operator+ (const S<N, Char>& lhs, const std::basic_string<Char>& rhs)
        {
            static_assert (N > 0, "S<N, Char> operator+ (const S<N, Char>& lhs, const std::basic_string<Char>& rhs) - lhs must be of size N > 0");

            if (rhs.size() != N2)
			{
				throw std::runtime_error("S<N, Char> operator+ (const S<N, Char>& lhs, const std::basic_string<Char>& rhs) - rhs must be of size N2");
			}

            S<N + N2, Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a S object and a null terminated string. Must be of same char type.
        template<int N, int N2, typename Char = char>
		S<N + N2, Char> operator+ (const S<N, Char>& lhs, const Char* rhs)
		{
            static_assert (N > 0, "S<N, Char> operator+ (const S<N, Char>& lhs, const Char* rhs) - lhs must be of size N > 0");

            if (std::char_traits<Char>::length(rhs) != N2)
			{
				throw std::runtime_error("S<N, Char> operator+ (const S<N, Char>& lhs, const Char* rhs) - rhs must be of size N2");
			}
			S<N + N2, Char> result;
			std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
			std::copy(rhs, rhs + N, result.data.begin() + N);
			return result;
		}

		// Concatenates a S object and a std::array. Must be of same char type.
		template<int N, int N2, typename Char = char>
		S<N + N2, Char> operator+ (const S<N, Char>& lhs, const std::array<Char, N2>& rhs) noexcept
		{
            static_assert (N2 > 0, "S<N, Char> operator+ (const S<N, Char>& lhs, const std::array<Char, N2>& rhs) - rhs must be of size > 0");
            static_assert (N > 0, "S<N, Char> operator+ (const S<N, Char>& lhs, const std::array<Char, N2>& rhs) - lhs must be of size > 0");

			S<N + N2, Char> result;
			std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
			std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
			return result;
		}

		// Concatenates a std::basic_string and a S object. Must be of same char type.
		template<int N, typename Char = char>
		S<N + 1, Char> operator+ (const std::basic_string<Char>& lhs, const S<N, Char>& rhs) noexcept
		{
            static_assert (N > 0, "S<N, Char> operator+ (const std::basic_string<Char>& lhs, const S<N, Char>& rhs) - rhs must be of size > 0");

			S<N + 1, Char> result;
			std::copy(lhs.begin(), lhs.end(), result.data.begin());
			std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
			return result;
		}

		// Concatenates a std::array and a S object. Must be of same char type.
        template<int N, int N2, typename Char = char>
        S<N + N2, Char> operator+ (const std::array<Char, N>& lhs, const S<N2, Char>& rhs) noexcept
        {
            static_assert (N > 0, "S<N, Char> operator+ (const std::array<Char, N>& lhs, const S<N2, Char>& rhs) - lhs must be of size > 0");

            S<N + N2, Char> result;
            std::copy(lhs.begin(), lhs.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a S object and a char. Must be of same char type.
        template<int N, typename Char = char>
        S<N + sizeof(Char), Char> operator+ (const S<N, Char>& lhs, const Char& rhs) noexcept
        {
            static_assert (N > 0, "S<N, Char> operator+ (const S<N, Char>& lhs, const Char& rhs) - lhs must be of size > 0");

            S<N + sizeof(Char), Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin()); 
                result.data[N] = rhs;
            return result;
        }

        // Concatenates a char and a S object. Must be of same char type.
        template<int N, typename Char = char>
        S<N + sizeof(Char), Char> operator+ (const Char& lhs, const S<N, Char>& rhs) noexcept
        {
            static_assert (N > 0, "S<N, Char> operator+ (const Char& lhs, const S<N, Char>& rhs) - lhs must be of size > 0");

            S<N + sizeof(Char), Char> result;
            result.data[0] = lhs;
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + 1);
            return result;
        }

    }   // namespace cpplib
}       // namespace pensar_digital
#endif  // S_HPP

