// $Id: html_util.cpp 40 2010-07-16 22:04:49Z mgpensar $

#ifdef VISUAL_STUDIO
#include "stdafx.h"
#endif
#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "html_util.h"
#include <string>

#ifdef CODE_GEAR
#pragma package(smart_init)
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        std::string escape_html (const std::string& s)
        {
            std::string t;
            std::string::const_iterator i;
            for(i = s.begin (); i != s.end (); ++i)
            {
                switch (*i)
                {
                    case '<':
                        t += "&lt;";
                        break;
                    case '>':
                        t += "&gt;";
                        break;
                    case '&':
                        t += "&amp;";
                        break;
                    default:
                        t += *i;
                        break;
                }
            }
            return t;
        }

        std::string unescape_html (const std::string& s)
        {
        //	 string t = find_replace ("&lt;", "<", s);
        //	 t = find_replace ("&gt;", ">", t);
        //	 t = find_replace ("&amp;", "&", t);
        //
        //	 return t;
            std::string t;
            enum _state {START, E, EL, EG, EA, ELT, EGT, EAM, EAMP} state = START; // {nothing, &, &l, &g, &a, &lt, &gt, &am, &amp}
            for(std::string::const_iterator i = s.begin (); i != s.end (); ++i)
            {
                switch (state)
                {
                    case START:
                        if (*i == '&')
                            state = E;
                        else
                            t += *i;
                        break;
                    case E:
                        switch (*i)
                        {
                            case 'l':
                                state = EL;
                                break;
                            case 'g':
                                state = EG;
                                break;
                            case 'a':
                                state = EA;
                                break;
                            case '&':
                                break;
                            default:
                                t += '&';
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case EL:
                        switch (*i)
                        {
                            case 't':
                                state = ELT;
                                break;
                            case '&':
                                state = E;
                                break;
                            default:
                                t += "&l";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case EG:
                        switch (*i)
                        {
                            case 't':
                                state = EGT;
                                break;
                            case '&':
                                state = E;
                                break;
                            default:
                                t += "&g";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case EA:
                        switch (*i)
                        {
                            case 'm':
                                state = EAM;
                                break;
                            case '&':
                                state = E;
                                break;
                            default:
                                t += "&a";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case ELT:
                        switch (*i)
                        {
                            case ';':
                                t += '<';
                                state = START;
                                break;
                            case '&':
                                t += "&lt";
                                state = E;
                                break;
                            default:
                                t += "&lt";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case EGT:
                        switch (*i)
                        {
                            case ';':
                                t += '>';
                                state = START;
                                break;
                            case '&':
                                t += "&gt";
                                state = E;
                                break;
                            default:
                                t += "&gt";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case EAM:
                        switch (*i)
                        {
                            case 'p':
                                state = EAMP;
                                break;
                            case '&':
                                t += "&am";
                                state = E;
                                break;
                            default:
                                t += "&am";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    case EAMP:
                        switch (*i)
                        {
                            case ';':
                                t += '&';
                                state = START;
                                break;
                            case '&':
                                t += "&amp";
                                state = E;
                                break;
                            default:
                                t += "&amp";
                                t += *i;
                                state = START;
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
            return t;
        }

        #ifdef CODE_GEAR
        AnsiString escape_html(AnsiString s)
        {
            AnsiString t;
            for(int i = 1; i <= s.Length(); i++)
            {
                char c = s[i];
                switch(c) {
                case '<': t+="&lt;"; break;
                case '>': t+="&gt;"; break;
                case '&': t+="&amp;"; break;
                default:  t+=c; break;
                }
            }
            return t;
        }

        AnsiString unescape_html (AnsiString s)
        {
            AnsiString t;
            enum _state {START, E, EL, EG, EA, ELT, EGT, EAM, EAMP} state = START; // {nothing, &, &l, &g, &a, &lt, &gt, &am, &amp}
            for(int i = 1; i <= s.Length (); ++i)
            {
                char c = s[i];
                switch (state)
                {
                    case START:
                        if (c == '&')
                            state = E;
                        else
                            t += c;
                        break;
                    case E:
                        switch (c)
                        {
                            case 'l':
                                state = EL;
                                break;
                            case 'g':
                                state = EG;
                                break;
                            case 'a':
                                state = EA;
                                break;
                            case '&':
                                break;
                            default:
                                t += '&';
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case EL:
                        switch (c)
                        {
                            case 't':
                                state = ELT;
                                break;
                            case '&':
                                state = E;
                                break;
                            default:
                                t += "&l";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case EG:
                        switch (c)
                        {
                            case 't':
                                state = EGT;
                                break;
                            case '&':
                                state = E;
                                break;
                            default:
                                t += "&g";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case EA:
                        switch (c)
                        {
                            case 'm':
                                state = EAM;
                                break;
                            case '&':
                                state = E;
                                break;
                            default:
                                t += "&a";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case ELT:
                        switch (c)
                        {
                            case ';':
                                t += '<';
                                state = START;
                                break;
                            case '&':
                                t += "&lt";
                                state = E;
                                break;
                            default:
                                t += "&lt";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case EGT:
                        switch (c)
                        {
                            case ';':
                                t += '>';
                                state = START;
                                break;
                            case '&':
                                t += "&gt";
                                state = E;
                                break;
                            default:
                                t += "&gt";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case EAM:
                        switch (c)
                        {
                            case 'p':
                                state = EAMP;
                                break;
                            case '&':
                                t += "&am";
                                state = E;
                                break;
                            default:
                                t += "&am";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    case EAMP:
                        switch (c)
                        {
                            case ';':
                                t += '&';
                                state = START;
                                break;
                            case '&':
                                t += "&amp";
                                state = E;
                                break;
                            default:
                                t += "&amp";
                                t += c;
                                state = START;
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
            return t;
        }
        #endif
    }   // namespace cpplib
}       // namespace pensar_digital
