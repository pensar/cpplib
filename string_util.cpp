// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)



#ifdef CODE_GEAR
#pragma hdrstop
#endif

//---------------------------------------------------------------------------

#ifdef CODE_GEAR
#pragma package(smart_init)
#endif

#include "string_util.hpp"

#include <iostream>

namespace pensar_digital
{
    namespace cpplib
    {
        const bool PAD_RIGHT = true ;
        const bool PAD_LEFT  = false;
        const bool TRIM_ELEMENTS = true;
        const bool INCLUDE_EMPTY_FIELDS = true;

        std::string  reverse (const std::string&  s)
        {
            std::string out;
            for (std::string::const_reverse_iterator i = s.crbegin(); i != s.crend(); ++i)
            {
              out += *i;
            }
            return out;
        }

        std::wstring reverse (const std::wstring& s)
        {
            std::wstring out;
            for (std::wstring::const_reverse_iterator i = s.crbegin(); i != s.crend(); ++i)
            {
              out += *i;
            }
            return out;
        }

        void to_upper(std::string& s)
        {
            for (std::string::iterator p = s.begin (); p != s.end (); ++p)
            {
                if (*p >= 'a' && *p <= 'z')
                    *p = 'A' + (*p - 'a');
                else
                {
                    switch (*p)
                    {
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        default:
                            break;
                    }
                }
            } //for
        };

        void to_upper(std::wstring& s)
        {
           for (std::wstring::iterator p = s.begin (); p != s.end (); ++p)
              *p = towupper(*p); // towupper is for wchar_t
        };

        std::string upper(const std::string& s)
        {
            std::string r = s;
            to_upper (r);
            return r;
        }

        std::string lower(const std::string& s)
        {
            std::string r = s;
            to_lower (r);
            return r;
        }

        void to_lower(std::string& s)
        {
            for (std::string::iterator p = s.begin (); p != s.end (); ++p)
            {
                if (*p >= 'A' && *p <= 'Z')
                    *p = 'a' + (*p - 'A');
                else
                {
                    switch (*p)
                    {
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        case '�':
                            *p = '�';
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        void to_lower(std::wstring& s)
        {
           for (std::wstring::iterator p = s.begin (); p != s.end (); ++p)
              *p = towlower(*p);
        };

        void troca_char (char* c, const std::locale& loc)
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
                switch (*c)
                {
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'a';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'e';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'i';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'o';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'u';
                        break;
                    case '�':
                        *c = 'c';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'A';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'E';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'I';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'O';
                        break;
                    case '�':
                    case '�':
                    case '�':
                    case '�':
                        *c = 'U';
                        break;
                    case '�':
                        *c = 'C';
                        break;
                    default:
                        break;
                }
        }

        void troca_char (wchar_t* c, const std::locale& loc)
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
                switch (*c)
                {
                                    // According to codepage ISO/IEC 8859-1
                    case L'\u00C0': // A com crase.
                    case L'\u00C1': // A com agudo.
                    case L'\u00C2': // A com circunflexo.
                    case L'\u00C3': // A com til.
                    case L'\u00C4': // A com trema.
                    case L'\u00C5': // A com bolinha de angstron.
                        *c = L'A';
                        break;
                    case L'\u00C8':  // E com crase.
                    case L'\u00C9':  // E com agudo.
                    case L'\u00CA':  // E com circunflexo.
                    case L'\u00CB':  // E com trema.
                        *c = L'E';
                        break;
                    case L'\u00CC':  // I com crase.
                    case L'\u00CD':  // I com agudo.
                    case L'\u00CE':  // I com circunflexo.
                    case L'\u00CF':  // I com trema.
                        *c = L'I';
                        break;
                    case L'\u00D2': // O com crase.
                    case L'\u00D3': // O com agudo.
                    case L'\u00D4': // O com circunflexo.
                    case L'\u00D5': // O com til.
                    case L'\u00D6': // O com trema.
                        *c = L'O';
                        break;
                    case L'\u00D9': // U com crase.
                    case L'\u00DA': // U com agudo.
                    case L'\u00DB': // U com circunflexo.
                    case L'\u00DC': // U com trema.
                        *c = L'U';
                        break;
                    case L'\u00C7': // C cidilha.
                        *c = L'C';
                        break;
                    case L'\u00D1': // N com til
                        *c = L'N';
                        break;
                    case L'\u00E0': // a com crase.
                    case L'\u00E1': // a com agudo.
                    case L'\u00E2': // a com circunflexo.
                    case L'\u00E3': // a com til.
                    case L'\u00E4': // a com trema.
                        *c = L'a';
                        break;
                    case L'\u00E8':  // e com crase.
                    case L'\u00E9':  // e com agudo.
                    case L'\u00EA':  // e com circunflexo.
                    case L'\u00EB':  // e com trema.
                        *c = L'e';
                        break;
                    case L'\u00EC':  // i com crase.
                    case L'\u00ED':  // i com agudo.
                    case L'\u00EE':  // i com circunflexo.
                    case L'\u00EF':  // i com trema.
                        *c = L'i';
                        break;
                    case L'\u00F2': // o com crase.
                    case L'\u00F3': // o com agudo.
                    case L'\u00F4': // o com circunflexo.
                    case L'\u00F5': // o com til.
                    case L'\u00F6': // o com trema.
                        *c = L'o';
                        break;
                    case L'\u00E7': // c cidilha.
                        *c = L'c';
                        break;
                    case L'\u00F1': // n com til
                        *c = L'n';
                        break;
                    case L'\u00F9': // u com crase.
                    case L'\u00FA': // u com agudo.
                    case L'\u00FB': // u com circunflexo.
                    case L'\u00FC': // u com trema.
                        *c = L'u';
                        break;
                    default:
                        break;
                }
        }

        #ifdef CODE_GEAR
        void __fastcall split (const AnsiString& as, char c, std::vector<AnsiString>* v, bool trim_elements)
        {
            std::string s = as.c_str ();
            std::string::size_type i = 0;
            std::string::size_type j = s.find(c);

            if (j == std::string::npos)
                v->push_back (as);
            else
            while (j != std::string::npos)
            {
                std::string aux = s.substr(i, j-i);
                if (trim_elements)
                    trim (aux);
                AnsiString s1 = aux.c_str ();
                v->push_back (s1);
                i = ++j;
                j = s.find (c, j);

                if (j == std::string::npos)
                {
                    aux = s.substr (i, s.length( ));
                    if (trim_elements)
                        trim (aux);
                    s1 = aux.c_str ();
                    v->push_back (s1);
                }
            }
        }

        AnsiString __fastcall only_digits (const AnsiString& s)
        {
            AnsiString d = "";
            for (int i = 1; i <= s.Length(); i++)
            {
                if (isdigit (s[i]))
                    d += s[i];
            }
            return d;
        }

        AnsiString __fastcall remove_acentos (const AnsiString& s)
        {
            AnsiString sem_acentos = s;
            for (int i = 1; i <= s.Length(); ++i)
            {
                troca_char<char> (&sem_acentos[i]);
            }
            return sem_acentos;
        }
        #endif
    }   // namespace cpplib
}       // namespace pensar_digital
/// \example string_util_example.cpp
/// string_util usage examples.
