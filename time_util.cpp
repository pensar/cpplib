// $Id: time_util.cpp 70 2014-01-16 07:04:22Z mgpensar $

#include "mg.hpp"
#ifdef VISUAL_STUDIO
#include "stdafx.h"
#endif

#include <string>
#include <iostream>
#include <vector>

#ifdef BORLAND
#include <vcl.h>
#endif

#include "time_util.h"
#include "string_util.hpp"

#ifdef CODE_GEAR
#pragma hdrstop
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        using namespace std;
        const char *diaSemana[] = {"Domingo", "Segunda-feira", "Ter�a-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "S�bado"};

        #ifdef BORLAND
        time_t today ()
        {
            TDateTime date = TDateTime::CurrentDate();
            AnsiString s = date.FormatString("dd/mm/YY");
            return date2time_t (s.c_str());
        }
        #endif

        //---------------------------------------------------------------------------
        // Converte string do tipo dd/mm/aa para time_t.
        time_t date2time_t (const std::string& data)
        {
            if ((&data == 0) || (data.size() == 0)) return 0;

            time_t t = time (&t);
            struct tm *date = gmtime (&t);
            std::vector<std::string> v;
            split (data, '/', v);
            try
            {
                date->tm_mday = std::stoi(v[0]);
                date->tm_mon  = std::stoi(v[1]) - 1;
                /* TODO : Arquivo deve ter ano com 4 d�gitos e "+ 100" deve ser trocado para "- 1900" para evitar bug no ano 3000. */
                date->tm_year = std::stoi(v[2]) + 100; // Anos desde 1900 ent�o temos que somar 100 para 20xx.
                date->tm_hour = 0;
                date->tm_min  = 0;
                date->tm_sec  = 0;
            }
            catch (exception& e)
            {
              cerr << "date2time_t - Bad cast: " << e.what( ) << endl;
            }

            t = mktime (date);
            return t;
        }

        // Converte string do tipo yyyy-mm-aa para time_t.
        time_t strdate2time_t (const std::string& data)
        {
            if ((&data == 0) || (data.size() == 0)) return 0;

            time_t t = time (&t);
            struct tm *date = gmtime (&t);
            std::vector<std::string> v;
            split (data, '-', v);
            try
            {
                date->tm_mday = stoi(v[2]);
                date->tm_mon  = stoi(v[1]) - 1;
                date->tm_year = stoi(v[0]) - 1900;
                date->tm_hour = 0;
                date->tm_min  = 0;
                date->tm_sec  = 0;
            }
            catch (exception& e)
            {
              cerr << "strdate2time_t - Bad cast: " << e.what( ) << endl;
            }

            t = mktime (date);
            return t;
        }

        /// Converte string do tipo yyyy-mm-aa hh:mm:ss para time_t.
        time_t sdatetime2time_t (const std::string& datahora)
        {
            if ((&datahora == 0) || (datahora.size() == 0)) return 0;

            time_t t = time (&t);
            struct tm *date = gmtime (&t);
            std::vector<std::string> v;
            split (datahora, ' ', v);

            std::stringstream err;
            err << "invalid argument: " << datahora;
            if (v.size () < 2)
            {
                err << " v.size = " << v.size ();
                throw err.str ();
            }

            std::vector<std::string> v1;
            split (v[0], '-', v1);
            if (v1.size () < 3)
            {
                err << " v1.size = " << v1.size ();
                throw err.str ();
            }

            std::vector<std::string> v2;
            split (v[1], ':', v2);
            if (v2.size () < 3)
            {
                err << " v2.size = " << v2.size ();
                throw err.str ();
            }

            try
            {
                date->tm_mday = std::stoi(v1[2]);
                date->tm_year = std::stoi(v1[0]) - 1900;
                date->tm_hour = std::stoi(v2[0]);
                date->tm_min  = std::stoi(v2[1]);
                date->tm_sec  = std::stoi(v2[2]);
            }
            catch (std::exception& e)
            {
              cerr << "sdatetime2time_t - Bad cast: " << e.what( ) << endl;
            }

            t = mktime (date);
            return t;
        }

        /// Converte string do tipo yyyy-mm-aa para ALT_FILETIME.
        POSIX::ALT_FILETIME strdate2ALT_FILE_TIME (const string& date)
        {
            time_t t = strdate2time_t (date);
            POSIX::ALT_FILETIME aft;
            POSIX::TimeT_To_FILETIME (t, &aft);
            return aft;
        }

        time_t hour2time_t (const std::string& hora)
        {
            time_t t = time (&t);
            struct tm *timeinfo = localtime (&t);
            std::vector<std::string> v;
            split (hora, ':', v);
            try
            {
                timeinfo->tm_hour = stoi(v[0]);
                timeinfo->tm_min  = stoi(v[1]);
                if (v.size () > 2)
                    timeinfo->tm_sec  = stoi(v[2]);
                else
                    timeinfo->tm_sec  = 0;
            }
            catch (std::exception& e)
            {
              cerr << "hora2time_t - Bad cast: " << e.what( ) << endl;
            }

            return mktime (timeinfo);
        }

        std::string time_t2hour (time_t time)
        {
          struct tm * timeinfo;
          char buffer [80];

          timeinfo = localtime (&time);

          strftime (buffer, 80, "%H:%M", timeinfo);
          string s(buffer);
          return s;
        }

        std::string time_t2hhmmss (time_t time, const char* pattern)
        {
          struct tm * timeinfo;
          char buffer [80];

          timeinfo = localtime (&time);

          strftime (buffer, 80, pattern, timeinfo);
          std::string s(buffer);
          return s;
        }

        std::string time_t2hhmmss (time_t time)
        {
          return time_t2hhmmss (time, "%H:%M:%S");
        }

        const char* time_t2weekday (time_t t)
        {
          struct tm * timeinfo;
          timeinfo = localtime (&t);
          return diaSemana[timeinfo->tm_wday];
        }

        string time_t2strdate (time_t time)
        {
          if (time == 0) return "1900-01-01";
          //assert (time != 0);
          struct tm * timeinfo;
          char buffer [80];

          timeinfo = gmtime (&time);

          strftime (buffer, 11, "%Y-%m-%d", timeinfo);
          std::string s(buffer);
          return s;
        }

        std::string stimestamp (std::time_t t)
        {
            t = (t ? t : time (0));
            return time_t2strdate (t) + " " + time_t2hhmmss (t);
        }

        std::string now ()
        {
            time_t t = time (0);
            return time_t2strdate (t) + "_" + time_t2hhmmss (t, "%Hh%Mm%Ss");
        }

        std::string time_t2date (time_t time, const char* pattern)
        {
          if (time == 0) return "01/01/1900";
          //assert (time != 0);
          struct tm * timeinfo;
          char buffer [80];

          timeinfo = gmtime (&time);

          strftime (buffer, 11, pattern, timeinfo);
          std::string s(buffer);
          return s;
        }

        std::string time_t2date (time_t time)
        {
          return time_t2date (time, "%d/%m/%Y");
        }

        /// Produces today date following the specified format.
        /// \return std::string the today date string.
        /// Example: today ("%Y-%m-%d") returns an ISO formatted date string.
        std::string today (const char* pattern)
        {
            return time_t2date (time (0), pattern);
        }

        #ifdef CODE_GEAR
        /// yyyy-mm-dd
        ftime string2ftime (std::string date)
        {
            struct ftime t;
            std::vector<std::string> v;
            split (date, '-', &v);
            try
            {
                t.ft_year  = stoi(v[0]) - 1980;
                t.ft_month = stoi(v[1]) - 1;
                t.ft_day   = stoi(v[2]);
                t.ft_hour = 0;
                t.ft_min  = 0;
                t.ft_tsec  = 0;
            }
            catch (bad_lexical_cast& e)
            {
              cerr << "string2ftime - Bad cast: " << e.what( ) << endl;
            }

            return ftime(t);
        }
        #endif
    }   // namespace cpplib
}       // namespace pensar_digital

