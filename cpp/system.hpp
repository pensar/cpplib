// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef  SYSTEM_HPP
#define SYSTEM_HPP

#ifdef _WIN32 
    // Win32
    #define _WINSOCKAPI_	// stops windows.h including winsock.h
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <intrin.h>
	#include <windows.h>
#elif __APPLE__
    // MacOS
    #include <sys/socket.h>
    #include <sys/sysctl.h>
    #include <net/if.h>
    #include <net/if_dl.h>
	#include <sys/statvfs.h>
#elif __linux__
    // Linux
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
    #include <linux/if.h>
    #include <linux/sockios.h>
    #include <sys/statvfs.h>
#endif

#include "s.hpp"
#include "mac_address.hpp"
//#include "error.hpp"

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
        #if defined(_WIN32) || defined(_WIN64)
            #include <windows.h>
        #elif __linux__
            #include <unistd.h>
            #include <sys/utsname.h>
            #define MAX_PATH         4096 // Max path length in Linux
            #define MAX_UNC_PATH     4096 // Max UNC path length.
            #define PATH_SEPARATOR W('/')
        #endif

        static S os_name()
        {
            #if defined(_WIN32) || defined(_WIN64)
                return W("Windows");
            #elif defined(__APPLE__) && defined(TARGET_OS_IOS)
                return W("iOS");
            #elif defined(__ANDROID__)
                return W("Android");
            #elif defined(__linux__)
                return W("Linux");
            #else
                return W("Other");
            #endif
        }

        enum class OS
        {
            Windows,
            Linux,
            MacOS,
            IOS,
            Android,
            Other
        };

        inline static constexpr OS os()
        {
            #if defined(_WIN32) || defined(_WIN64)
                return OS::Windows;
            #elif defined(__APPLE__) && defined(TARGET_OS_IOS)
                return OS::IOS;
            #elif defined(__ANDROID__)
                return OS::Android;
            #elif defined(__linux__)
                return OS::Linux;
            #else
                return OS::Other;
            #endif
        }

        static S os_version()
        {
            #if defined(_WIN32) || defined(_WIN64)
                OSVERSIONINFO info;
                ZeroMemory(&info, sizeof(OSVERSIONINFO));
                info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                S s = to_string(info.dwMajorVersion);
                s += W(".");
                s += to_string(info.dwMinorVersion);
                S r(s.c_str());
                return r;
            #elif __linux__
                struct utsname buffer;
                uname(&buffer);
                return buffer.release;
            #else
                return W("Unknown");
            #endif
        }

         class BaseSystem
        {
            public:
                // Common constraints
                inline static constexpr size_t DEFAULT_MAX_NAME_LENGTH = 255;
                inline static constexpr size_t DEFAULT_MAX_PATH = 260;

                inline static S endianess_name() noexcept { return (std::endian::native == std::endian::little) ? W("Little Endian") : W("Big Endian"); }

                static std::endian endianess() noexcept
                {
                    return (std::endian::native == std::endian::little) ? std::endian::little : std::endian::big;
                }

                // Helper function to check the first character and max size
                inline static bool is_name_valid_common(const S& name) {
                    return !name.empty() && name[0] != '.' && name.size() <= DEFAULT_MAX_NAME_LENGTH;
                }
        };

        template<OS OSTtype>
        class System : public BaseSystem
        {
            // Throws an exception if the operating system is not supported.
            static_assert (OSTtype == OS::Windows || OSTtype == OS::Linux || OSTtype == OS::MacOS || OSTtype == OS::IOS || OSTtype == OS::Android, W("Unsupported operating system."));            
        };

        // Specialization for Linux
		template<>
        class System<OS::Linux> : public BaseSystem
        {
            public:
                inline static const S LINE_FEED = W("\n");

                inline static size_t get_max_name_length () noexcept { return 255; /* ext4 */ }
                inline static size_t get_max_path () noexcept
                {
                    #define DEFAULT_LINUX_MAX_PATH 4096

                    #ifdef PATH_MAX 
                        return PATH_MAX;
                    #else
                        return DEFAULT_LINUX_MAX_PATH;
                    #endif
                }

                inline static constexpr C path_separator() { return W('/'); }
                inline static bool is_valid_file_name (const S& file_name)
                {
                    return is_name_valid_common (file_name) && file_name.find (path_separator ()) == S::npos && file_name.find ('\0') == S::npos;
                }

                inline static bool is_valid_path (const S& path_name)
                {
                    return is_name_valid_common(path_name) && path_name.find('\0') == S::npos;
                }

                inline static std::vector<MacAddress> mac_addresses ()
                {

                    #ifdef __linux__

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
                            return mac_addresses;
                        #endif
                    }   // mac_addresses
                #ifdef __linux__
                    static inline S cpu_id()
                    {
                        S cpu_id;
                        InFStream cpuinfo (W("/proc/cpuinfo"));
                        if (cpuinfo) 
                        {
                            S line;
                            while (std::getline (cpuinfo, line)) {
                                if (line.substr (0, 9) == W("processor"))
                                {
                                    cpu_id = line;
                                    break;
                                }
                            }
                        }
                        return cpu_id;
                    }
            #endif
        };

        // Specialization for iOS
        template<> 
        class System<OS::IOS> : public BaseSystem
        {
            public:
                inline static const S LINE_FEED = W("\n");
                inline static constexpr size_t get_max_name_length () noexcept { return 255; /* APFS */ }
                
                inline static constexpr size_t get_max_path () noexcept
				{
					#define DEFAULT_IOS_MAX_PATH 4096
					return DEFAULT_IOS_MAX_PATH;
				}
                inline static constexpr C path_separator () noexcept { return W('/'); }
                inline static bool is_valid_file_name (const S& file_name)
				{
					return is_name_valid_common (file_name) && file_name.find (path_separator ()) == S::npos && file_name.find ('\0') == S::npos;
				}

                inline static bool is_valid_path (const S& path_name)
                {
                    return is_name_valid_common (path_name) && path_name.find (W('\0')) == S::npos;
				}
        };

        // Specialization for Android
        template<> 
        class System<OS::Android> : public BaseSystem
		{
			public:
                inline static const S LINE_FEED = W("\n");
                inline static constexpr size_t get_max_name_length() noexcept { return 255; /* Android can use a variety of filesystems, including ext4 and F2FS */}
                
                inline static constexpr size_t get_max_path () noexcept
                {
                    #define DEFAULT_ANDROID_MAX_PATH 4096
					return DEFAULT_ANDROID_MAX_PATH;
				}

                inline static constexpr C path_separator () noexcept { return W('/'); }
                inline static bool is_valid_file_name(const S& file_name)
                {
                    return is_name_valid_common(file_name) && file_name.find(path_separator()) == S::npos && file_name.find('\0') == S::npos;
                }

                inline static bool is_valid_path(const S& path_name)
				{
					return is_name_valid_common (path_name) && path_name.find (W('\0')) == S::npos;
				}
        };

        // Specialization for MacOS
        template<>
        class System<OS::MacOS> : public BaseSystem
		{
			public:
                inline static const S LINE_FEED = W("\n");
                inline static constexpr size_t get_max_name_length () noexcept { return 255; /* APFS */ }
				inline static constexpr size_t get_max_path () noexcept
				{
					#define DEFAULT_MACOS_MAX_PATH 1024
					return DEFAULT_MACOS_MAX_PATH;
				}
                inline static constexpr C path_separator () noexcept { return W('/'); }
                inline static bool is_valid_file_name (const S& file_name)
                {
                    return is_name_valid_common (file_name) && file_name.find (path_separator ()) == S::npos && file_name.find ('\0') == S::npos;
				}

                inline static bool is_valid_path (const S& path_name)
                {
                    return is_name_valid_common (path_name) && path_name.find (W('\0')) == S::npos;
				}

                inline static std::vector<MacAddress> mac_addresses ()
                {
                    // Your implementation here
                    return std::vector<MacAddress>();
                }
        };

        // Specialization for Windows
        template<>
        class System <OS::Windows> : public BaseSystem
        {
            public:
                inline static const S LINE_FEED = W("\r\n");
                inline static size_t get_max_name_length () noexcept { return 255; /* NTFS */ }
                inline static size_t get_max_path        () noexcept { return MAX_PATH; /* MAX_PATH */ }

                inline static constexpr C path_separator () noexcept { return W('\\'); }

                inline static bool is_valid_path (const S& path_name)
				{
					return is_name_valid_common (path_name) && path_name.find (W('\0')) == S::npos;
				}

                inline static bool is_valid_file_name (const S& file_name)
                {
                    static const S invalidChars = W("<>:\"/\\|?*");
                    return is_name_valid_common (file_name) && file_name.find_first_of (invalidChars) == S::npos;
                }


            #if defined(_WIN32) || defined(_WIN64)                
                #pragma comment(lib, "IPHLPAPI.lib") // Link with Iphlpapi.lib
            #endif          

            inline static std::vector<MacAddress> mac_addresses ()
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

            inline static S cpu_id()
            {
                std::array<int, 4> cpuid;
                __cpuid (cpuid.data (), 0);
                S cpu_id = to_string (cpuid[1]) + to_string (cpuid[3]);
                return cpu_id;
            }
        };  // Windows System class.

        using Sys = System<os()>;
        inline const static S LF = Sys::LINE_FEED;
        inline static bool is_valid_path(const S& path_name) { return Sys::is_valid_file_name(path_name); }
		inline static bool is_valid_file_name(const S& file_name) { return Sys::is_valid_file_name(file_name); }
        inline static std::vector<MacAddress> mac_addresses() { return Sys::mac_addresses(); }
        inline static S cpu_id() { return Sys::cpu_id(); }

        // Gets file name from a path string.
        inline static S file_name(const S& path_name)
        {
            size_t pos = path_name.find_last_of(Sys::path_separator());
            if (pos == S::npos)
                return path_name;
            return path_name.substr(pos + 1);
        }
    } // namespace cpplib
} // namespace pensar_digital
#endif	// SYSTEM_HPP
