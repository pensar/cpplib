// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../e-mail.hpp"

namespace pensar_digital
{
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        namespace contact
        {
            TEST(Email, true)
                CHECK(is_valid_email_address("x@x.com"), "0");
            TEST_END(Email)
        } // namespace contact
    } // namespace cpplib
} // namespace pensar_digital