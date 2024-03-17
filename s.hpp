// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef S_HPP
#define S_HPP

#include "constant.hpp"
#include "concept.hpp"
#include "string_def.hpp"

#include <array>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>  // setprecision

//#include <unicode/unistr.h> // ICU library

namespace pensar_digital
{
    namespace cpplib
    {

        inline static const C* EMPTY = W("");
        inline static const C  SPACE = W(' ');
        inline static const C* CURRENT_DIR = W(".");
        inline static const C* DEFAULT_TXT_FILE_EXTENSION = W(".txt");
        inline static const C* DEFAULT_BIN_FILE_EXTENSION = W(".bin");
        inline static const C* DOUBLE_QUOTES = W("\"");

        inline static const C NULL_CHAR = W('\0');

        using S                = std::basic_string<C>;
        using SView            = std::basic_string_view<C>;
        using SIter            = std::basic_string<C>::iterator;
        using SConstIter       = std::basic_string<C>::const_iterator;
        using InStream         = std::basic_istream<C>;
        using OutStream        = std::basic_ostream<C>;
        using SStream          = std::basic_stringstream<C>;
        using InStreamBuf      = std::basic_streambuf<C>;
		using OutStreamBuf     = std::basic_streambuf<C>;
        using InStreamBufIter  = std::istreambuf_iterator<C>;
		using OutStreamBufIter = std::ostreambuf_iterator<C>;
        using InStringStream   = std::basic_istringstream<C>;
		using OutStringStream  = std::basic_ostringstream<C>;
        using InFileStream     = std::basic_ifstream<C>;
        using OutFStream       = std::basic_ofstream<C>;
        using FStream          = std::basic_fstream<C>;
        using InFileBuf        = std::basic_filebuf<C>;
        using InFileBufIter    = std::istreambuf_iterator<C>;
        using OutFileBuf       = std::basic_filebuf<C>;
        using OutFileBufIter   = std::ostreambuf_iterator<C>;

        extern const bool PAD_RIGHT;
        extern const bool PAD_LEFT;
        extern const bool TRIM_ELEMENTS;
        extern const bool INCLUDE_EMPTY_FIELDS;

        extern std::wstring to_wstring(const std::string& s);
        extern std::string  to_string (const std::wstring& s);

        // Reads all chars from a istream into a string.
        // \param is the input stream.
        // \param s the string to hold the result.
        inline S& read_all(InStream& is, S& s)
        {
            C c;
            while (is.get(c))
                s += c;
            return s;
        }

        static inline S& rtrim(S& s)
        {
            #ifdef WIDE_CHAR    
                s.erase(std::find_if_not(s.rbegin(), s.rend(), [](wchar_t c) {return iswspace(c); }).base(), s.end());
            #else
                s.erase(std::find_if_not(s.rbegin(), s.rend(), [](unsigned char c) {return iswspace(c); }).base(), s.end());
            #endif
            return s;
         }

