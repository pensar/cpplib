// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)


#ifndef string_utilH
#define string_utilH

#ifdef CODE_GEAR
#include <vcl.h>
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <locale>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <cwctype>
#include <memory>
#include <sstream>
#include <cassert>

#include "log.hpp"
#include "string_def.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        extern const bool PAD_RIGHT;
        extern const bool PAD_LEFT ;
        extern const bool TRIM_ELEMENTS;
        extern const bool INCLUDE_EMPTY_FIELDS;

        // Reads all chars from a istream into a string.
        // \param is the input stream.
        // \param s the string to hold the result.
        inline String& read_all (std::basic_istream<String::value_type>& is, String& s)
        {
			String::value_type c;
			while (is.get (c))
				s += c;
            return s;
		}
        
        //std::string str((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());

        
        template<typename T>
        static inline std::basic_string<T>& ltrim (std::basic_string<T> &s)
        {
            s.erase(s.begin(),find_if_not(s.begin(),s.end(),[](int c){return isspace(c);}));
            return s;
        }

        template<typename T>
        static inline std::basic_string<T>& rtrim (std::basic_string<T> &s)
        {
            s.erase(find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base(), s.end());
            return s;
        }

        template<typename T>
        static inline void trim (std::basic_string<T> &s)
        {
            ltrim (rtrim (s));
        }

        template<typename T>
        static inline std::basic_string<T> trim (const std::basic_string<T> &s)
        {
            std::string t = s;
            return ltrim (rtrim (t));
        }

        /// An auto_ptr to a vector of strings.
        typedef std::unique_ptr<std::vector<std::string> > StringVectorPtr;
        typedef std::unique_ptr<std::vector<std::wstring> > WStringVectorPtr;

        /// Breaks the string s using the character c as a separator and puts the results in a vector.
        /// Examples: \ref string_util_example.cpp
        /// \param s the string to be splitted.
        /// \param c the separator char.
        /// \param it the iterator of the container to hold the results.
        /// \param trim_elements if true, split will trim the elements before adding them to vector v.
        template<typename Container = std::vector<std::basic_string<char>>, typename CharType = char>
        void split (const std::basic_string<CharType>& s, CharType ch,
                    Container& c, bool trim_elements = true,  bool include_empty_fields = false)//, const std::locale& loc=std::locale())
        {
            size_t i = 0;
            size_t j = s.find(ch);

            if (j == std::basic_string<CharType>::npos)
                c.insert (c.end (), s);
            else
            while (j != std::basic_string<CharType>::npos)
            {
                std::basic_string<CharType> aux = s.substr(i, j-i);
                if (trim_elements)
                    trim (aux);
                if (include_empty_fields || aux.length () > 0)
                    c.insert (c.end (), aux);
                i = ++j;
                j = s.find (ch, j);

                if (j == std::basic_string<CharType>::npos)
                {
                    aux = s.substr (i, s.length( ));
                    if (trim_elements)
                        trim (aux);
                    if (include_empty_fields || aux.length () > 0)
                        c.insert (c.end (), aux);
                }
            }
        }

        /// Breaks the string s using the character c as a separator and puts the results in a vector.
        /// Examples: \ref string_util_example.cpp
        /// \param s the string to be splitted.
        /// \param c the separator char.
        /// \param v the vector of strings holding the results.
        /// \param trim_elements if true, split will trim the elements before adding them to vector v.
        template<typename CharType> void split (const std::basic_string<CharType>& s, CharType c,
                                                std::vector<std::basic_string<CharType> >& v,
                                                bool trim_elements = true,  bool include_empty_fields = false)//, const std::locale& loc=std::locale())
        {
            size_t i = 0;
            size_t j = s.find(c);

            if (j == std::basic_string<CharType>::npos)
                v.push_back (s);
            else
            while (j != std::basic_string<CharType>::npos)
            {
                std::basic_string<CharType> aux = s.substr(i, j-i);
                if (trim_elements)
                    trim (aux);
                if (include_empty_fields || aux.length () > 0)
                    v.push_back (aux);
                i = ++j;
                j = s.find (c, j);

                if (j == std::basic_string<CharType>::npos)
                {
                    aux = s.substr (i, s.length( ));
                    if (trim_elements)
                        trim (aux);
                    if (include_empty_fields || aux.length () > 0)
                        v.push_back (aux);
                }
            }
        }

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
        inline void split (const std::string& s, char c, std::vector<std::string>& v, bool trim_elements = true)
        {
            split<char> (s, c, v, trim_elements);
        }

        /*
        /// \copydoc template<typename CharType> std::auto_ptr<std::vector<std::basic_string<CharType> > > split (const std::basic_string<CharType>&, CharType, bool)
        inline std::auto_ptr<std::vector<std::string> > split (const std::string& s, char c, bool trim_elements = true)
        {
            return split<char> (s, c, trim_elements);
        }
        */

        /// \copydoc template<typename CharType> void split(const std::basic_string<CharType>&, CharType, std::vector<std::basic_string<CharType> >*, bool)
        inline void split (const std::wstring& s, wchar_t c, std::vector<std::wstring>& v, bool trim_elements = true)
        {
            split<wchar_t> (s, c, v, trim_elements);
        }

        /*
        /// \copydoc template<typename CharType> std::auto_ptr<std::vector<std::basic_string<CharType> > > split (const std::basic_string<CharType>&, CharType, bool)
        inline std::auto_ptr<std::vector<std::wstring> > split (const std::wstring& s, wchar_t c, bool trim_elements = true)
        {
            return split<wchar_t> (s, c, trim_elements);
        }
        */
        extern std::string remove_acentos (const std::string& s);

        #ifdef CODE_GEAR
        extern void        __fastcall split (const AnsiString& as, char c, std::vector<AnsiString>* v, bool trim_elements = true);
        extern AnsiString  __fastcall only_digits    (const AnsiString& s);
        extern AnsiString  __fastcall remove_acentos (const AnsiString& s);
        extern AnsiString format_cpf (AnsiString cpf);
        #endif

        /// Pads a string to the <b>right</b> with the given character till it reaches the given size.<p>
        /// Important: negative values are not allowed for parameter n. An assertion will end the program if
        /// that happens.
        /// If length is less than s.length () it does nothing, returns the same string.<br>
        /// \param s the string to be padded.
        /// \param c the character to be used to pad the string.
        /// \param n the number of characters the string needs to be padded.
        template<typename T = char>
        void pad (std::basic_string<T>& s, const T c, const typename std::basic_string<T>::size_type n, bool where = PAD_RIGHT)
        {
            if (n == 0)
              return;
            // This is a protection against negative numbers being passed and automatically converted to huge unsigned numbers.
            // Unfortunately most compilers do not warn about it.
            assert (static_cast<int>(n) > 0);

            const typename std::basic_string<T>::size_type length = s.length ();
            if (where == PAD_RIGHT)
            {
                if (n > length)
                    s.append (n - length, c);
            }
            else
            {
                if (n > length)
                    s.insert (0, n - length, c);
            }
        }

        /// \copydoc pad_copy(std::basic_string<T>&, const T, const typename std::basic_string<T>::size_type)
        /// \return padded string.
        template<typename T = char>
        std::basic_string<T> pad_copy (const std::basic_string<T>& s, const T c, const typename std::basic_string<T>::size_type n, bool where = PAD_RIGHT)
        {
            std::basic_string<T> aux (s);
            pad (aux, c, n, where);
            return aux;
        }

        /// \copydoc pad_copy(std::basic_string<T>&, const T, const typename std::basic_string<T>::size_type)
        /// \return Padded string.
        inline String pad_copy (const char* s, const char c, const unsigned n, bool where = PAD_RIGHT)
        {
            assert (s != 0);

            std::string aux = s;
            pad (aux, c, n, where);
            return aux;
        }

        /// Removes all characters that are not digits from string.
        template<typename T>
        std::basic_string<T> only_digits (const std::basic_string<T>& s)
        {
            std::basic_string<T> out;
            for (const auto& ch: s)
            {
                if (isdigit (ch))
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
       // void only_alpha_numeric_inplace (std::basic_string<T>& s)
       // {
        //    if (! s.size ()) return;
        //    std::remove_if (s.begin (), s.end (), std::not1(IsAlphaNumeric<T> ()));
       // }

        /// Removes all characters that are not alpha-numeric from string.
        template<typename T>
        std::basic_string<T> only_alpha_numeric (const std::basic_string<T>& s)
        {
            std::string out;
            for (const T& ch: s)
            {
                if (isalnum (ch))
                    out += ch;
            }

            //std::string out = s;
            //only_alpha_numeric_inplace (out);
            ////std::remove_copy_if (s.begin (), s.end (), out.begin (), std::not1(IsAlphaNumeric<T> ()));
            return out;
        }

        extern void troca_char (char*    c, const std::locale& loc = std::locale ());
        extern void troca_char (wchar_t* c, const std::locale& loc = std::locale ());


        /// Remove accents from string s (replacing for example � for a).
        template<typename T = char>
        void remove_accents (std::basic_string<T>& s)
        {
            for (T& ch: s)
            {
                troca_char (&ch);
            }
        }

        template<class String = std::string>
        String no_accents (const String& s)
        {
            String r = s;
            remove_accents<typename String::value_type> (r);
            return r;
        }

        template <class CharT = char>
        bool is_space (CharT c)//, const std::locale& loc = std::locale())
        {
            return std::isspace<CharT> (c, std::locale());//, loc);
        }

        /// Remove blanks from string.
        /// ' '	(0x20)	space (SPC)
        /// '\t'	(0x09)	horizontal tab (TAB)
        /// '\n'	(0x0a)	newline (LF)
        /// '\v'	(0x0b)	vertical tab (VT)
        /// '\f'	(0x0c)	feed (FF)
        /// '\r'	(0x0d)	carriage return (CR)
        template<typename T = char>
        void remove_blanks (std::basic_string<T>& s)
        {
            s.erase (std::remove_if (s.begin (), s.end (), is_space<T>), s.end ());
        }

        /// Remove accents from string s (replacing for example � for a).
        template<typename T = char>
        inline std::basic_string<T> copy_remove_accents (std::basic_string<T> s)
        {
            remove_accents (s);
            return s;
        }

        /// Remove all occurrences of string s from target.
        template<typename T = char>
        void remove (const std::basic_string<T>& s, std::basic_string<T>& target)
        {
            typename std::basic_string<T>::size_type pos = target.find (s);
            while (pos != std::basic_string<T>::npos)
            {
                target.erase (pos, s.length ());
                pos = target.find (s);
            }
        }

        /// Remove delimiter char from string.
        template<typename T = char>
        void remove_delimiters (T delimiter, std::basic_string<T>& s)
        {
            if (s[0] == delimiter && s[s.length () - 1] == delimiter)
            {
                s.erase (0              , sizeof (T));
                s.erase (s.length () - 1, sizeof (T));
            }
        }

        /// Remove all occurrences of string s from target.
        template<typename T = char>
        inline std::basic_string<T> copy_remove (const std::basic_string<T>& s, std::basic_string<T> target)
        {
            remove (s, target);
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
        void to_upper (std::basic_string<C>& s, const std::locale& loc = std::locale ())
        {
            typename std::basic_string<C>::iterator p;
            for (p = s.begin (); p != s.end (); ++p)
            {
                *p = std::use_facet<std::ctype<C> >(loc).toupper (*p);
            }
        }

        template<typename C = char>
        void to_lower(std::basic_string<C>& s, const std::locale& loc = std::locale ())
        {
            typename std::basic_string<C>::iterator p;
            for (p = s.begin (); p != s.end (); ++p)
            {
                *p = std::use_facet<std::ctype<C> >(loc).tolower (*p);
            }
        }
        */

        extern std::string  reverse (const std::string&  s);
        extern std::wstring reverse (const std::wstring& s);

        extern void to_upper(std::string& s);
        extern void to_upper(std::wstring& s);
        extern void to_lower(std::string& s);
        extern void to_lower(std::wstring& s);

        extern std::string upper(const std::string& s);
        extern std::string lower(const std::string& s);

        /// Removes all instances of substring p from s.
        template<typename C = char>
        void remove_substr (std::basic_string<C>& s, const std::basic_string<C>& p)
        {
           std::string::size_type n = p.length ();
           for (std::string::size_type i = s. find(p); i != std::string::npos; i = s. find(p))
              s.erase(i, n) ;
        }

        /// Removes all instances of substring p from s.
        // o: original substring
        // r: replacement
        template<typename C = char>
        bool replace_substr (std::basic_string<C>& s, const std::basic_string<C>& o, const std::basic_string<C>& r)
        {
           if (s.size () == 0)
              return false;
           bool replaced = false;
           std::string::size_type n = o.length ();
           for (std::string::size_type i = s.find (o); i != std::string::npos; i = s.find (o))
           {
              s.replace(i, n, r);
              replaced = true;
           }
           return replaced;
        }

        inline bool replace_substr (std::string& s, const char* o, const char* r)
        {
          return replace_substr<char> (s, std::string(o), std::string(r));
        }

        inline bool replace_substr (std::wstring& s, const wchar_t* o, const wchar_t* r)
        {
          return replace_substr<wchar_t> (s, std::wstring(o), std::wstring(r));
        }

        template<class String = std::string>
        String insert_grouping_char (const String& s, typename String::value_type grouping_char = ',')
        {
            String f; //formatted string.
            unsigned primeiro = s.length () % 3;
            if (primeiro == 0)
                primeiro = 3;
            --primeiro;
            unsigned i = 0;
            unsigned count = 0;
            for (typename String::const_iterator it = s.begin (); it != s.end (); ++it)
            {
                f += *it;
                bool last = (++it == s.end ()) ? true : false;
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
        String to_string (IntType number, typename String::value_type grouping_char = ',')
        {
            std::ostringstream ss;
            ss << number;
            String s = ss.str ();
            if (number < 0)
                s = "-" + s;
            return use_grouping_char ? insert_grouping_char<String>(s, grouping_char) : s;
        }

        template<bool use_grouping_char = false>
        String to_string (int number, typename String::value_type grouping_char = ',')
        {
            return to_string <int, use_grouping_char> (number, grouping_char);
        }

        template<bool use_grouping_char = false>
        String to_string(long number, typename String::value_type grouping_char = ',')
        {
            return to_string <long, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        String to_string(long long int number, typename String::value_type grouping_char = ',')
        {
            return to_string <long long int, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        String to_string(unsigned int number, typename String::value_type grouping_char = ',')
        {
            return to_string <unsigned int, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        String to_string(unsigned long int number, typename String::value_type grouping_char = ',')
        {
            return to_string <unsigned long int, use_grouping_char>(number, grouping_char);
        }

        template<bool use_grouping_char = false>
        String to_string(unsigned long long int number, typename String::value_type grouping_char = ',')
        {
            return to_string <unsigned long long int, use_grouping_char>(number, grouping_char);
        }

        // todo: use use_grouping.
        template<class String = std::string>
        String to_string (double number, unsigned num_decimals = 2, bool use_grouping = true, typename String::value_type grouping_char = ',', char decimal_separator = '.')
        {
            typedef typename String::value_type CharT;
            int integer_part = trunc (number);
            String s = to_string<String> (integer_part);
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(num_decimals) << number;
            String s1 = ss.str ();
            typename String::size_type pos = s1.find (decimal_separator);
            String decimal_part = ".00";
            if (pos != std::string::npos)
            {
                unsigned n = (num_decimals + pos) < s1.length () ? num_decimals : s1.length ();
                decimal_part = s1.substr (pos, n + 1);
            }
            pad<CharT> (decimal_part, '0', num_decimals);
            return s + decimal_part;
        }

        inline String pad_left0(long long number, const unsigned n = 4)
        {
            return pad_copy(to_string<long long>(number).c_str(), '0', n, PAD_LEFT);
        }

        /// Remove extension from file name.
        template<typename T = char>
        void remove_ext (std::basic_string<T>& fname)
        {
            typename std::basic_string<T>::size_type pos = fname.find_last_of ('.');
            if (pos != std::basic_string<T>::npos)
            {
                fname.erase (pos, fname.length ());
            }
        }

        /// Remove todos espa�os duplos.
        template<class String = std::wstring>
        String& remove_double_spaces (String& s)
        {
          if (s.size () == 0)
              return s;
          while (replace_substr<typename String::value_type>(s, L"  ", L" "));
          return s;
        }

        inline std::string& remove_double_spaces (std::string& s)
        {
          while (replace_substr<std::string::value_type>(s, "  ", " "));
          return s;
        }

    }   // namespace cpplib
}       // namespace pensar_digital
#endif
