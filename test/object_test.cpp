// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#include "../../unit-test/test.hpp"
#include "../string_def.hpp"
#include "../string_util.hpp"
#include "../object.hpp"
#include <memory>

namespace pensar_digital
{
    namespace pd = pensar_digital::cpplib;
    namespace unit_test
    {
        /// <summary>
        /// Dummy class is streamable and comparable.
        /// </summary>
        class Dummy: public Object 
        {
            public:
				Dummy (const Id& id): Object (id) {}
				Dummy (const Dummy& d): Object (d) {}
				Dummy (Dummy&& d) noexcept : Object (d) {}
				Dummy& operator = (const Dummy& d) noexcept {Object::operator = (d); return *this;}
				Dummy& operator = (Dummy&& d) noexcept {Object::operator = (d); return *this;}
                bool operator == (const Dummy& d) const {return Object::operator == (d);}
                //std::ostream& operator << (std::ostream& os) const { Object::operator << (os); return os;  }
                /// Makes Dummy Streamable.
                friend std::ostream& operator << (std::ostream& os, const Dummy& d) { return os << d.get_id (); }   
                using Object::operator !=;
				virtual ~Dummy () {}

            virtual Dummy assign (const Dummy& d) noexcept {return Dummy (d);}
            virtual Dummy assign (Dummy&& d) noexcept {return Dummy (d);}
            virtual Dummy clone (const Dummy& d) {return Dummy (d);}

			/// Implicity convertion to object.
            /// \return A copy of the object.
            operator Object () const {return Object (get_id ());}
            
			protected:
				virtual bool _equals (const Object& o) const {return Object::_equals (o);}
        };
        TEST(ObjectClone)
        {
            pd::Object o(42);
            pd::Object o1 = o.clone(o);
            CHECK(o == o1, "0. o == o1 should be true");

            Dummy d(42);
            Dummy d1 = d.clone(d);
            static_assert(Streamable<Dummy>);
            CHECK_EQ(Dummy, d, d1, "1. d == d1 should be true");
        }
        TEST_END(ObjectClone)
    }
}
