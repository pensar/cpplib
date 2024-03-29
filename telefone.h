// $Id: telefone.h 40 2010-07-16 22:04:49Z mgpensar $

#ifndef telefoneH
#define telefoneH

#include <string>
#ifdef CODE_GEAR
#include <vcl.h>
#endif
#include "constants.h"

namespace pensar_digital
{
    namespace cpplib
    {
        // Qualificadores.
        #ifdef CODE_GEAR
        extern const AnsiString asCOMERCIAL;
        extern const AnsiString asRESIDENCIAL;
        extern const AnsiString asCELULAR;
        extern const AnsiString asEMERGENCIA;

        extern const AnsiString asDEFAULT_DDD;
        extern const AnsiString asDEFAULT_COD_PAIS;

        typedef struct strvclTelefone
        {
            AnsiString cod_pais;
            AnsiString cod_ddd;
            AnsiString numero;
            AnsiString qualificador;
            int id;
            strvclTelefone (AnsiString numero_telefone = "", AnsiString ddd = "",
                         AnsiString pais = "", AnsiString qualif = asCOMERCIAL)
                         : numero(numero_telefone), cod_ddd(ddd), cod_pais(pais), qualificador(qualif), id(ID_INEXISTENTE) {}
            strvclTelefone (const strvclTelefone& t): numero(t.numero), cod_ddd(t.cod_ddd), cod_pais(t.cod_pais), qualificador(t.qualificador), id(ID_INEXISTENTE) {}

            operator AnsiString ()
            {
                AnsiString s = "";
                if (cod_pais != "")
                    s += cod_pais + "-";
                if (cod_ddd != "")
                    s += cod_ddd + "-";
                if (numero.Length () >= 8)
                {
                    s += numero.SubString (numero.Length () - 7, 4) + "-";
                    s += numero.SubString (numero.Length () - 3, 4);
                }
                else
                    s += numero;
                return s;
            }
            bool operator == (const strvclTelefone& t)
            {
                return cod_pais == t.cod_pais && cod_ddd == t.cod_ddd && numero == t.numero;
            }
            bool operator != (const strvclTelefone& t)
            {
                return !(*this == t);
            }
        } vclTelefone;

        extern AnsiString format_phone_number (AnsiString valid_number);
        extern vclTelefone parse_phone_number (AnsiString valid_number);
        #endif
        //---------------------------------------------------------------------------

        extern const std::string COMERCIAL;
        extern const std::string RESIDENCIAL;
        extern const std::string CELULAR;
        extern const std::string EMERGENCIA;

        extern const std::string DEFAULT_DDD;
        extern const std::string DEFAULT_COD_PAIS;

        typedef struct strTelefone
        {
            std::string cod_pais;
            std::string cod_ddd;
            std::string numero;
            std::string qualificador;
            int id;
            strTelefone (std::string numero_telefone = "", std::string ddd = "",
                         std::string pais = "", std::string qualif = COMERCIAL)
                         : cod_pais(pais), cod_ddd(ddd), numero(numero_telefone), qualificador(qualif), id(ID_INEXISTENTE) {}
            strTelefone (const strTelefone& t): cod_pais(t.cod_pais), cod_ddd(t.cod_ddd), numero(t.numero), qualificador(t.qualificador), id(ID_INEXISTENTE) {}

            operator std::string ()
            {
                std::string s = "";
                if (cod_pais != "")
                    s += cod_pais + "-";
                if (cod_ddd != "")
                    s += cod_ddd + "-";
                if (numero.length () >= 8)
                {
                    s += numero.substr (numero.length () - 8, 4) + "-";
                    s += numero.substr (numero.length () - 4, 4);
                }
                else
                    s += numero;
                return s;
            }
            bool operator == (const strTelefone& t)
            {
                return cod_pais == t.cod_pais && cod_ddd == t.cod_ddd && numero == t.numero;
            }
            bool operator != (const strTelefone& t)
            {
                return !(*this == t);
            }
        } Telefone;
    }   // namespace cpplib
}       // namespace pensar_digital

#endif
