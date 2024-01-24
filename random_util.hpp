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
    
    template <typename T = std::chrono::system_clock::rep, class Distribution = std::uniform_int_distribution<T>, class RandomGenerator = std::default_random_engine>
    class Random 
    {
        public:

        Random(const T min_value, const T max_value) : 
            mmin(min_value), mmax(max_value), mseed(seed()), mdistribution(min_value, max_value), mgenerator(mseed) {}

        T operator()() {
            return mdistribution(mgenerator);
        }

    private:
        Distribution mdistribution;
        RandomGenerator mgenerator;
        std::chrono::system_clock::rep mseed;
        T mmin;
        T mmax;
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
