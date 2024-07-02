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
                #define DEFAULT_MAX_NAME_LENGTH 255
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

            static OS os ()
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

            template<OS OSTtype>
            size_t get_max_name_length ()
            {
                return DEFAULT_MAX_NAME_LENGTH;
            }

            // Specialization for Windows
            template<> size_t get_max_name_length<OS::Windows> () {	return 255; /* NTFS */ }

            // Specialization for Linux
            template<> size_t get_max_name_length<OS::Linux  > () { return 255; /* ext4 */ }

            // Specialization for MacOS
            template<> size_t get_max_name_length<OS::MacOS  > () { return 255; /* APFS or HFS+ */ }

            // Specialization for iOS
            template<> size_t get_max_name_length<OS::IOS    > () { return 255; /* APFS */ }

            // Specialization for Android
            template<> size_t get_max_name_length<OS::Android> () { return 255; /* Android can use a variety of filesystems, including ext4 and F2FS */ }

            // Specialization for Other
            template<> size_t get_max_name_length<OS::Other  > () { return DEFAULT_MAX_NAME_LENGTH; /* Default or unknown filesystem */ }

            #define DEFAULT_MAX_PATH 260

            template<OS OSTtype>
            size_t get_max_path ()
			{
				return DEFAULT_MAX_PATH;
			}
            
            // Specialization for Windows
            template<> size_t get_max_path<OS::Windows> () { return MAX_PATH; /* MAX_PATH */ }

            // Specialization for Linux
            template<> size_t get_max_path<OS::Linux  > () 
            { 
                #define DEFAULT_LINUX_MAX_PATH 4096

                #ifdef PATH_MAX 
                    return PATH_MAX; 
                #else
                    return DEFAULT_LINUX_MAX_PATH;
                #endif
            }

            // Specialization for MacOS
            template<> size_t get_max_path<OS::MacOS  > () 
            { 
                #define DEFAULT_MACOS_MAX_PATH 1024;
                return DEFAULT_MACOS_MAX_PATH;
            }

            // Specialization for iOS.
            template<> size_t get_max_path<OS::IOS    > () 
			{ 
				#define DEFAULT_IOS_MAX_PATH 1024;
				return DEFAULT_IOS_MAX_PATH;
			}

            // Specialization for Android.
            template<> size_t get_max_path<OS::Android> ()
            {
                #define DEFAULT_ANDROID_MAX_PATH 4096;
					return DEFAULT_ANDROID_MAX_PATH;
		    }

            static inline S endianess_name () { return (std::endian::native == std::endian::little) ? W("Little Endian") : W("Big Endian"); }

            static std::endian endianess()
            {
                return (std::endian::native == std::endian::little) ? std::endian::little : std::endian::big;
            }

            static inline constexpr C path_separator ()
            {
                #if defined(_WIN32) || defined(_WIN64)
                return W('\\');
                #else
                        return W('/');
                #endif
            }
            inline const static S LF = LINE_FEED;

            // Common constraints
            static constexpr size_t MAX_NAME_LENGTH = 255;

            // Helper function to check the first character and max size
            bool is_name_valid_common(const S& name) {
                return !name.empty() && name[0] != '.' && name.size() <= MAX_NAME_LENGTH;
            }

            template<OS OSTtype>
            bool is_valid_file_name(const S& file_name)
			{
				throw "Unknown operating system.";
			}

            // Specialization for Windows
            template<>
            bool is_valid_file_name<OS::Windows> (const S& file_name) 
            {
                static const S invalidChars = W("<>:\"/\\|?*");
                return is_name_valid_common(file_name) && file_name.find_first_of(invalidChars) == S::npos;
            }

            // Specialization for Linux
            template<>
            bool is_valid_file_name<OS::Linux> (const S& file_name) 
			{
				return is_name_valid_common(file_name) && file_name.find(path_separator ()) == S::npos && file_name.find('\0') == S::npos;
			}

            // Specialization for MacOS
            template<>
            bool is_valid_file_name<OS::MacOS> (const S& file_name)
            {
				return is_name_valid_common(file_name) && file_name.find(path_separator ()) == S::npos && file_name.find('\0') == S::npos;
			}

            // Specialization for iOS
            template<>
            bool is_valid_file_name<OS::IOS> (const S& file_name)
			{
                return is_name_valid_common(file_name) && file_name.find(path_separator ()) == S::npos && file_name.find('\0') == S::npos;
            }

            // Specialization for Android
            template<>
            bool is_valid_file_name<OS::Android> (const S& file_name)
            {
				return is_name_valid_common(file_name) && file_name.find(path_separator ()) == S::npos && file_name.find('\0') == S::npos;
			}


            template<OS OSTtype>
            bool is_valid_path_name(const S& path_name)
            {
                throw "Unknown operating system.";
            }

            // Specialization for Windows
            template<>
            bool is_valid_path_name<OS::Windows> (const S& path_name) 
            {
                static const S invalidChars = W("<>:\"|?*");
                return is_name_valid_common(path_name) && path_name.find_first_of(invalidChars) == S::npos;
            }

			// Specialization for Linux
            template<>
            bool is_valid_path_name <OS::Linux> (const S& path_name) 
            {
                return is_name_valid_common(path_name) && path_name.find('\0') == std::string::npos;
            }

            // Specialization for MacOS
            template<>
            bool is_valid_path_name<OS::MacOS> (const S& path_name)
			{
				return is_name_valid_common(path_name) && path_name.find('\0') == std::string::npos;
			}

            // Specialization for iOS
            template<>
            bool is_valid_path_name <OS::IOS> (const S& path_name) 
            {
                return is_name_valid_common(path_name) && path_name.find('\0') == std::string::npos;
            }

            // Specialization for Android   
            template<>
            bool is_valid_path_name<OS::Android> (const S& path_name)
			{
				return is_name_valid_common(path_name) && path_name.find('\0') == std::string::npos;
			}

            #if defined(_WIN32) || defined(_WIN64)                
                #pragma comment(lib, "IPHLPAPI.lib") // Link with Iphlpapi.lib
            #endif          

            template <OS OSTtype>
            std::vector<MacAddress> mac_addresses()
			{
				throw "Unknown operating system.";
			}

            // Specialization for Windows
            template<>
            std::vector<MacAddress> mac_addresses<OS::Windows> ()
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

            // Specialiation for MacOS
            template<>
            std::vector<MacAddress> mac_addresses<OS::MacOS> ()
            {
                // Your implementation here
				return std::vector<MacAddress>();
			}

            // Specialization for Linux.
            template<>
            std::vector<MacAddress> mac_addresses<OS::Linux> ()
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
        };  // System class


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
