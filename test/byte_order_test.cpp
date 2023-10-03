// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../byte_order.hpp"

#include <span>
#include <bit>   


namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace pd = pensar_digital::cpplib;
    namespace cpplib
    {
        template <typename U>
        void test_byte_order_conversion(Test& t, std::vector<U> a, std::vector<U> a4, uint_fast8_t count = 0)
        {
            std::vector<U> a6 = a;
            auto data = std::span{ a };
            // Get the bytes of the array as writable bytes.
            auto b = std::as_writable_bytes(data);
            auto size = sizeof(U);
            pd::convert<>(b, size, pd::native_byte_order, pd::big_address_8_byte_order);

            // Allocate a new array of U named c of the same size as a.
            auto c = new U[a.size()];   
            
            std::memcpy(c, b.data(), b.size());
            std::vector<U> a3(c, c + a.size ());
            t.check<U>(a3, a4, pd::to_string(count++) + ".", __FILE__, __LINE__);

            pd::convert<>(b, size, pd::big_address_8_byte_order, pd::native_byte_order);
            std::memcpy(c, b.data(), b.size());
            std::vector<U> a5(c, c + a.size());
            t.check<U> (a5, a6, pd::to_string(count++) + ".", __FILE__, __LINE__);
        };

        TEST(ByteOrder, true)
            std::vector<uint8_t> a1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
            std::vector<uint8_t> a2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
            test_byte_order_conversion<uint8_t > (*this, a1, a2, 0);

            std::vector<uint16_t> a3 = { 0, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008 };
            std::vector<uint16_t> a4 = { 0, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700, 0x0800 };
			test_byte_order_conversion<uint16_t> (*this, a3, a4, 1);
            
            std::vector<uint32_t> a5 = { 0, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008 };   
            std::vector<uint32_t> a6 = { 0, 0x01000000, 0x02000000, 0x03000000, 0x04000000, 0x05000000, 0x06000000, 0x07000000, 0x08000000 };
            test_byte_order_conversion<uint32_t> (*this, a5, a6, 2);

            std::vector<uint64_t> a7 = { 0, 0x0000000000000001, 0x0000000000000002, 0x0000000000000003, 0x0000000000000004, 0x0000000000000005, 0x0000000000000006, 0x0000000000000007, 0x0000000000000008 };
            std::vector<uint64_t> a8 = { 0, 0x0100000000000000, 0x0200000000000000, 0x0300000000000000, 0x0400000000000000, 0x0500000000000000, 0x0600000000000000, 0x0700000000000000, 0x0800000000000000 };
			test_byte_order_conversion<uint64_t> (*this, a7, a8, 3);

            std::vector<int8_t>  a9 = { -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
            std::vector<int8_t> a10 = { -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2 ,3, 4, 5, 6, 7, 8 };
            test_byte_order_conversion<int8_t >(*this, a9, a10, 4);

            TEST_END(ByteOrder)
    }
}
