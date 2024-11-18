#include "defines.hpp"
#ifdef CODE_GEAR
#pragma hdrstop
#endif

//---------------------------------------------------------------------------

#ifdef CODE_GEAR
#pragma package(smart_init)
#endif

#include <iostream>
#include "s.hpp"
#include "icu_util.hpp"


namespace pensar_digital
{
	namespace cpplib
	{
 
        //----------------------------------------------------------------------
        const bool PAD_RIGHT = true;
        const bool PAD_LEFT = false;
        const bool TRIM_ELEMENTS = true;
        const bool INCLUDE_EMPTY_FIELDS = true;

        std::wstring to_wstring(const std::string& s)
        {
           return icu::to_wstring(s);
        }

        // Converts from std::wstring to std::string.
        std::string  to_string(const std::wstring& s)
		{
            #ifdef WINDOWS
                return icu::utf16_to_utf8 (s);
            #else
                return icu::utf32_to_utf8 (s);
            #endif
		}


        S  reverse(const S& s)
        {
            S out;
            for (typename S::const_reverse_iterator i = s.crbegin(); i != s.crend(); ++i)
            {
                out += *i;
            }
            return out;
        }

        void to_upper(S& s)
        {
            for (S::iterator p = s.begin(); p != s.end(); ++p)
            {
                if (*p >= W('a') && *p <= W('z'))
                    *p = W('A') + (*p - W('a'));
                else
                {
                    switch (*p)
                    {
                    case W('á'):
                        *p = W('Á');
                        break;
                    case W('à'):
                        *p = W('À');
                        break;
                    case W('ã'):
                        *p = W('Ã');
                        break;
                    case W('ä'):
                        *p = W('Ä');
                        break;
                    case W('â'):
                        *p = W('Â');
                        break;
                    case W('é'):
                        *p = W('É');
                        break;
                    case W('è'):
                        *p = W('È');
                        break;
                    case W('ë'):
                        *p = W('Ë');
                        break;
                    case W('ê'):
                        *p = W('Ê');
                        break;
                    case W('í'):
                        *p = W('Í');
                        break;
                    case W('ì'):
                        *p = W('Ì');
                        break;
                    case W('ï'):
                        *p = W('Ï');
                        break;
                    case W('î'):
                        *p = W('Î');
                        break;
                    case W('ó'):
                        *p = W('Ó');
                        break;
                    case W('ò'):
                        *p = W('Ò');
                        break;
                    case W('õ'):
                        *p = W('Õ');
                        break;
                    case W('ö'):
                        *p = W('Ö');
                        break;
                    case W('ô'):
                        *p = W('Ô');
                        break;
                    case W('ú'):
                        *p = W('Ú');
                        break;
                    case W('ù'):
                        *p = W('Ù');
                        break;
                    case W('ü'):
                        *p = W('Ü');
                        break;
                    case W('û'):
                        *p = W('Û');
                        break;
                    case W('Ç'):
                        *p = W('ç');
                        break;
                    case W('Ñ'):
                        *p = W('ñ');
                        break;
                    default:
                        break;
                    }
                }
            } //for
        };

        S upper(const S& s)
        {
            S r = s;
            to_upper(r);
            return r;
        }

        S lower(const S& s)
        {
            S r = s;
            to_lower(r);
            return r;
        }

