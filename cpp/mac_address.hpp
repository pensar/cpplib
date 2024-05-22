#ifndef MAC_ADDRESS_HPP
#define MAC_ADDRESS_HPP


#include <string>
#include <inttypes.h>


namespace pensar_digital
{
    namespace cpplib
    {

        class MacAddress 
        {
            int64_t mac;

        public:
            MacAddress(int64_t mac) : mac(mac) {}

            operator std::string() const
            {
                char buffer[18];
                sprintf_s(buffer, "%02" PRIx64 ":%02" PRIx64 ":%02" PRIx64 ":%02" PRIx64 ":%02" PRIx64 ":%02" PRIx64,
                    (mac >> 40) & 0xff, (mac >> 32) & 0xff, (mac >> 24) & 0xff,
                    (mac >> 16) & 0xff, (mac >> 8) & 0xff, mac & 0xff);
                return buffer;
            }

            operator int64_t() const 
            {
                return mac;
            }

            operator std::wstring() const
            {
                wchar_t buffer[18];
                swprintf_s(buffer, L"%02" PRIx64 L":%02" PRIx64 L":%02" PRIx64 L":%02" PRIx64 L":%02" PRIx64 L":%02" PRIx64,
                    (mac >> 40) & 0xff, (mac >> 32) & 0xff, (mac >> 24) & 0xff,
                    (mac >> 16) & 0xff, (mac >> 8) & 0xff, mac & 0xff);
                return buffer;
            }
        };
    } // namespace cpplib
} // namespace pensar_digital
#endif // MAC_ADDRESS_HPP
