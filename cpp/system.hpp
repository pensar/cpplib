// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef  SYSTEM_HPP
#define SYSTEM_HPP

#ifdef _WIN32
    // Win32
    #define _WINSOCKAPI_
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <intrin.h>
#elif __APPLE__
    // MacOS
    #include <sys/socket.h>
    #include <sys/sysctl.h>
    #include <net/if.h>
    #include <net/if_dl.h>
#elif __linux__
    // Linux
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
    #include <linux/if.h>
    #include <linux/sockios.h>
#endif

#include "s.hpp"
#include "mac_address.hpp"

#include <iostream>
#include <type_traits>
#include <string>
#include <bit>  // std::endian
#include <cstdint>
#include <cstring> // for strcpy
#include <cstdio>  // for sscanf
#include <array>
#include <fstream>

namespace pensar_digital
{
	namespace cpplib
	{


#ifdef _WIN32 
            #include <windows.h>
            #define LINE_FEED W("\r\n")
        #elif __linux__
            #include <unistd.h>
            #include <sys/utsname.h>
            #define MAX_PATH         4096 // Max path length in Linux
            #define MAX_UNC_PATH     4096 // Max UNC path length.
            #define PATH_SEPARATOR W('/')
            #define LINE_FEED W("\n")
        #endif


        class System 
        {
            public:

            static S os_name() 
            {
                #ifdef _WIN32   
                    return W("Windows");
                #elif __linux__ 
                    return   W("Linux");
                #else 
                    return W(Other);
                #endif
            }

            static S os_version() 
            {
                #ifdef _WIN32
                    OSVERSIONINFO info;
                    ZeroMemory(&info, sizeof(OSVERSIONINFO));
                    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                    S s = to_string(info.dwMajorVersion);
                    s += W(".");
                    s += to_string(info.dwMinorVersion);
                    S r(s.c_str ());
                return r;

                #elif __linux__
                    struct utsname buffer;
                    uname(&buffer);
                    return buffer.release;
                #else
                return W("Unknown");
                #endif
            }

            static inline S endianess () { return (std::endian::native == std::endian::little) ? W("Little Endian") : W("Big Endian"); }
            static inline C path_separator ()
            {
                #ifdef _WIN32
                return W('\\');
                #else
                        return W('/');
                #endif
            }
            inline const static S LF = LINE_FEED;
            
        };

#ifdef _WIN32
#pragma comment(lib, "IPHLPAPI.lib")

        static inline std::vector<MacAddress> win32_mac_addresses()
        {
            std::vector<MacAddress> mac_addresses;
            PIP_ADAPTER_INFO AdapterInfo;
            DWORD dwBufLen = sizeof(AdapterInfo);
            char* mac_addr = new char[18];

            AdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
            if (AdapterInfo == NULL) {
                printf("Error allocating memory needed to call GetAdaptersinfo\n");
                return mac_addresses;
            }

            if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
                AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
                if (AdapterInfo == NULL) {
                    printf("Error allocating memory needed to call GetAdaptersinfo\n");
                    return mac_addresses;
                }
            }

            if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
                PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
                do {
                    sprintf_s(mac_addr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
                        pAdapterInfo->Address[0], pAdapterInfo->Address[1],
                        pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                        pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
                    printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);

                    int64_t mac;
                    sscanf_s(mac_addr, "%llx", &mac);
                    mac_addresses.push_back(MacAddress(mac));

                    pAdapterInfo = pAdapterInfo->Next;
                } while (pAdapterInfo);
            }
            free(AdapterInfo);

            return mac_addresses;
        }
#elif __APPLE__
        static inline std::vector<MacAddress> macos_mac_addresses()
        {
            // Your implementation here
            return std::vector<MacAddress>();
        }
#elif __linux__
        static inline std::vector<MacAddress> linux_mac_addresses()
        {
            std::vector<MacAddress> mac_addresses;
            struct ifreq ifr;
            struct ifconf ifc;
            char buf[1024];

            int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
            if (sock == -1) { /* handle error*/ };

            ifc.ifc_len = sizeof(buf);
            ifc.ifc_buf = buf;
            if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

            struct ifreq* it = ifc.ifc_req;
            const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

            for (; it != end; ++it) {
                strcpy(ifr.ifr_name, it->ifr_name);
                if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
                    if (!(ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                        if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                            unsigned char mac_address[6];
                            memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);

                            int64_t mac;
                            sscanf_s((char*)mac_address, "%llx", &mac);
                            mac_addresses.push_back(MacAddress(mac));
                        }
                    }
                }
                else { /* handle error */ }
            }

            return mac_addresses;
        }
#else
#error Platform not supported
#endif

        static inline std::vector<MacAddress> mac_addresses()
        {
#ifdef _WIN32
            return win32_mac_addresses();
#elif __APPLE__
            return macos_mac_addresses();
#elif __linux__
            return linux_mac_addresses();
#else
#error Platform not supported
#endif
        }

#ifdef _WIN32
        static inline std::string windows_cpu_id() 
        {
            std::array<int, 4> cpuid;
            __cpuid(cpuid.data(), 0);
            std::string cpu_id = std::to_string(cpuid[1]) + std::to_string(cpuid[3]);
            return cpu_id;
        }
#elif __linux__
        static inline std::string linux_cpu_id() 
        {
            std::string cpu_id;
            std::ifstream cpuinfo("/proc/cpuinfo");
            if (cpuinfo) {
                std::string line;
                while (std::getline(cpuinfo, line)) {
                    if (line.substr(0, 9) == "processor") 
                    {
                        cpu_id = line;
                        break;
                    }
                }
            }
            return cpu_id;
        }
#endif

        static inline std::string cpu_id() 
        {
            #ifdef _WIN32
                return windows_cpu_id();
            #elif __linux__
                return linux_cpu_id();
            #else
                return "Unsupported platform";
            #endif
        }


    } // namespace cpplib
} // namespace pensar_digital
#endif	// SYSTEM_HPP
