#ifndef GENE_HPP
#define GENE_HPP

#include <random>
#include <iostream>
#include <functional>
#include "random_util.hpp"
#include "log.hpp"

namespace pensar_digital
{
  namespace genetic
  {
    template <typename T = int>
    class GeneRandomValueMutator
    {
      public:
        typedef GeneRandomValueMutator<T> M;

        GeneRandomValueMutator (const T& min_v, const T& max_v):
          min_value(min_v),
          max_value(max_v),
          r(pensar_digital::cpplib::CRandom(min_v, max_v))
        {
        };

        void mutate (T& value)
        {
            value += r.get ();
        }
      private:
        T min_value;
        T max_value;
        pensar_digital::cpplib::CRandom r;
    };

    template <class T = int>
    class GeneVoidMutator
    {
      public:
        typedef GeneVoidMutator<T> M;
        void mutate (T& value) {}
    };

    template <class Type = int,
              const Type DefaultValue = 0,
              class MutatorType = GeneVoidMutator<Type> >
    class Gene
    {
      public:
        typedef Type T;
        typedef MutatorType Mutator;
        typedef Gene<T, DefaultValue, Mutator> G;

        Gene (const T& v = DefaultValue)                  : value(v), mutator(Mutator()) {};
        Gene (const Mutator& m, const T& v = DefaultValue): value(v), mutator(m) {};
        ~Gene(){};

        void mutate ()
        {
          mutator.mutate(value);
        };

        const T& get_value () const { return value; }
        void set_value (const T& v) { value = v;}

        G& operator = (const T& v)
        {
          value = v;
          return *this;
        }

        bool operator == (const G& g) const { return value == g.value; }
        bool operator != (const G& g) const { return value != g.value; }
        bool operator <  (const G& g) const { return value  < g.value; }
        G   operator +  (const G& g) { value +=  g.value; return *this; }
        T    operator +  (const T& v) const { return value +  v; }
        G   operator -  (const G& g) const { value -= g.value; return *this; }
        T    operator -  (const T& v) const { return value -  v; }

        friend std::ostream& operator << (std::ostream& s, const G& g) { return s << g.get_value(); }

      private:
        T value;
        Mutator mutator;
    };
  }
}

#endif // GENE_HPP
