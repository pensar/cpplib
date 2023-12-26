// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DATE_HPP
#define DATE_HPP

#include <chrono>

#include "concept.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        typedef std::chrono::year_month_day Date;

        // Ensure the class is trivially copyable and standard layout.
        static_assert (StdLayoutTriviallyCopyable<Date>);

    } // namespace contact
} // namespace pensar_digital

#endif // DATE_HPP
