#ifndef MEMORY_WINDOWS_H_INCLUDED
#define MEMORY_WINDOWS_H_INCLUDED

/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */


#include "memory.hpp"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>

#endif

#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        /**
            * Returns the peak (maximum so far) resident set size (physical
            * memory use) measured in bytes, or zero if the value cannot be
            * determined on this OS.
            */
        size_t getPeakRSS()
        {
#if defined(_WIN32)
            /* Windows -------------------------------------------------- */
            PROCESS_MEMORY_COUNTERS info;
            GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
            return (size_t)info.PeakWorkingSetSize;
#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
            /* AIX and Solaris ------------------------------------------ */
            struct psinfo psinfo;
            int fd = -1;
            if ((fd = open("/proc/self/psinfo", O_RDONLY)) == -1)
                return (size_t)0L;      /* Can't open? */
            if (read(fd, &psinfo, sizeof(psinfo)) != sizeof(psinfo))
            {
                close(fd);
                return (size_t)0L;      /* Can't read? */
            }
            close(fd);
            return (size_t)(psinfo.pr_rssize * 1024L);
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
            /* BSD, Linux, and OSX -------------------------------------- */
            struct rusage rusage;
            getrusage(RUSAGE_SELF, &rusage);
#if defined(__APPLE__) && defined(__MACH__)
            return (size_t)rusage.ru_maxrss;
#else
            return (size_t)(rusage.ru_maxrss * 1024L);
#endif
#else
            /* Unknown OS ----------------------------------------------- */
            return (size_t)0L;          /* Unsupported. */
#endif
        }

        // Get available memory in bytes. Windows only.
        size_t get_windows_available_memory()
        {
            MEMORYSTATUSEX status;
            status.dwLength = sizeof(status);
            GlobalMemoryStatusEx(&status);
            return status.ullAvailPhys;
        }

        // Get available memory in bytes. Unix only.
        template <bool UseExceptions = false>
        size_t get_unix_available_memory()
        {
#if defined(_SC_AVPHYS_PAGES)
            return sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE);
#elif defined(_SC_PHYS_PAGES)
            return sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE);
#else
            if (UseExceptions)
            {
                throw std::runtime_error("Failed to determine available memory.");
            }
            else
                return 0L;          /* Unsupported. */
#endif
        }

        // Get available memory in bytes. Linux only.
        size_t get_linux_available_memory()
        {
            std::ifstream meminfo("/proc/meminfo");
            std::string line;
            while (std::getline(meminfo, line))
            {
                std::istringstream iss(line);
                std::string name;
                size_t value;
                if (iss >> name >> value)
                {
                    if (name == "MemAvailable:")
                    {
                        return value * 1024;
                    }
                }
            }
            return 0;
        }

        /// Get available memory in bytes.
        size_t get_available_memory()
        {
#if defined(_WIN32)
            return get_windows_available_memory();
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
            return get_unix_available_memory<true>();
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
            return get_linux_available_memory();
#else
#error "Cannot define get_available_memory () for an unknown OS."
#endif
        }
	}   // namespace cpplib
} // namespace pensar_digital
#endif MEMORY_WINDOWS_H_INCLUDED

