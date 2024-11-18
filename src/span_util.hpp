#ifndef SPAN_UTIL
#define SPAN_UTIL

#include <span>
#include <array>

#include "constant.hpp"
#include "memory.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		// Add left and right spans and returns the sum. Avoids unnecessary allocations.
		ConstByteSpan add (const ConstByteSpan& left, const ConstByteSpan& right)
		{
			Bytes v(left.size() + right.size());

			ConstByteSpan r = v;
			return r; // todo: this is not correct since v will be deallocated and the span will be invalid.
		}

	}
}

#endif // !SPAN_UTIL

