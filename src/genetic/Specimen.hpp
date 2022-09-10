#ifndef SPECIMEN_HPP
#define SPECIMEN_HPP

#include "Chromossome.hpp"
#include "Environment.hpp"

namespace pensar_digital
{
  namespace genetic
  {
    template <class SpecimenEvaluatorType,
              class ChromossomeType,
              class FitnessValueType = int,
              class Container = std::vector<ChromossomeType> >
    class Specimen
    {
      public:
        typedef SpecimenEvaluatorType Evaluator;
        typedef FitnessValueType FitnessValue;
        typedef ChromossomeType C;
        typedef Container Chromossomes;
        typedef Specimen<Evaluator, C, FitnessValue, Container> S;

        Specimen (const typename Container::size_type num_chromossomes = 1, const C& c = C()): chromossomes(Container(num_chromossomes, c)), evaluator (Evaluator())
        {
          value = evaluator.evaluate (*this);
        };

        virtual ~Specimen () {};

        void evaluate ()
        {
          value = evaluator.evaluate (*this);
        }

        virtual void mutate (const Environment& e = Environment::get_void_environment())
        {
            for (typename Chromossomes::iterator i = chromossomes.begin (); i != chromossomes.end (); ++i)
            {
                i->mutate ();
            }
            evaluate ();
        }

        Container& get_chromossomes () { return chromossomes; }

        const FitnessValue& get_value () const
        {
          return value;
        };

        bool less_than (const S& s) const { return value < s.value; };

        bool operator < (const S& s) const { return less_than(s); }
        void add (const C& c) { chromossomes.push_back(c); }

        size_t size () const { return chromossomes.size (); }
        // todo: implement a change subscription scheme. For now remember to call evaluate() if the reference is used to change the value.
        C&       operator [] (const unsigned i)       { return chromossomes[i]; }
        const C& operator [] (const unsigned i) const { return chromossomes[i]; }
        bool     operator == (const S& s      ) const { return chromossomes == s.chromossomes; }

        friend std::ostream& operator << (std::ostream& o, const S& s)
        {
          o << '[';
          if (s.size () > 0)
          {
            const typename Chromossomes::size_type _end = s.size () - 1;
            for (typename Chromossomes::size_type i = 0; i < _end; ++i)
            {
              o << s[i] << ", ";
            }
            o << s[_end] << "] = " << s.get_value();
          }
          return o;
        }

      private:
        Container chromossomes;
        Evaluator evaluator;
        FitnessValue value;
    };
  }
}

#endif // SPECIMEN_HPP
