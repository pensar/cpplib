// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../cpplib/cpp/e-mail.hpp"

namespace pensar_digital
{
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        namespace contact
        {
            TEST(Email, true)
                CHECK(is_valid_email_address(W("x@x.com")), W("0"));
                Email email(W("x@x.com"));
                CHECK_EQ(S, email.str (), W("x@x.com"), W("1"));

            TEST_END(Email)
        } // namespace contact
    } // namespace cpplib
} // namespace pensar_digital