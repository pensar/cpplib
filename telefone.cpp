// $Id: telefone.cpp 40 2010-07-16 22:04:49Z mgpensar $

#ifdef VISUAL_STUDIO
#include "stdafx.h"
#endif
#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "telefone.h"

namespace pensar_digital
{
    namespace cpplib
    {
        #ifdef CODE_GEAR
        // Qualificadores.
        const AnsiString asCOMERCIAL   = "Comercial"  ;
        const AnsiString asRESIDENCIAL = "Residencial";
        const AnsiString asCELULAR     = "Celular"    ;
        const AnsiString asEMERGENCIA  = "Emergencia";

        const AnsiString asDEFAULT_DDD = "11";
        const AnsiString asDEFAULT_COD_PAIS = "55";

        #pragma package(smart_init)
        #endif

        // Qualificadores.
        extern const std::string COMERCIAL		  = "Comercial";
        extern const std::string RESIDENCIAL	  = "Residencial";
        extern const std::string CELULAR		  = "Celular";
        extern const std::string EMERGENCIA		  = "Emergencia";

        extern const std::string DEFAULT_DDD      = "11";
        extern const std::string DEFAULT_COD_PAIS = "55";


        #ifdef CODE_GEAR
            vclTelefone parse_phone_number (AnsiString valid_number)
            {
                vclTelefone t;
                AnsiString d = only_digits (valid_number);
                //assert (is_valid_phone_number (valid_number));
                int size = d.Length ();
                if (size >= 8)
                {
                    int restante = size - 8;
                    t.numero = d.SubString (size - 7, 8);
                    if (restante)
                    {
                        t.cod_ddd = d.SubString (restante - 1, 2);
                        if (restante - 2)
                        {
                            t.cod_pais = d.SubString (0, restante - 2);
                        }
                    }
                }
                else
                    t.numero = d;
                return t;
            }

            AnsiString format_phone_number (AnsiString valid_number)
            {
                AnsiString d = only_digits (valid_number);
                assert (is_valid_phone_number (valid_number));
                int size = d.Length ();
                int restante = size - 8;
                AnsiString numero = d.SubString (size - 7, 4);
                AnsiString numero2 = d.SubString (size - 3, 4);
                AnsiString result;
                if (restante)
                {
                    AnsiString cod_ddd = d.SubString (restante - 1, 2);
                    if (restante - 2)
                    {
                        AnsiString cod_pais = d.SubString (0, restante - 2);
                        result = cod_pais + "-";
                    }
                    result += cod_ddd + "-";
                }
                return result + numero + "-" + numero2;
            }
        #endif
    }   // namespace cpplib
}       // namespace pensar_digital