        static inline S& ltrim(S& s)
		{
			#ifdef WIDE_CHAR    
				s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), [](wchar_t c) {return iswspace(c); }));
			#else
				s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), [](unsigned char c) {return iswspace(c); }));
			#endif
			return s;
		}

        static inline void trim(S& s)
        {
            ltrim(rtrim(s));
        }

        static inline S trim(const S& s)
        {
            S t = s;
            return ltrim(rtrim(t));
        }

        /// An auto_ptr to a vector of strings.
        typedef std::unique_ptr<std::vector<S>> SVectorPtr;

        /// Breaks the string s using the character c as a separator and puts the results in a vector.
        /// \param s the string to be splitted.
        /// \param c the separator char.
        /// \param it the iterator of the container to hold the results.
        /// \param trim_elements if true, split will trim the elements before adding them to vector v.
        template<typename Container = std::vector<S>>
        inline void split(const S& s, C ch,
            Container& c, bool trim_elements = true, bool include_empty_fields = false)//, const std::locale& loc=std::locale())
        {
            size_t i = 0;
            size_t j = s.find(ch);

            if (j == S::npos)
                c.insert(c.end(), s);
            else
                while (j != S::npos)
                {
                    S aux = s.substr(i, j - i);
                    if (trim_elements)
                        trim(aux);
                    if (include_empty_fields || aux.length() > 0)
                        c.insert(c.end(), aux);
                    i = ++j;
                    j = s.find(ch, j);

                    if (j == S::npos)
                    {
                        aux = s.substr(i, s.length());
                        if (trim_elements)
                            trim(aux);
                        if (include_empty_fields || aux.length() > 0)
                            c.insert(c.end(), aux);
                    }
                }
        }

        /// Breaks the string s using the character c as a separator and puts the results in a vector.
        /// Examples: \ref string_util_example.cpp
        /// \param s the string to be splitted.
        /// \param c the separator char.
        /// \param v the vector of strings holding the results.
        /// \param trim_elements if true, split will trim the elements before adding them to vector v.
        /*void split(const S& s, C c, std::vector<S>& v,
            bool trim_elements = true, bool include_empty_fields = false)//, const std::locale& loc=std::locale())
        {
            size_t i = 0;
            size_t j = s.find(c);

            if (j == S::npos)
                v.push_back(s);
            else
                while (j != S::npos)
                {
                    S aux = s.substr(i, j - i);
                    if (trim_elements)
                        trim(aux);
                    if (include_empty_fields || aux.length() > 0)
                        v.push_back(aux);
                    i = ++j;
                    j = s.find(c, j);

                    if (j == S::npos)
                    {
                        aux = s.substr(i, s.length());
                        if (trim_elements)
                            trim(aux);
                        if (include_empty_fields || aux.length() > 0)
                            v.push_back(aux);
                    }
                }
        }*/

        /// Breaks the string s using the character c as a separator and puts the results in a vector.
        /// Examples: \ref string_util_example.cpp
        /// \param s the string to be splitted.
        /// \param c the separator char.
        /// \param trim_elements if true, split will trim the elements before adding them to vector v.
        /// \result An auto_ptr to a vector of strings holding the results.
        /*
        template<typename CharType>
        std::auto_ptr<std::vector<std::basic_string<CharType> > > split (const std::basic_string<CharType>& s, CharType c, bool trim_elements = true)
        {
            std::vector<std::basic_string<CharType> > *vp = new std::vector<std::basic_string<CharType> > ();
            split<CharType> (s, c, *vp, trim_elements);
            return std::auto_ptr<std::vector<std::basic_string<CharType> > > (vp);
        }
        */

        /// \copydoc template<typename CharType> void split(const std::basic_string<CharType>&, CharType, std::vector<std::basic_string<CharType> >*, bool)
        /*inline void split(S& s, C c, std::vector<S>& v, bool trim_elements = true)
        {
            split(s, c, v, trim_elements);
        }*/

        /*
        /// \copydoc template<typename CharType> std::auto_ptr<std::vector<std::basic_string<CharType> > > split (const std::basic_string<CharType>&, CharType, bool)
        inline std::auto_ptr<std::vector<std::string> > split (const std::string& s, char c, bool trim_elements = true)
        {
            return split<char> (s, c, trim_elements);
        }
        */


#ifdef CODE_GEAR
       // extern void        __fastcall split(const AnsiString& as, char c, std::vector<AnsiString>* v, bool trim_elements = true);
        //extern AnsiString  __fastcall only_digits(const AnsiString& s);
        //extern AnsiString  __fastcall remove_acentos(const AnsiString& s);
        //extern AnsiString format_cpf(AnsiString cpf);
