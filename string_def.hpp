// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef STRING_DEF_HPP_INCLUDED
#define STRING_DEF_HPP_INCLUDED

namespace pensar_digital
{
	namespace cpplib
	{
        #define WIDE_CHAR   ///< Define WIDE_CHAR to use wide char strings. Comment this line to use char strings.
        #ifdef WIDE_CHAR 
            using C = wchar_t;
            // define macro W(x) to be used with string literals it just adds L before x.
            #define W(x) L ## x    
        #else
            using C = char;
            // define macro W(x) to be used with string literals it just repeats x.
            #define W(x) x
        #endif // WIDE_CHAR 
 
    }
}

#endif // STRING_DEF_HPP_INCLUDED
