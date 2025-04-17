// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef S_HPP
#define S_HPP

#include "constant.hpp"
#include "string_def.hpp"

#include "concept.hpp"


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
        using InFStream        = std::basic_ifstream<C>;
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
        

        inline void runtime_error(const S& message)
        {
            #ifdef WIDE_CHAR
                throw std::runtime_error(to_string(message));
            #else
                throw std::runtime_error(message);
            #endif
        }

        
        // Based on #ifdef WIDE_CHAR defines a constexpr function returning __FILE__ as a wide string or a narrow string.
        inline static constexpr S sfile () 
        {
            #ifdef WIDE_CHAR
                        return __FILEW__;
            #else
                        return __FILE__;
            #endif
        }

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


    }   // namespace cpplib
}       // namespace pensar_digital
#endif  // S_HPP

