#include "s.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
        void remove_accent  (char* c) noexcept
        {
            //T ch = std::use_facet<std::ctype<T> >(loc).tolower (*c);
            switch (*c)
            {
            case 'á':
            case 'à':
            case 'ã':
            case 'ä':
            case 'â':
                *c = 'a';
                break;
            case 'é':
            case 'è':
            case 'ë':
            case 'ê':
                *c = 'e';
                break;
            case 'í':
            case 'ì':
            case 'ï':
            case 'î':
                *c = 'i';
                break;
            case 'ó':
            case 'ò':
            case 'õ':
            case 'ö':
            case 'ô':
                *c = 'o';
                break;
            case 'ú':
            case 'ù':
            case 'ü':
            case 'û':
                *c = 'u';
                break;
            case 'ç':
                *c = 'c';
                break;
            case 'Á':
            case 'À':
            case 'Ã':
            case 'Ä':
            case 'Â':
                *c = 'A';
                break;
            case 'É':
            case 'È':
            case 'Ë':
            case 'Ê':
                *c = 'E';
                break;
            case 'Í':
            case 'Ì':
            case 'Ï':
            case 'Î':
                *c = 'I';
                break;
            case 'Ó':
            case 'Ò':
            case 'Õ':
            case 'Ö':
            case 'Ô':
                *c = 'O';
                break;
            case 'Ú':
            case 'Ù':
            case 'Ü':
            case 'Û':
                *c = 'U';
                break;
            case 'Ç':
                *c = 'C';
                break;
            default:
                break;
            }
        }

        void remove_accent (wchar_t* c) noexcept
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
    }   // namespace cpplib.
}   // namespace pensar_digital.