#endif

        /// Pads a string to the <b>right</b> with the given character till it reaches the given size.<p>
        /// Important: negative values are not allowed for parameter n. An assertion will end the program if
        /// that happens.
        /// If length is less than s.length () it does nothing, returns the same string.<br>
        /// \param s the string to be padded.
        /// \param c the character to be used to pad the string.
        /// \param n the number of characters the string needs to be padded.
        inline void pad(S& s, const C c, const typename S::size_type n, bool where = PAD_RIGHT)
        {
            if (n == 0)
                return;
            // This is a protection against negative numbers being passed and automatically converted to huge unsigned numbers.
            // Unfortunately most compilers do not warn about it.
            assert(static_cast<int>(n) > 0);

            const typename S::size_type length = s.length();
            if (where == PAD_RIGHT)
            {
                if (n > length)
                    s.append(n - length, c);
            }
            else
            {
                if (n > length)
                    s.insert(0, n - length, c);
            }
        }

        /// \copydoc pad_copy(S&, const T, const typename S::size_type)
        /// \return padded string.
        inline S pad_copy (const S& s, const C c, const typename S::size_type n, bool where = PAD_RIGHT)
        {
            S aux(s);
            pad(aux, c, n, where);
            return aux;
        }

        /// \copydoc pad_copy(S&, const T, const typename S::size_type)
        /// \return Padded string.
        inline S pad_copy(const C* s, const C c, const unsigned n, bool where = PAD_RIGHT)
        {
            assert(s != 0);

            S aux = s;
            pad(aux, c, n, where);
            return aux;
        }

        /// Removes all characters that are not digits from string.
        inline S only_digits(const S& s)
        {
            S out;
            for (const auto& ch : s)
            {
                if (isdigit(ch))
                    out += ch;
            }

            return out;
        }

        //template<typename T = int>
        //struct IsAlphaNumeric : std::unary_function<T,bool>
        //{
        //    bool operator() (const T& ch) const {return isalnum (ch);}
        //};


        /// Removes all characters that are not alpha-numeric from string.
        //template<typename T>
       // void only_alpha_numeric_inplace (S& s)
       // {
        //    if (! s.size ()) return;
        //    std::remove_if (s.begin (), s.end (), std::not1(IsAlphaNumeric<T> ()));
       // }

        /// Removes all characters that are not alpha-numeric from string.
        inline S only_alpha_numeric(const S& s)
        {
            S out;
            for (const C& ch : s)
            {
                if (isalnum(ch))
                    out += ch;
            }

            //std::string out = s;
            //only_alpha_numeric_inplace (out);
            ////std::remove_copy_if (s.begin (), s.end (), out.begin (), std::not1(IsAlphaNumeric<T> ()));
            return out;
        }

        extern void troca_char(C* c, const std::locale& loc = std::locale());


        /// Remove accents from string s (replacing for example ã for a).
        inline void remove_accents(S& s)
        {
            for (C& ch : s)
            {
                troca_char(&ch);
            }
        }

        inline S no_accents(const S& s)
        {
            S r = s;
            remove_accents(r);
            return r;
        }

        template <class CharT = char>
        inline bool is_space(CharT c)//, const std::locale& loc = std::locale())
        {
            return std::isspace<CharT>(c, std::locale());//, loc);
        }

        /// Remove blanks from string.
        /// ' '	(0x20)	space (SPC)
        /// '\t'	(0x09)	horizontal tab (TAB)
        /// '\n'	(0x0a)	newline (LF)
        /// '\v'	(0x0b)	vertical tab (VT)
        /// '\f'	(0x0c)	feed (FF)
        /// '\r'	(0x0d)	carriage return (CR)
        template<typename T = char>
        inline void remove_blanks(S& s)
        {
            s.erase(std::remove_if(s.begin(), s.end(), is_space<T>), s.end());
        }

        /// Remove accents from string s (replacing for example ã for a).
        template<typename T = char>
        inline S copy_remove_accents(S s)
        {
            remove_accents(s);
            return s;
        }

        /// Remove all occurrences of string s from target.
        template<typename T = char>
        inline void remove(const S& s, S& target)
        {
            typename S::size_type pos = target.find(s);
            while (pos != S::npos)
            {
                target.erase(pos, s.length());
                pos = target.find(s);
            }
        }

        /// Remove delimiter char from string.
        template<typename T = char>
        inline void remove_delimiters(T delimiter, S& s)
        {
            if (s[0] == delimiter && s[s.length() - 1] == delimiter)
            {
                s.erase(0, sizeof(T));
                s.erase(s.length() - 1, sizeof(T));
            }
        }

        /// Remove all occurrences of string s from target.
        template<typename T = char>
        inline S copy_remove(const S& s, S target)
        {
            remove(s, target);
            return target;
        }

        /*
        //This code failed to convert accented chars.
        void to_upper(std::basic_string<char>& s);
        void to_upper(std::basic_string<wchar_t>& s);
        void to_lower(std::basic_string<char>& s);
        void to_lower(std::basic_string<wchar_t>& s);
        */

        //This code failed to convert accented chars because the locale implementation does not support it.
        /*
        template<typename C>
        void to_upper (S& s, const std::locale& loc = std::locale ())
        {
            typename S::iterator p;
            for (p = s.begin (); p != s.end (); ++p)
            {
                *p = std::use_facet<std::ctype<C> >(loc).toupper (*p);
            }
        }

        template<typename C = char>
        void to_lower(S& s, const std::locale& loc = std::locale ())
        {
            typename S::iterator p;
            for (p = s.begin (); p != s.end (); ++p)
            {
                *p = std::use_facet<std::ctype<C> >(loc).tolower (*p);
            }
        }
        */

        extern std::string  reverse(const std::string& s);
        extern std::wstring reverse(const std::wstring& s);

        extern void to_upper(std::string& s);
        extern void to_upper(std::wstring& s);
        extern void to_lower(std::string& s);
        extern void to_lower(std::wstring& s);

        extern std::string upper(const std::string& s);
        extern std::string lower(const std::string& s);

        /// Removes all instances of substring p from s.
        template<typename C = char>
        inline void remove_substr(S& s, const S& p)
        {
            std::string::size_type n = p.length();
            for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p))
                s.erase(i, n);
        }

        /// Removes all instances of substring p from s.
        // o: original substring
        // r: replacement
        inline bool replace_substr(S& s, const S& o, const S& r)
        {
            if (s.size() == 0)
                return false;
            bool replaced = false;
            S::size_type n = o.length();
            for (S::size_type i = s.find(o); i != S::npos; i = s.find(o))
            {
                s.replace(i, n, r);
                replaced = true;
            }
            return replaced;
        }

        inline bool replace_substr (S& s, const C* o, const C* r)
        {
            return replace_substr (s, S(o), S(r));
        }

        inline S insert_grouping_char (const S& s, typename S::value_type grouping_char = W(','))
        {
            S f; //formatted string.
            unsigned primeiro = s.length() % 3;
            if (primeiro == 0)
                primeiro = 3;
            --primeiro;
            unsigned i = 0;
            unsigned count = 0;
            for (typename S::const_iterator it = s.begin(); it != s.end(); ++it)
            {
                f += *it;
                bool last = (++it == s.end()) ? true : false;
                --it;
                if ((i++ == primeiro || ++count == 3) && !last)
                {
                    f += grouping_char;
                    count = 0;
                }
            }
            return f;
        }

        template<typename IntType = int, bool use_grouping_char = false>
        inline S to_string(IntType number, C grouping_char = W(','))
        {
            OutStringStream ss;
            ss << number;
            S s = ss.str();
            //if (number < 0) inserts a minus sign at the beginning of the string.
            if (number < 0)
                s.insert(s.begin(), W('-'));

            return use_grouping_char ? insert_grouping_char (s, grouping_char) : s;
        }

        template<bool use_grouping_char = false>
        inline S to_string(size_t number, typename C grouping_char = W(','))
        {
            return to_string <size_t, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        inline S to_string(int number, C grouping_char = W(','))
        {
            return to_string <int, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        inline S to_string(long number, C grouping_char = W(','))
        {
            return to_string <long, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        inline S to_string(long long int number, C grouping_char = W(','))
        {
            return to_string <long long int, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        inline S to_string(unsigned int number, C grouping_char = W(','))
        {
            return to_string <unsigned int, use_grouping_char, C>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        inline S to_string(unsigned long int number, C grouping_char = W(','))
        {
            return to_string <unsigned long int, use_grouping_char>(number, grouping_char);
        }

        //template<bool use_grouping_char = false>
        //S to_string(unsigned long long int number, typename String::value_type grouping_char = W(','))
        //{
        //    return to_string <unsigned long long int, use_grouping_char>(number, grouping_char);
       // }

        // todo: use use_grouping.
        inline S to_string(double number, unsigned num_decimals /*= 2*/, bool use_grouping/* = true*/, C grouping_char = W(','), C decimal_separator = W(','))
        {
            long long int integer_part = (long long) trunc (number);
            S s = to_string (integer_part);
            SStream ss;
            ss << std::fixed << std::setprecision(num_decimals) << number;
            S s1 = ss.str();
            typename S::size_type pos = s1.find(decimal_separator);
            S decimal_part = W(".00");
            if (pos != S::npos)
            {
                size_t n = (num_decimals + pos) < s1.length() ? num_decimals : s1.length();
                decimal_part = s1.substr(pos, n + 1);
            }
            pad(decimal_part, W('0'), num_decimals);
            return s + decimal_part;
        }

        inline S pad_left0(long long int number, const unsigned n = 4)
        {
            return pad_copy(to_string<false>(number).c_str(), W('0'), n, PAD_LEFT);
        }

        /// Remove extension from file name.
        inline void remove_ext(S& fname)
        {
            typename S::size_type pos = fname.find_last_of(W('.'));
            if (pos != S::npos)
            {
                fname.erase(pos, fname.length());
            }
        }

        /// Remove todos espaços duplos.
        inline S& remove_double_spaces(S& s)
        {
            if (s.size() == 0)
                return s;
            while (replace_substr (s, W("  "), W(" ")));
            return s;
        }

        //---------------------------------------------------------
        extern void remove_accent (C* c) noexcept;

        inline C remove_accent(const C c) noexcept
        {
            C ch = c;
            remove_accent (&ch);
            return ch;
        }

        inline bool equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            if (case_sensitive)
            {
                if (accent_sensitive)
                {
                    return c == c2;
                }
                else
                {
                    return remove_accent (c) == remove_accent (c2);
                }
            }
            else
            {
                if (accent_sensitive)
                {
                    return std::tolower (c) == std::tolower (c2);
                }
                else
                {
                    return std::tolower (remove_accent (c)) == std::tolower (remove_accent (c2));
                }
            }
        }

        inline bool less (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            if (case_sensitive)
            {
                if (accent_sensitive)
                {
                    return c < c2;
                }
                else
                {
                    return remove_accent (c) < remove_accent (c2);
                }
            }
            else
            {
                if (accent_sensitive)
                {
                    return std::tolower (c) < std::tolower (c2);
                }
                else
                {
                    return std::tolower (remove_accent (c)) < std::tolower (remove_accent (c2));
                }
            }
        }

        inline bool not_equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return !equal(c, c2, case_sensitive, accent_sensitive);
        }

        template <typename C = char>
        inline bool greater (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return !less (c, c2, case_sensitive, accent_sensitive) && !equal (c, c2, case_sensitive, accent_sensitive);
        }

        inline bool less_equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return less (c, c2, case_sensitive, accent_sensitive) || equal (c, c2, case_sensitive, accent_sensitive);
        }

        inline bool greater_equal (const C c, const C c2, bool case_sensitive = false, bool accent_sensitive = false) noexcept
        {
            return greater (c, c2, case_sensitive, accent_sensitive) || equal (c, c2, case_sensitive, accent_sensitive);
        }
        
        inline static const bool ADD_NULL_AT_END              =  true;  ///< Add null character at the end of the string.
        inline static const bool DO_NOT_ADD_NULL_AT_END       = false;  ///< Do not add null character at the end of the string.
        inline static const bool FILL_NULL_BEFORE_COPY        =  true;  ///< Fill dest memory with null characters before copying the data.
        inline static const bool DO_NOT_FILL_NULL_BEFORE_COPY = false;  ///< Do not fill dest memory with null characters before copying the data.



        template<size_t MIN = 0, size_t MAX = 20> //, typename Encoding = icu::UnicodeString>
        class CS
        {
            public:
                typedef C value_type;
                std::array<C, MAX> data;
                bool case_sensitive = false;
                bool accent_sensitive = false;
                inline static const size_t MAX_SIZE   = MAX;
                inline static const size_t MAX_LENGTH = MAX - 1;
                inline static const size_t MIN_SIZE   = MIN;

                // Returns the size of the string.
                const constexpr inline size_t size() const noexcept
                {
                    return MAX;
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

                inline void copy(const S& str)
                {
                    copy(str.c_str(), str.length());
                }

                CS(const C* str)
                {
                    copy(str);
                }

                CS(const S& str)
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

                inline S to_string() const noexcept
                {
                    return S(data.data());
                }

                inline S str () const noexcept
			    {
				    return S(data.data());
			    }

                // Converts to S.
                operator S() const noexcept
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
            CS& operator= (const S& str) noexcept
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
            OutStream& operator<< (OutStream& os) noexcept
            {
                os << data.data();
                return os;
            }
        };

        // Concatenates two CS objects. Must be of same char type.
        template<int N, int N2>
        CS<0, N + N2> operator+ (const CS<0, N>& lhs, const CS<0, N2>& rhs) noexcept
        {
            static_assert (N > 0, W("CS<N> operator+ (const CS<N>& lhs, const CS<N>& rhs) - lhs must be of size N > 0"));

            CS<0, N + N2> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a std::basic_string. Must be of same char type.
        template<int N, int N2, typename Char = char>
        CS<0, N + N2> operator+ (const CS<0, N>& lhs, const S& rhs)
        {
            static_assert (N > 0, W("CS<N> operator+ (const CS<N>& lhs, const S& rhs) - lhs must be of size N > 0"));

            if (rhs.size() != N2)
            {
                throw std::runtime_error (W("CS<N> operator+ (const CS<N>& lhs, const std::basic_string<Char>& rhs) - rhs must be of size N2"));
            }

            CS<0, N + N2> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a null terminated string. Must be of same char type.
        template<int N, int N2>
        CS<0, N + N2> operator+ (const CS<0, N>& lhs, const C* rhs)
        {
            static_assert (N > 0, W("CS<N> operator+ (const CS<N>& lhs, const Char* rhs) - lhs must be of size N > 0"));

            if (std::char_traits<C>::length(rhs) != N2)
            {
                throw std::runtime_error (W("CS<N> operator+ (const CS<N>& lhs, const Char* rhs) - rhs must be of size N2"));
            }
            CS<0, N + N2> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs, rhs + N, result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a std::array. Must be of same char type.
        template<int N, size_t N2>
        CS<0, N + N2> operator+ (const CS<0, N>& lhs, const std::array<C, N2>& rhs) noexcept
        {
            static_assert (N2 > 0, W("CS<N> operator+ (const CS<N>& lhs, const std::array<Char, N2>& rhs) - rhs must be of size > 0"));
            static_assert (N > 0, W("CS<N> operator+ (const CS<N>& lhs, const std::array<Char, N2>& rhs) - lhs must be of size > 0"));

            CS<0, N + N2> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            std::copy(rhs.begin(), rhs.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates an S string and a CS object. Must be of same char type.
        template<size_t N>
        CS<0, N + 1> operator+ (const S& lhs, const CS<0, N>& rhs) 
        {
            size_t min_size = lhs.length() + rhs.length() + 1;
            if (N < min_size)
                throw std::runtime_error (W("CS<N> operator+ (const std::basic_string<Char>& lhs, const CS<N>& rhs) - rhs must be of size > ") + pd::to_string (min_size));
            CS<0, N + 1> result;
            std::copy (lhs.begin (), lhs.end (), result.data.begin ());
            std::copy (rhs.data.begin (), rhs.data.begin () + rhs.length (), result.data.begin () + lhs.length ());
            return result;
        }

        // Concatenates a std::array and a CS object. Must be of same char type.
        template<int N, int N2>
        CS<0, N + N2> operator+ (const std::array<C, N>& lhs, const CS<0, N2>& rhs) noexcept
        {
            static_assert (N > 0, W("CS<N> operator+ (const std::array<Char, N>& lhs, const CS<N2>& rhs) - lhs must be of size > 0"));

            CS<0, N + N2> result;
            std::copy(lhs.begin(), lhs.end(), result.data.begin());
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + N);
            return result;
        }

        // Concatenates a CS object and a char. Must be of same char type.
        template<int N>
        CS<0, N + sizeof(C)> operator+ (const CS<0, N>& lhs, const C& rhs) noexcept
        {
            static_assert (N > 0, W("CS<N> operator+ (const CS<N>& lhs, const Char& rhs) - lhs must be of size > 0"));

            CS<0, N + sizeof(C)> result;
            std::copy(lhs.data.begin(), lhs.data.end(), result.data.begin());
            result.data[N] = rhs;
            return result;
        }

        // Concatenates a char and a CS object. Must be of same char type.
        template<int N>
        CS<0, N + sizeof(C)> operator+ (const C& lhs, const CS<0, N>& rhs) noexcept
        {
            static_assert (N > 0, W("CS<N> operator+ (const Char& lhs, const CS<N>& rhs) - lhs must be of size > 0"));

            CS<0, N + sizeof(C)> result;
            result.data[0] = lhs;
            std::copy(rhs.data.begin(), rhs.data.end(), result.data.begin() + 1);
            return result;
        }

        // Assignment operator for std::array<C, N> with other std::array. returns a std::array<C, N>&.
        template<int N>
        std::array<C, N>& assign(std::array<C, N>& lhs, const std::array<C, N>& rhs) noexcept
        {
            std::memcpy(lhs.data(), rhs.data(), N);
            return lhs;
        }

        /*
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

            S(const S& s)
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

            inline void copy(const S& s)
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

            inline S to_string() const noexcept
            {
                return S(data.data());
            }

            // Converts to const C*.
            operator const C* () const noexcept
            {
                return data.data();
            }

            // Converts to S.
            operator S() const noexcept
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
            S& operator= (const S& str) noexcept
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

            S& operator += (const S& str)
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
        S<C> operator+ (const S<C>& lhs, const S& rhs)
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
        S<C> operator+ (const S& lhs, const S<C>& rhs)
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
        }*/
    }   // namespace cpplib
}       // namespace pensar_digital
#endif  // S_HPP

