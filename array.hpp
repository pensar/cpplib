// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef ARRAY_HPP
#define ARRAY_HPP

namespace pensar_digital
{
    namespace cpplib
    {
        template <typename T>
        class Array
        {
        private:
            T* marray;
            size_t msize;    
        public:
            // Define value_type as T.
            typedef T value_type;

            Array (size_t size): msize(size) { marray = new T[size]; }
            ~Array () { delete[] marray; }

            //T& operator[] (const size_t index) const { return marray[index]; }
            //T& at (const size_t index) const { return marray[index]; }
            constexpr T& operator[] (const size_t index) const { return marray[index]; }
            constexpr T& at(const size_t index) const { return marray[index]; }

            [[nodiscard]] constexpr T* data () const noexcept { return marray; }

            [[nodiscard]] constexpr size_t size () const noexcept { return msize; }
        };
    }
}
#endif // ARRAY_HPP