        void to_lower(S& s)
        {
            for (S::iterator p = s.begin(); p != s.end(); ++p)
            {
                if (*p >= W('A') && *p <= W('Z'))
                    *p = W('a') + (*p - W('A'));
                else
                {
                    switch (*p)
                    {
                    case W('Á'):
                        *p = W('á');
                        break;
                    case W('À'):
                        *p = W('à');
                        break;
                    case W('Ã'):
                        *p = W('ã');
                        break;
                    case W('Ä'):
                        *p = W('ä');
                        break;
                    case W('Â'):
                        *p = W('â');
                        break;
                    case W('É'):
                        *p = W('é');
                        break;
                    case W('È'):
                        *p = W('è');
                        break;
                    case W('Ë'):
                        *p = W('ë');
                        break;
                    case W('Ê'):
                        *p = W('ê');
                        break;
                    case W('Í'):
                        *p = W('í');
                        break;
                    case W('Ì'):
                        *p = W('ì');
                        break;
                    case W('Ï'):
                        *p = W('ï');
                        break;
                    case W('Î'):
                        *p = W('î');
                        break;
                    case W('Ó'):
                        *p = W('ó');
                        break;
                    case W('Ò'):
                        *p = W('ò');
                        break;
                    case W('Õ'):
                        *p = W('õ');
                        break;
                    case W('Ö'):
                        *p = W('ö');
                        break;
                    case W('Ô'):
                        *p = W('ô');
                        break;
                    case W('Ú'):
                        *p = W('ú');
                        break;
                    case W('Ù'):
                        *p = W('ù');
                        break;
                    case W('Ü'):
                        *p = W('ü');
                        break;
                    case W('Û'):
                        *p = W('û');
                        break;
                    case W('Ç'):
                        *p = W('ç');
                        break;
                    case W('Ñ'):
                        *p = W('ñ');
                        break;
                    default:
                        break;
                    }
                }
            }
        };

        void troca_char(C* c, const std::locale& loc)
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
            switch (*c)
            {
            case W('á'):
            case W('à'):
            case W('ã'):
            case W('ä'):
            case W('â'):
                *c = W('a');
                break;
            case W('é'):
            case W('è'):
            case W('ë'):
            case W('ê'):
                *c = W('e');
                break;
            case W('í'):
            case W('ì'):
            case W('ï'):
            case W('î'):
                *c = W('i');
                break;
            case W('ó'):
            case W('ò'):
            case W('õ'):
            case W('ö'):
            case W('ô'):
                *c = W('o');
                break;
            case W('ú'):
            case W('ù'):
            case W('ü'):
            case W('û'):
                *c = W('u');
                break;
            case W('ç'):
                *c = W('c');
                break;
            case W('Á'):
            case W('À'):
            case W('Ã'):
            case W('Ä'):
            case W('Â'):
                *c = W('A');
                break;
            case W('É'):
            case W('È'):
            case W('Ë'):
            case W('Ê'):
                *c = W('E');
                break;
            case W('Í'):
            case W('Ì'):
            case W('Ï'):
            case W('Î'):
                *c = W('I');
                break;
            case W('Ó'):
            case W('Ò'):
            case W('Õ'):
            case W('Ö'):
            case W('Ô'):
                *c = W('O');
                break;
            case W('Ú'):
            case W('Ù'):
            case W('Ü'):
            case W('Û'):
                *c = W('U');
                break;
            case W('Ç'):
                *c = W('C');
                break;
            default:
                break;
            }
        }

