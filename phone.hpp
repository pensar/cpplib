// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef PHONE_HPP
#define PHONE_HPP

#include "s.hpp"
#include "contact.hpp"

#include <sstream>

namespace pensar_digital
{
    namespace cpplib
    {
        namespace contact
        {
            namespace pd = pensar_digital::cpplib;

            template <typename C = char>
            struct PhoneNumber : public Contact
            {
                typedef CS<0,  3, C> CountryCode;
                typedef CS<0,  4, C> AreaCode; // Referred as Identification Code in E.164 standard. (https://en.wikipedia.org/wiki/E.164)
                typedef CS<0, 10, C> Number; // E.164 standard allows for a maximum of 15 digits. (https://en.wikipedia.org/wiki/E.164)

                CountryCode mcountry_code;
                AreaCode    mareacode;
                Number      mnumber;
                ContactQualifier mqualifier;
                PhoneNumber(const CountryCode& cc = empty<C>(), const AreaCode& ac = empty<C>(), const Number& n = empty<C>(), ContactQualifier cq = ContactQualifier::Business)
                    : mcountry_code(cc), mareacode(ac), mnumber(n), mqualifier(cq) {}

                inline bool equal_country_code(const PhoneNumber& other) const noexcept
                {
                    return mcountry_code == other.mcountry_code;
                }

                inline bool equal_area_code(const PhoneNumber& other) const noexcept
                {
                    return mareacode == other.mareacode;
                }

                inline bool equal_number(const PhoneNumber& other) const noexcept
                {
                    return mnumber == other.mnumber;
                }

                inline bool operator==(const PhoneNumber& other) const noexcept
                {
                    return equal_country_code(other) && equal_area_code(other) && equal_number(other);
                }

                inline bool operator!=(const PhoneNumber& other) const noexcept
                {
                    return !(*this == other);
                }

                inline std::string json() const noexcept
                {
                    std::stringstream ss;
                    ss << "{ \"class\" : \"" << class_name<PhoneNumber<char>>();
                    ss << ", \"country_code\" : " << mcountry_code << ", \"area_code\" : " << mareacode << ", \"number\" : " << mnumber << " }";
                    return ss.str();
                }

                inline std::wstring wjson() const noexcept
                {
                    std::wstringstream ss;
                    ss << L"{ \"class\" : \"" << class_name<PhoneNumber<wchar_t>>();
                    ss << L", \"country_code\" : " << mcountry_code << L", \"area_code\" : " << mareacode << L", \"number\" : " << mnumber << L" }";
                    return ss.str();
                }
            };
        }   // namespace contact
    }       // namespace cpplib
}           // namespace pensar_digital

#endif // PHONE_HPP
