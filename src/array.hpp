// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "concept.hpp"
#include "error.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        template <typename T>
        class DynamicArray
        {
        private:
            T* marray;
            size_t msize;    
        public:
            // Define value_type as T.
            typedef T value_type;

            DynamicArray(size_t size): msize(size) { marray = new T[size]; }
            ~DynamicArray() { delete[] marray; }

            //T& operator[] (const size_t index) const { return marray[index]; }
            //T& at (const size_t index) const { return marray[index]; }
            constexpr T& operator[] (const size_t index) const { return marray[index]; }
            constexpr T& at(const size_t index) const { return marray[index]; }

            [[nodiscard]] constexpr T* data () const noexcept { return marray; }

            [[nodiscard]] constexpr size_t size () const noexcept { return msize; }
        };

		// A StdLayoutTriviallyCopyable version of Array.
		template <size_t Size = 1, typename T = size_t>
			requires StdLayoutTriviallyCopyable<T>
        class CArray
        {
            public:
                T _array[Size];
                size_t _data_size;

                // Define value_type as T.
                using value_type = T;
                const size_t SIZE = Size;

                template <bool safe = true>
                constexpr T& operator[] (const size_t index) const
                {
                    if (index >= Size)
                    {
                        S msg = W("Array::operator[]: size is bigger than max (") + Size + W(")");
                        log_and_throw(msg);
                    }
                    return _array[index];
                }

                // specialization for safe = false
                template <>
                constexpr T& operator[]<false> (const size_t index) const
                {
                    return _array[index];
                }

                constexpr T* data() const noexcept { return _array; }

                constexpr size_t size() const noexcept { return Size; }
        };
		static_assert(StdLayoutTriviallyCopyable<pensar_digital::cpplib::CArray<10>>);
    }
}
#endif // ARRAY_HPP
