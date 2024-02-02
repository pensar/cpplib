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
            
        template <typename C = char>
        class S
        {
            public:
                std::array<C, 50> data;
                bool case_sensitive = false;
                bool accent_sensitive = false;
                S (C* s = EMPTY_CSTR<C>, size_t length = 1)
			    {
				    std::memcpy(data.data (), s, length * sizeof(C));
			    }   
                typedef C value_type;
                const C NULL_CHAR = null_char<C>();

                // Returns the length of the string.
                const constexpr inline size_t length() const noexcept
                {
                    return std::char_traits<C>::length(data);
                }

                inline bool empty() const noexcept
                {
                    return length() == 0;
                }
        };

        template<int N, typename C = char> //, typename Encoding = icu::UnicodeString>
        class CS
        {
            public:
                typedef C value_type;
                std::array<C, N> data;
                bool case_sensitive = false;
                bool accent_sensitive = false;
                const C NULL_CHAR = null_char<C>();

                // Returns the size of the string.
                const constexpr inline size_t size() const noexcept
                {
				    return N;
                }
            
                void inline fill (C c) noexcept
                {
				    data.fill (c);
			    }   

                void fill_null () noexcept
                {
                    fill (NULL_CHAR);
                }

                // Default constructor
                CS()
                {
                    fill_null ();
                }

                inline bool is_null_char (size_t index) const noexcept
                {
				    return (data[index] == NULL_CHAR);
			    }

                inline size_t length () const noexcept
                {
                    return std::char_traits<C>::length(data.data ());
			    }

                inline void copy(const C* str, size_t str_length)
                {
                    if (str_length > N)
                    {
                        std::string error = "String is too long. Max size is ";
                        error += std::to_string(N);
                        throw std::runtime_error(error);
                    }
                    //if (str == nullptr)
                    //{
                        fill_null();
                    //}
                    //else
                    //{
                        std::memcpy(data.data(), str, str_length * sizeof(C));
                        //data[str_length] = NULL_CHAR;
                    //}
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

                CS (const C* str)
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
                    size_t size = length () + 1;
                    C* c = new C[size];
                    // Copy the string. With null termination.
                    std::memcpy (c, data.data(), size);
                    return c;
			    }

                inline std::basic_string<C> to_string () const noexcept
				{
					return std::basic_string<C>(data.data());
				}

                // Converts to std::basic_string<C>.
                operator std::basic_string<C> () const noexcept
				{
					return to_string ();
				}

                // Compare strings length.
                inline size_t cmp_strlen(const CS& other) const noexcept
                {
                    return length () - other.length ();
                }

                inline bool eq_strlen(const CS& other) const noexcept
                {
				    return length () == other.length ();
			    }

                inline bool empty () const noexcept
                {
				    return length () == 0;
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
                        auto strlen = length ();
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
                    bool result = less (data[0], other.data[0], case_sensitive, accent_sensitive);
                    if (result)
                    {
                        auto strlen = length ();
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
                /*CS& operator= (const std::array<C, N>& arr) noexcept
			    {
                    std::memcpy(data.data(), arr.data(), N * sizeof(C));
				    return *this;
			    }*/
                
                CS& operator+= (const CS& other)
                {
					auto strlen = length ();
					auto other_strlen = other.length ();
                    if (strlen + other_strlen > N)
                    {
						std::string error = "CString is too long. Max size is ";
						error += std::to_string(N);
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
        CS<N + N2, Char> operator+ (const CS<N, Char>& lhs, const CS<N2, Char>& rhs) noexcept
		{
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const CS<N, Char>& rhs) - lhs must be of size N > 0");

			CS<N + N2, Char> result;
			std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
			std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
		}

		// Concatenates a CS object and a std::basic_string. Must be of same char type.
		template<int N, int N2, typename Char = char>
        CS<N + N2, Char> operator+ (const CS<N, Char>& lhs, const std::basic_string<Char>& rhs)
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::basic_string<Char>& rhs) - lhs must be of size N > 0");

            if (rhs.size() != N2)
			{
				throw std::runtime_error("CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::basic_string<Char>& rhs) - rhs must be of size N2");
			}

            CS<N + N2, Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a null terminated string. Must be of same char type.
        template<int N, int N2, typename Char = char>
		CS<N + N2, Char> operator+ (const CS<N, Char>& lhs, const Char* rhs)
		{
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const Char* rhs) - lhs must be of size N > 0");

            if (std::char_traits<Char>::length(rhs) != N2)
			{
				throw std::runtime_error("CS<N, Char> operator+ (const CS<N, Char>& lhs, const Char* rhs) - rhs must be of size N2");
			}
			CS<N + N2, Char> result;
			std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
			std::copy(rhs, rhs + N, result.data.begin() + N);
            return result;
		}

		// Concatenates a CS object and a std::array. Must be of same char type.
		template<int N, int N2, typename Char = char>
		CS<N + N2, Char> operator+ (const CS<N, Char>& lhs, const std::array<Char, N2>& rhs) noexcept
		{
            static_assert (N2 > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::array<Char, N2>& rhs) - rhs must be of size > 0");
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const std::array<Char, N2>& rhs) - lhs must be of size > 0");

			CS<N + N2, Char> result;
			std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
			std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
		}

		// Concatenates a std::basic_string and a CS object. Must be of same char type.
		template<int N, typename Char = char>
		CS<N + 1, Char> operator+ (const std::basic_string<Char>& lhs, const CS<N, Char>& rhs) noexcept
		{
            static_assert (N > 0, "CS<N, Char> operator+ (const std::basic_string<Char>& lhs, const CS<N, Char>& rhs) - rhs must be of size > 0");

			CS<N + 1, Char> result;
			std::copy(lhs.begin(), lhs.end(), result.data.begin());
			std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
		}

		// Concatenates a std::array and a CS object. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<N + N2, Char> operator+ (const std::array<Char, N>& lhs, const CS<N2, Char>& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const std::array<Char, N>& lhs, const CS<N2, Char>& rhs) - lhs must be of size > 0");

            CS<N + N2, Char> result;
            std::copy(lhs.begin(), lhs.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a char. Must be of same char type.
        template<int N, typename Char = char>
        CS<N + sizeof(Char), Char> operator+ (const CS<N, Char>& lhs, const Char& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const CS<N, Char>& lhs, const Char& rhs) - lhs must be of size > 0");

            CS<N + sizeof(Char), Char> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin()); 
                result.data[N] = rhs;
            return result;
        }

        // Concatenates a char and a CS object. Must be of same char type.
        template<int N, typename Char = char>
        CS<N + sizeof(Char), Char> operator+ (const Char& lhs, const CS<N, Char>& rhs) noexcept
        {
            static_assert (N > 0, "CS<N, Char> operator+ (const Char& lhs, const CS<N, Char>& rhs) - lhs must be of size > 0");

            CS<N + sizeof(Char), Char> result;
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
    }   // namespace cpplib
}       // namespace pensar_digital
#endif  // CS_HPP

