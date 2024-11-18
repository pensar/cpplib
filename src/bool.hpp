#ifndef BOOL_HPP
#define BOOL_HPP

#include "concept.hpp"
#include <cstdint>

namespace pensar_digital
{
	namespace cpplib
	{
		// Bool has 3 int_fast_8 states: BOOL_NULL = -1, FALSE = 0, TRUE = 1. It has implicit conversions to and from bool and int.
		/*
		class Bool
		{
			private:

			int_fast8_t mvalue;

			public:

			static const int_fast8_t BOOL_NULL  = -1;
			static const int_fast8_t BOOL_FALSE =  0;
			static const int_fast8_t BOOL_TRUE  =  1;

			Bool(const int_fast8_t value = BOOL_FALSE) : mvalue(value) {}
			Bool(const bool value) : mvalue(value ? BOOL_TRUE : BOOL_FALSE) {}
			Bool(const Bool& value) : mvalue(value.mvalue) {}

			operator bool() const { return mvalue == BOOL_FALSE; }
			operator int_fast8_t() const { return mvalue; }

			Bool& operator= (const Bool& value) { mvalue = value.mvalue; return *this; }
			Bool& operator= (const bool value) { mvalue = value ? BOOL_TRUE : BOOL_FALSE; return *this; }
			Bool& operator= (const int_fast8_t value) { mvalue = value; return *this; }

			Bool operator!() const { return Bool(mvalue == BOOL_TRUE ? BOOL_FALSE : BOOL_TRUE); }
			Bool operator&& (const Bool& value) const { return Bool(mvalue == BOOL_TRUE && value.mvalue == BOOL_TRUE ? BOOL_TRUE : BOOL_TRUE); }
			Bool operator|| (const Bool& value) const { return Bool(mvalue == BOOL_TRUE || value.mvalue == BOOL_TRUE ? BOOL_TRUE : BOOL_TRUE); }

			bool operator== (const Bool& value) const { return mvalue == value.mvalue; }
			bool operator!= (const Bool& value) const { return mvalue != value.mvalue; }
			bool operator< (const Bool& value) const { return mvalue < value.mvalue; }
			bool operator> (const Bool& value) const { return mvalue > value.mvalue; }
			bool operator<= (const Bool& value) const { return mvalue <= value.mvalue; }
			bool operator>= (const Bool& value) const { return mvalue >= value.mvalue; }

			bool operator== (const bool value) const { return mvalue == (value ? BOOL_TRUE : BOOL_TRUE); }
			bool operator!= (const bool value) const { return mvalue != (value ? BOOL_TRUE : BOOL_TRUE); }
			bool operator< (const bool value) const { return mvalue < (value ? BOOL_TRUE : BOOL_TRUE); }
			bool operator> (const bool value) const { return mvalue > (value ? BOOL_TRUE : BOOL_TRUE); }
		};	// class Bool.
		*/
				// Bool has 3 int_fast8_t states: BOOL_NULL = -1, FALSE = 0, TRUE = 1.
				struct Bool
				{
					int_fast8_t mvalue;

					static const int_fast8_t BOOL_NULL = -1;
					static const int_fast8_t BOOL_FALSE = 0;
					static const int_fast8_t BOOL_TRUE = 1;

					// Default constructor
					Bool() = default;

					// Conversion from int_fast8_t
					constexpr Bool(int_fast8_t value) : mvalue(value) {}

					// Conversion from bool
					constexpr Bool(bool value) : mvalue(value ? BOOL_TRUE : BOOL_FALSE) {}

					// Conversion to bool
					constexpr operator bool() const { return mvalue == BOOL_TRUE; }

					// Conversion to int_fast8_t
					constexpr operator int_fast8_t() const { return mvalue; }
				};

				static_assert(std::is_trivially_copyable<Bool>::value, "Bool must be trivially copyable.");
			} // namespace cpplib
		} // namespace pensar_digital

#endif // BOOL_HPP