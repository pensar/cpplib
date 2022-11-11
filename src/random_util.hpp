#ifndef RANDOM_UTIL_HPP_INCLUDED
#define RANDOM_UTIL_HPP_INCLUDED

#include <chrono>
#include <random>
#include <cstdlib>

namespace pensar_digital
{
  namespace cpplib
  {
    extern std::chrono::system_clock::rep seed ();
    extern std::default_random_engine get_generator ();

    template <typename T = int, class Distribution = std::uniform_int_distribution<T>, class Generator = std::default_random_engine>
    class Random
    {
      public:
        Random (const T min_value, const T max_value):
          _min(min_value),
          _max(max_value),
          generator(Generator(seed ())),
          distribution(Distribution(min_value, min_value)) {}

        T get () { return distribution(generator); }

      private:
        T _min;
        T _max;
        Generator generator;
        Distribution distribution;
    };

    class CRandom
    {
      public:
        CRandom (int min_value, int max_value): _min(min_value), _max(max_value) { srand (seed()); }

        inline int get () const { return rand () % (_max - _min + 1) + _min;}
        inline int get_min () const { return _min; }
        inline int get_max () const { return _max; }

        inline void set_min (const int min_value) { _min = min_value; }
        inline void set_max (const int max_value) { _max = max_value; }
      private:
        int _min;
        int _max;
    };

    extern Random<> random_generator;
  }
}

#endif // RANDOM_UTIL_HPP_INCLUDED