        /*void troca_char(wchar_t* c, const std::locale& loc)
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
            switch (*c)
            {
                // According to codepage ISO/IEC 8859-1
            case L'\u00C0'): // A com crase.
            case L'\u00C1'): // A com agudo.
            case L'\u00C2'): // A com circunflexo.
            case L'\u00C3'): // A com til.
            case L'\u00C4'): // A com trema.
            case L'\u00C5'): // A com bolinha de angstron.
                *c = L'A');
                break;
            case L'\u00C8'):  // E com crase.
            case L'\u00C9'):  // E com agudo.
            case L'\u00CA'):  // E com circunflexo.
            case L'\u00CB'):  // E com trema.
                *c = L'E');
                break;
            case L'\u00CC'):  // I com crase.
            case L'\u00CD'):  // I com agudo.
            case L'\u00CE'):  // I com circunflexo.
            case L'\u00CF'):  // I com trema.
                *c = L'I');
                break;
            case L'\u00D2'): // O com crase.
            case L'\u00D3'): // O com agudo.
            case L'\u00D4'): // O com circunflexo.
            case L'\u00D5'): // O com til.
            case L'\u00D6'): // O com trema.
                *c = L'O');
                break;
            case L'\u00D9'): // U com crase.
            case L'\u00DA'): // U com agudo.
            case L'\u00DB'): // U com circunflexo.
            case L'\u00DC'): // U com trema.
                *c = L'U');
                break;
            case L'\u00C7'): // C cidilha.
                *c = L'C');
                break;
            case L'\u00D1'): // N com til
                *c = L'N');
                break;
            case L'\u00E0'): // a com crase.
            case L'\u00E1'): // a com agudo.
            case L'\u00E2'): // a com circunflexo.
            case L'\u00E3'): // a com til.
            case L'\u00E4'): // a com trema.
                *c = L'a');
                break;
            case L'\u00E8'):  // e com crase.
            case L'\u00E9'):  // e com agudo.
            case L'\u00EA'):  // e com circunflexo.
            case L'\u00EB'):  // e com trema.
                *c = L'e');
                break;
            case L'\u00EC'):  // i com crase.
            case L'\u00ED'):  // i com agudo.
            case L'\u00EE'):  // i com circunflexo.
            case L'\u00EF'):  // i com trema.
                *c = L'i');
                break;
            case L'\u00F2'): // o com crase.
            case L'\u00F3'): // o com agudo.
            case L'\u00F4'): // o com circunflexo.
            case L'\u00F5'): // o com til.
            case L'\u00F6'): // o com trema.
                *c = L'o');
                break;
            case L'\u00E7'): // c cidilha.
                *c = L'c');
                break;
            case L'\u00F1'): // n com til
                *c = L'n');
                break;
            case L'\u00F9'): // u com crase.
            case L'\u00FA'): // u com agudo.
            case L'\u00FB'): // u com circunflexo.
            case L'\u00FC'): // u com trema.
                *c = L'u');
                break;
            default:
                break;
            }
        }
        */
#ifdef CODE_GEAR
        /*void __fastcall split(const AnsiString& as, char c, std::vector<AnsiString>* v, bool trim_elements)
        {
            S s = as.c_str();
            S::size_type i = 0;
            S::size_type j = s.find(c);

            if (j == S::npos)
                v->push_back(as);
            else
                while (j != S::npos)
                {
                    S aux = s.substr(i, j - i);
                    if (trim_elements)
                        trim(aux);
                    AnsiString s1 = aux.c_str();
                    v->push_back(s1);
                    i = ++j;
                    j = s.find(c, j);

                    if (j == S::npos)
                    {
                        aux = s.substr(i, s.length());
                        if (trim_elements)
                            trim(aux);
                        s1 = aux.c_str();
                        v->push_back(s1);
                    }
                }
        }

        AnsiString __fastcall only_digits(const AnsiString& s)
        {
            AnsiString d = "";
            for (int i = 1; i <= s.Length(); i++)
            {
                if (isdigit(s[i]))
                    d += s[i];
            }
            return d;
        }

        AnsiString __fastcall remove_acentos(const AnsiString& s)
        {
            AnsiString sem_acentos = s;
            for (int i = 1; i <= s.Length(); ++i)
            {
                troca_char<char>(&sem_acentos[i]);
            }
            return sem_acentos;
        }*/
#endif
        //----------------------------------------------------------------------

