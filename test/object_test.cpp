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
				Dummy (const Id& id, const String& aname): Object (id), name(aname) {}
                Dummy(const Dummy& d) : Object(d) { name = d.name; }
                Dummy(Dummy&& d) noexcept : Object(d) { name = d.name; }
				Dummy& operator = (const Dummy& d) noexcept {assign (d); return *this;}
				Dummy& operator = (Dummy&& d) noexcept {assign(d); return *this;}
                bool operator == (const Dummy& d) const {return (Object::operator == (d) && (name == d.name));}
                //std::ostream& operator << (std::ostream& os) const { Object::operator << (os); return os;  }
                /// Makes Dummy Streamable.
                friend std::ostream& operator << (std::ostream& os, const Dummy& d) { return os << d.get_id (); }   
                using Object::operator !=;
				virtual ~Dummy () {}
                Dummy assign(const Dummy& d) noexcept { name = d.name; return *this; }

			/// Implicity convertion to object.
            /// \return A copy of the object.
            operator Object () const noexcept { return Object (get_id ()); }
            
            String get_name () const noexcept { return name; }

			protected:
				virtual bool _equals (const Object& o) const {return Object::_equals (o);}
            private:
                
                String name;
        };
        TEST(ObjectClone)
        {
            pd::Object o(42);
            pd::Object o1 = o.clone(o);
            CHECK(o == o1, "0. o == o1 should be true");

            Dummy d(42, "d");
            Dummy d1 = d.clone<Dummy>(d);
            static_assert(Streamable<Dummy>);
            CHECK_EQ(Dummy, d, d1, "1. d == d1 should be true");
        }
        TEST_END(ObjectClone)
    }
}
