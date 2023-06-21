// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef LOCALE_HPP
#define LOCALE_HPP

#include "object.hpp"
#include "string_def.hpp"
#include "language.hpp"
#include "country.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        class Locale : public Object
        {
        public:
            const static Version VERSION = 1;
            const static Version PUBLIC_INTERFACE_VERSION = 1;
            const static Version PROTECTED_INTERFACE_VERSION = 1;
            const static Version PRIVATE_INTERFACE_VERSION = 1;

            virtual Version get_version() const noexcept { return VERSION; }
            virtual Version get_public_interface_version() const noexcept { return PUBLIC_INTERFACE_VERSION; }
            virtual Version get_protected_interface_version() const noexcept { return PROTECTED_INTERFACE_VERSION; }
            virtual Version get_private_interface_version() const noexcept { return PRIVATE_INTERFACE_VERSION; }

            /** Default constructor */
            Locale(const typename Id aid = NULL_ID) {}
            /** Default destructor */
            virtual ~Locale() {}
            /** Copy constructor
             *  \param other Object to copy from
             */
            Locale(const Locale& other) {}
            /** Assignment operator
             *  \param other Object to assign from
             *  \return A reference to this
             */
            Locale& operator=(const Locale& other) {}
            /** Access name
             * \return The current value of name
             */
            String get_name() { return ""; }
            /** Access language
             * \return The current value of language
             */
            Language* get_language() { return language; }

            /** Access country
             * \return The current value of country
             */
            Country* get_country() { return country; }

        protected:
        private:
            Language* language; //!< Member variable "language"
            Country* country; //!< Member variable "country"
        };
    }
}
#endif // LOCALE_HPP
