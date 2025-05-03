#ifndef EQUAL_HPP
#define EQUAL_HPP

#include "concept.hpp"
#include "constant.hpp"
#include <type_traits>
#include <cstring>

namespace pensar_digital
{
    namespace cpplib
    {
        // TriviallyHashComparable concept.
        template <class T>
        concept TriviallyHashComparable = HasStdLayoutTriviallyCopyableData<T> && pensar_digital::cpplib::Hashable<T>;

        // Tag types for dispatch.
        struct HashComparableTag {};
        struct DataComparableTag {};
		struct TriviallyComparableTag {};

        // Type trait to select the appropriate tag.
        template <class T>
        using EqualDispatchTag = std::conditional_t<TriviallyHashComparable<T>, HashComparableTag, std::conditional_t<HasStdLayoutTriviallyCopyableData<T>, TriviallyComparableTag, DataComparableTag>>;

        // Implementation for StdLayoutTriviallyCopyable.
        template <StdLayoutTriviallyCopyable T>
        bool equal_impl(const T& l, const T& r, TriviallyComparableTag)
        {
            if (&l == &r) // Same object
                return true;
            return std::memcmp(l, r, sizeof(l)) == 0;
        }

        // Implementation for HasStdLayoutTriviallyCopyableData (no hash).
        template <HasStdLayoutTriviallyCopyableData T>
        bool equal_impl(const T& l, const T& r, DataComparableTag)
        {
            if (&l == &r) // Same object
                return true;
            return std::memcmp(l.data(), r.data(), l.data_size()) == 0;
        }

        // Implementation for TriviallyHashComparable (uses hash).
        template <TriviallyHashComparable T>
        bool equal_impl(const T& l, const T& r, HashComparableTag)
        {
            if (l.hash() != r.hash())
                return false;
            return equal_impl(l, r, DataComparableTag{}); // Fall back to memcmp
        }

        // Public interface: single equal function.
        template <HasStdLayoutTriviallyCopyableData T>
        bool equal(const T& l, const T& r)
        {
            return equal_impl(l, r, EqualDispatchTag<T>{});
        }
    }
}
#endif // EQUAL_HPP