        void remove_accent  (C* c) noexcept
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
            switch (*c)
            {
            case W('á'):
            case W('à'):
            case W('ã'):
            case W('ä'):
            case W('â'):
                *c = W('a');
                break;
            case W('é'):
            case W('è'):
            case W('ë'):
            case W('ê'):
                *c = W('e');
                break;
            case W('í'):
            case W('ì'):
            case W('ï'):
            case W('î'):
                *c = W('i');
                break;
            case W('ó'):
            case W('ò'):
            case W('õ'):
            case W('ö'):
            case W('ô'):
                *c = W('o');
                break;
            case W('ú'):
            case W('ù'):
            case W('ü'):
            case W('û'):
                *c = W('u');
                break;
            case W('ç'):
                *c = W('c');
                break;
            case W('Á'):
            case W('À'):
            case W('Ã'):
            case W('Ä'):
            case W('Â'):
                *c = W('A');
                break;
            case W('É'):
            case W('È'):
            case W('Ë'):
            case W('Ê'):
                *c = W('E');
                break;
            case W('Í'):
            case W('Ì'):
            case W('Ï'):
            case W('Î'):
                *c = W('I');
                break;
            case W('Ó'):
            case W('Ò'):
            case W('Õ'):
            case W('Ö'):
            case W('Ô'):
                *c = W('O');
                break;
            case W('Ú'):
            case W('Ù'):
            case W('Ü'):
            case W('Û'):
                *c = W('U');
                break;
            case W('Ç'):
                *c = W('C');
                break;
            default:
                break;
            }
        }

        /*void remove_accent(wchar_t* c) noexcept
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
            switch (*c)
            {
                // According to codepage ISO/IEC 8859-1
            case L'\u00C0'): // A com crase.
            case L'\u00C1'): // A com agudo.
            case L'\u00C2'): // A com circunflexo.
            case L'\u00C3'): // A com til.
            case L'\u00C4'): // A com trema.
            case L'\u00C5'): // A com bolinha de angstron.
                *c = L'A');
                break;
            case L'\u00C8'):  // E com crase.
            case L'\u00C9'):  // E com agudo.
            case L'\u00CA'):  // E com circunflexo.
            case L'\u00CB'):  // E com trema.
                *c = L'E');
                break;
            case L'\u00CC'):  // I com crase.
            case L'\u00CD'):  // I com agudo.
            case L'\u00CE'):  // I com circunflexo.
            case L'\u00CF'):  // I com trema.
                *c = L'I');
                break;
            case L'\u00D2'): // O com crase.
            case L'\u00D3'): // O com agudo.
            case L'\u00D4'): // O com circunflexo.
            case L'\u00D5'): // O com til.
            case L'\u00D6'): // O com trema.
                *c = L'O');
                break;
            case L'\u00D9'): // U com crase.
            case L'\u00DA'): // U com agudo.
            case L'\u00DB'): // U com circunflexo.
            case L'\u00DC'): // U com trema.
                *c = L'U');
                break;
            case L'\u00C7'): // C cidilha.
                *c = L'C');
                break;
            case L'\u00D1'): // N com til
                *c = L'N');
                break;
            case L'\u00E0'): // a com crase.
            case L'\u00E1'): // a com agudo.
            case L'\u00E2'): // a com circunflexo.
            case L'\u00E3'): // a com til.
            case L'\u00E4'): // a com trema.
                *c = L'a');
                break;
            case L'\u00E8'):  // e com crase.
            case L'\u00E9'):  // e com agudo.
            case L'\u00EA'):  // e com circunflexo.
            case L'\u00EB'):  // e com trema.
                *c = L'e');
                break;
            case L'\u00EC'):  // i com crase.
            case L'\u00ED'):  // i com agudo.
            case L'\u00EE'):  // i com circunflexo.
            case L'\u00EF'):  // i com trema.
                *c = L'i');
                break;
            case L'\u00F2'): // o com crase.
            case L'\u00F3'): // o com agudo.
            case L'\u00F4'): // o com circunflexo.
            case L'\u00F5'): // o com til.
            case L'\u00F6'): // o com trema.
                *c = L'o');
                break;
            case L'\u00E7'): // c cidilha.
                *c = L'c');
                break;
            case L'\u00F1'): // n com til
                *c = L'n');
                break;
            case L'\u00F9'): // u com crase.
            case L'\u00FA'): // u com agudo.
            case L'\u00FB'): // u com circunflexo.
            case L'\u00FC'): // u com trema.
                *c = L'u');
                break;
            default:
                break;
            }
        }*/
    }   // namespace cpplib.
}   // namespace pensar_digital.
