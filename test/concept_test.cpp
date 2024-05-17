// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../cpp/s.hpp"
#include "../cpp/concept.hpp"
#include "../cpp/constraint.hpp"

namespace pensar_digital
{
    namespace test = pensar_digital::unit_test;
    using namespace pensar_digital::unit_test;
    namespace cpplib
    {
        template <typename... Args>
        class Checkable0
        {
            public:
                virtual bool ok (Args& ... args) const { return true; }
        };
        
        class Checkable1
        {
            public:
                virtual bool ok () const noexcept { return true; }
        };

        TEST(CheckableConcept, true)
            static_assert(Checkable<Checkable0<int>, int>);
            static_assert(Checkable<Checkable0<S>, S>);
            static_assert(Checkable<Checkable0<>>);
            static_assert(Checkable<Checkable1>);
            static_assert(Checkable<StringConstraint, S>);
        TEST_END(CheckableConcept)
    }
}
