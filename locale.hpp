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