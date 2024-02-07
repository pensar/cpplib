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
        
        inline static const bool ADD_NULL_AT_END              =  true;  ///< Add null character at the end of the string.
        inline static const bool DO_NOT_ADD_NULL_AT_END       = false;  ///< Do not add null character at the end of the string.
        inline static const bool FILL_NULL_BEFORE_COPY        =  true;  ///< Fill dest memory with null characters before copying the data.
        inline static const bool DO_NOT_FILL_NULL_BEFORE_COPY = false;  ///< Do not fill dest memory with null characters before copying the data.



        template<size_t MIN = 0, size_t MAX = 20, typename C = char> //, typename Encoding = icu::UnicodeString>
        class CS
        {
        public:
            typedef C value_type;
            std::array<C, MAX> data;
            bool case_sensitive = false;
            bool accent_sensitive = false;
            const C NULL_CHAR = (sizeof(C) == sizeof(char)) ? '\0' : L'\0';
            inline static const size_t MAX_SIZE   = MAX;
            inline static const size_t MAX_LENGTH = MAX - 1;
            inline static const size_t MIN_SIZE   = MIN;

            // Returns the size of the string.
            const constexpr inline size_t size() const noexcept
            {
                return MAX;
            }

            void inline fill(C c) noexcept
            {
                data.fill(c);
            }

            void fill_null() noexcept
            {
                fill(NULL_CHAR);
            }

            // Default constructor
            CS()
            {
                fill_null();
            }

            inline bool is_null_char(size_t index) const noexcept
            {
                return (data[index] == NULL_CHAR);
            }

            inline size_t length() const noexcept
            {
                return std::char_traits<C>::length(data.data());
            }

            inline void copy(const C* s, size_t s_length, bool add_null_at_end = true, bool fill_null_before_copy = true)
            {
                if (s_length >= MAX)
                {
                    std::string error = "String is too long. Max size is ";
                    error += std::to_string(MAX);
                    throw std::runtime_error(error);
                }
                if (s_length < MIN)
				{
					std::string error = "String is too short. Min size is ";
					error += std::to_string(MIN);
					throw std::runtime_error(error);
				}   
                if (fill_null_before_copy)
                    fill_null();

                if (s_length > 0)
                    std::memcpy(data.data(), s, s_length * sizeof(C));

                if (add_null_at_end and (s_length >= 0))
                    data[s_length] = NULL_CHAR;
            }

            inline void copy(const C* str)
            {
                auto strlen = std::char_traits<C>::length(str);
                copy(str, strlen);
            }

            inline void copy(const std::basic_string<C>& str)
            {
                copy(str.c_str(), str.length());
            }

            CS(const C* str)
            {
                copy(str);
            }

            CS(const std::basic_string<C>& str)
            {
                copy(str.c_str(), str.length());
            }

            // Converts to C*. Must be null terminated.
            operator C* () const noexcept
            {
                // Allocate memory for the new string.
                size_t size = length() + 1;
                C* c = new C[size];
                // Copy the string. With null termination.
                std::memcpy(c, data.data(), size);
                return c;
            }

            inline std::basic_string<C> to_string() const noexcept
            {
                return std::basic_string<C>(data.data());
            }

            // Converts to std::basic_string<C>.
            operator std::basic_string<C>() const noexcept
            {
                return to_string();
            }

            // Compare strings length.
            inline size_t cmp_strlen(const CS& other) const noexcept
            {
                return length() - other.length();
            }

            inline bool eq_strlen(const CS& other) const noexcept
            {
                return length() == other.length();
            }

            inline bool empty() const noexcept
            {
                return length() == 0;
            }

            // operator[]
            inline C& operator[] (const size_t index) const noexcept
            {
                // Removes const and returns C&.
                return const_cast<C&>(data[index]);
            }

            inline C& at(const size_t index) const noexcept
            {
                return operator[](index);
            }

            // Comparison operators
            bool operator== (const CS& other) const noexcept
            {
                bool result = eq_strlen(other);
                if (result)
                {
                    auto strlen = length();
                    for (size_t i = 0; i < strlen; ++i)
                    {
                        if (!equal(data[i], other.data[i], case_sensitive, accent_sensitive))
                        {
                            result = false;
                            break;
                        }
                    }
                }

                return result;
            }

            bool operator!=(const CS& other) const noexcept
            {
                return !(*this == other);
            }

            bool operator<(const CS& other) const noexcept
            {
                bool result = less(data[0], other.data[0], case_sensitive, accent_sensitive);
                if (result)
                {
                    auto strlen = length();
                    for (size_t i = 1; i < strlen; ++i)
                    {
                        if (!less(data[i], other.data[i], case_sensitive, accent_sensitive))
                        {
                            result = false;
                            break;
                        }
                    }
                }

                return result;
            }

            bool operator>(const CS& other) const noexcept
            {
                return other < *this;
            }

            bool operator<=(const CS& other) const noexcept
            {
                return !(other < *this);
            }

            bool operator>=(const CS& other) const noexcept
            {
                return !(*this < other);
            }

            // Assigns a std::basic_string.
            CS& operator= (const std::basic_string<C>& str) noexcept
            {
                copy(str);
                return *this;
            }

            // Assigns a null terminated string.
            CS& operator= (const C* str)
            {
                copy(str);
                return *this;
            }

            // Assigns a std::array.
            CS& operator= (const std::array<C, MAX>& arr) noexcept
            {
                std::memcpy(data.data(), arr.data(), MAX * sizeof(C));
                return *this;
            }

            CS& assign (const CS& other)
            {
                copy (other.data.data(), other.length (), ADD_NULL_AT_END);
                return *this;
            }

            CS& operator+= (const CS& other)
            {
                auto strlen = length();
                auto other_strlen = other.length();
                if (strlen + other_strlen > MAX)
                {
                    std::string error = "CString is too long. Max size is ";
                    error += std::to_string(MAX);
                    throw std::runtime_error(error);
                }
                std::memcpy(data.data() + strlen, other.data.data(), other_strlen * sizeof(C));
                data[strlen + other_strlen] = NULL_CHAR;
                return *this;
            }

            CS operator+ (const CS& other)
            {
                CS result = *this;
                result += other;
                return result;
            }

            // Makes CS compatible with NarrowOutuputStreamable concept.
            std::ostream& operator<< (std::ostream& os) noexcept
            {
                os << data.data();
                return os;
            }

            // Makes CS compatible with WideOutuputStreamable concept.
            std::wostream& operator<< (std::wostream& os) noexcept
            {
                os << data.data();
                return os;
            }
        };

        // Concatenates two CS objects. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<0, N + N2, Char> operator+ (const CS<0, N, Char>& lhs, const CS<0, N2, Char>& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const CS<N, Char>& rhs) - lhs must be of size N > 0");

            CS<0, N + N2, Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a std::basic_string. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<0, N + N2, Char> operator+ (const CS<0, N, Char>& lhs, const std::basic_string<Char>& rhs)
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::basic_string<Char>& rhs) - lhs must be of size N > 0");

            if (rhs.size() != N2)
            {
                throw std::runtime_error("CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::basic_string<Char>& rhs) - rhs must be of size N2");
            }

            CS<0, N + N2, Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a null terminated string. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<0, N + N2, Char> operator+ (const CS<0, N, Char>& lhs, const Char* rhs)
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const Char* rhs) - lhs must be of size N > 0");

            if (std::char_traits<Char>::length(rhs) != N2)
            {
                throw std::runtime_error("CS<N, Char> operator+ (const CS<N, Char>& lhs, const Char* rhs) - rhs must be of size N2");
            }
            CS<0, N + N2, Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs, rhs + N, result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a std::array. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<0, N + N2, Char> operator+ (const CS<0, N, Char>& lhs, const std::array<Char, N2>& rhs) noexcept
        {
            static_assert (N2 > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::array<Char, N2>& rhs) - rhs must be of size > 0");
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::array<Char, N2>& rhs) - lhs must be of size > 0");

            CS<0, N + N2, Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a std::basic_string and a CS object. Must be of same char type.
        template<int N, typename Char = char>
        CS<0, N + 1, Char> operator+ (const std::basic_string<Char>& lhs, const CS<0, N, Char>& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const std::basic_string<Char>& lhs, const CS<N, Char>& rhs) - rhs must be of size > 0");

            CS<0, N + 1, Char> result;
            std::copy(lhs.begin(), lhs.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a std::array and a CS object. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<0, N + N2, Char> operator+ (const std::array<Char, N>& lhs, const CS<0, N2, Char>& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const std::array<Char, N>& lhs, const CS<N2, Char>& rhs) - lhs must be of size > 0");

            CS<0, N + N2, Char> result;
            std::copy(lhs.begin(), lhs.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a char. Must be of same char type.
        template<int N, typename Char = char>
        CS<0, N + sizeof(Char), Char> operator+ (const CS<0, N, Char>& lhs, const Char& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const Char& rhs) - lhs must be of size > 0");

            CS<0, N + sizeof(Char), Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            result.data[N] = rhs;
            return result;
        }

        // Concatenates a char and a CS object. Must be of same char type.
        template<int N, typename Char = char>
        CS<0, N + sizeof(Char), Char> operator+ (const Char& lhs, const CS<0, N, Char>& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const Char& lhs, const CS<N, Char>& rhs) - lhs must be of size > 0");

            CS<0, N + sizeof(Char), Char> result;
            result.data[0] = lhs;
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + 1);
            return result;
        }

        // Assignment operator for std::array<C, N> with other std::array. returns a std::array<C, N>&.
        template<int N, typename C = char>
        std::array<C, N>& assign(std::array<C, N>& lhs, const std::array<C, N>& rhs) noexcept
        {
            std::memcpy(lhs.data(), rhs.data(), N);
            return lhs;
        }

        template <typename C = char>
        class S
        {
        public:
            typedef C value_type;

            inline static const size_t MAX_LENGTH = 100;
            inline static const size_t MAX_SIZE = MAX_LENGTH + 1;
            inline static const typename C NULL_CHAR = null_char<C>();

            std::array<C, MAX_SIZE> data;   ///< The string data.

            S(const C* s = empty<C> (), const size_t str_length = 0)
            {
                size_t l = (str_length == 0) ? std::char_traits<C>::length(s) : str_length;
                copy(s, l * sizeof(C), ADD_NULL_AT_END);
            }

            S(const std::basic_string<C>& s)
            {
                copy(s);
            }

            // Returns the length of the string.
            const constexpr inline size_t length() const noexcept
            {
                return std::char_traits<C>::length(data.data());
            }

            inline void copy(const C* s, const size_t size, bool add_null_at_end = true, bool fill_null_before_copy = true)
            {
                if (size > MAX_LENGTH)
                {
                    std::string error = "String is too long. Max size is ";
                    error += std::to_string(MAX_LENGTH);
                    throw std::runtime_error(error);
                }
                if (fill_null_before_copy)
                    fill_null();

                if (size > 0)
                    std::memcpy(data.data(), s, size * sizeof(C));

                if (add_null_at_end and (size >= 0))
                    data[size] = NULL_CHAR;
            }

            inline void copy(const C* s)
            {
                auto length = std::char_traits<C>::length(s);
                copy(s, length);
            }

            inline void copy(const std::basic_string<C>& s)
            {
                copy(s.c_str(), s.length());
            }

            inline bool empty() const noexcept
            {
                return length() == 0;
            }

            inline void fill(C c) noexcept
            {
                std::memset(data.data(), c, data.size());
            }

            void fill_null() noexcept
            {
                fill(NULL_CHAR);
            }

            inline bool is_null_char(size_t index) const noexcept
            {
                return (data[index] == NULL_CHAR);
            }

            inline std::basic_string<C> to_string() const noexcept
            {
                return std::basic_string<C>(data.data());
            }

            // Converts to const C*.
            operator const C* () const noexcept
            {
                return data.data();
            }

            // Converts to std::basic_string<C>.
            operator std::basic_string<C>() const noexcept
            {
                return to_string();
            }

            // Compare strings length.
            inline size_t cmp_strlen(const S& other) const noexcept
            {
                return length() - other.length();
            }

            inline bool eq_strlen(const S& other) const noexcept
            {
                return length() == other.length();
            }

            // operator[]
            inline C& operator[] (const size_t index) const noexcept
            {
                // Removes const and returns C&.
                return const_cast<C&>(data[index]);
            }

            inline C& at(const size_t index) const noexcept
            {
                return operator[](index);
            }

            inline bool raw_equal(const S& other, size_t size = 0) const noexcept
            {
                if (size == 0)
                    size = length();
                return (size == other.length()) ? (std::memcmp(data.data(), other.data(), size) == 0) : false;
            }

            inline size_t size() const noexcept
            {
                return data.size();
            }

            bool equal(const S& other, const bool case_sensitive = false, const bool accent_sensitive = false) const noexcept
            {
                bool result = eq_strlen(other);
                if (result)
                {
                    auto strlen = length();
                    for (size_t i = 0; i < strlen; ++i)
                    {
                        if (!pd::equal<C>(data[i], other.data[i], case_sensitive, accent_sensitive))
                        {
                            result = false;
                            break;
                        }
                    }
                }

                return result;
            }

            bool operator== (const S& other) const noexcept
            {
                return equal(other);
            }

            bool operator!=(const S& other) const noexcept
            {
                return !(*this == other);
            }

            bool less(const C& other, const bool case_sensitive = false, bool accent_sensitive = false) const noexcept
            {
                bool result = less(data[0], other.data[0], case_sensitive, accent_sensitive);
                if (result)
                {
                    auto strlen = length();
                    for (size_t i = 1; i < strlen; ++i)
                    {
                        if (!less(data[i], other.data[i], case_sensitive, accent_sensitive))
                        {
                            result = false;
                            break;
                        }
                    }
                }

                return result;
            }

            bool operator< (const S& other) const noexcept
            {
                return less(other);
            }

            bool operator> (const S& other) const noexcept
            {
                return other < *this;
            }

            bool operator<= (const S& other) const noexcept
            {
                return !(other > *this);
            }

            bool operator>= (const S& other) const noexcept
            {
                return !(*this < other);
            }

            // @brief Assigns a std::basic_string.
            S& operator= (const std::basic_string<C>& str) noexcept
            {
                copy(str);
                return *this;
            }

            /// @brief Assigns a null terminated string.
            S& operator= (const C* str)
            {
                copy(str);
                return *this;
            }

            S& operator += (const S& other)
            {
                size_t new_length = length() + other.length();
                if (new_length > MAX_LENGTH)
                    throw std::runtime_error("Strings add to > MAX_LENGTH");

                std::memcpy(data.data() + length(), other.data.data(), other.length() * sizeof(C));
                data[new_length] = NULL_CHAR;
                return *this;
            }

            S& operator += (const std::basic_string<C>& str)
            {
                if ((length() + str.length()) > MAX_LENGTH)
                    throw std::runtime_error("Strings add to > MAX_LENGTH");

                std::memcpy(data.data() + length(), str.c_str(), str.length() * sizeof(C));
                data[length() + str.length()] = NULL_CHAR;
                return *this;
            }

            S& operator += (const C* str)
            {
                auto length = std::char_traits<C>::length(str);
                if ((this->length() + length) > MAX_LENGTH)
                    throw std::runtime_error("Strings add to > MAX_LENGTH");

                std::memcpy(data.data() + this->length(), str, length * sizeof(C));
                data[this->length() + length] = NULL_CHAR;
                return *this;
            }

            S& operator += (const C c)
            {
                if ((length() + 1) > MAX_LENGTH)
                    throw std::runtime_error("Strings add to > MAX_LENGTH");

                data[length()] = c;
                data[length() + 1] = NULL_CHAR;
                return *this;
            }

        };

        // S stream functions.
        inline std::ostream& operator << (std::ostream& out, const S<char   >& s) { return out << s.to_string(); }
        inline std::wostream& operator << (std::wostream& out, const S<wchar_t>& s) { return out << s.to_string(); }
        inline std::istream& operator >> (std::istream& in, S<char   >& s) { std::string str; in >> str; s = str; return in; }
        inline std::wistream& operator >> (std::wistream& in, S<wchar_t>& s) { std::wstring str; in >> str; s = str; return in; }

        // Concatenates two S objects. Must be of same char type.
        template<typename C = char>
        S<C> operator+ (const S<C>& lhs, const S<C>& rhs) 
		{
            if ((lhs.length () + rhs.length ()) > S<>::MAX_LENGTH)
                throw std::runtime_error ("Strings add to > MAX_LENTGTH");

			S<C> result = lhs;
            result += rhs;
            return result;
		}

		// Concatenates an S object and a std::basic_string. Must be of same char type.
		template<typename C = char>
        S<C> operator+ (const S<C>& lhs, const std::basic_string<C>& rhs)
        {
            if ((lhs.length() + rhs.length()) > S<>::length ())
                throw std::runtime_error("Strings add to > MAX_LENTGTH");

            S<C> result = lhs;
            result += rhs;
            return result;
        }

        // Concatenates an S object and a null terminated string. Must be of same char type.
        template<typename C = char>
		S<C> operator+ (const S<C>& lhs, const C* rhs)
		{
			if ((lhs.length() + std::char_traits<C>::length(rhs)) > S<>::length ())
				throw std::runtime_error("Strings add to > MAX_LENTGTH");
			S<C> result = lhs;
            result += rhs;
            return result;
		}

		// Concatenates a std::basic_string and an S object. Must be of same char type.
        template<typename C = char>
        S<C> operator+ (const std::basic_string<C>& lhs, const S<C>& rhs)
        {
	        if ((lhs.length() + rhs.length()) > S<>::MAX_LENGTH)
		        throw std::runtime_error("Strings add to > MAX_LENGTH");
	        S<C> result = lhs;
	        result += rhs;
	        return result;  
        }

        // Concatenates a S object and a char. Must be of same char type.
        template<typename C = char>
		S<C> operator+ (const S<C>& lhs, const C& rhs)
		{
			if ((lhs.length() + 1) > S<>::MAX_LENGTH)
				throw std::runtime_error("Strings add to > MAX_LENGTH");
			S<C> result = lhs;
			result += rhs;
			return result;
		}

        // Concatenates a char and an S object. Must be of same char type.
        template<typename C = char>
        S<C> operator+ (const C& lhs, const S<C>& rhs)
        {
            if ((1 + rhs.length()) > S<>::MAX_LENGTH)
                throw std::runtime_error("Strings add to > MAX_LENGTH");
            S<C> result = lhs;
            result += rhs;
            return result;
        }
    }   // namespace cpplib
}       // namespace pensar_digital
#endif  // CS_HPP

