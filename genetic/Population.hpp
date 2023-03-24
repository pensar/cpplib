#ifndef POPULATION_HPP_INCLUDED
#define POPULATION_HPP_INCLUDED

#include <random>
#include <algorithm>
#include <utility>
#include <deque>
#include "Specimen.hpp"
#include "Environment.hpp"

namespace pensar_digital
{
  namespace genetic
  {
    template <class Specimen>
    struct SpecimenPtrLessThan
    {
      bool operator () (const Specimen* left,
                        const Specimen* right) const
      { return *left < *right; }
    };

    template <class Specimen,
              class Container = std::deque<Specimen> >
    class Population
    {
      public:
        typedef Container Individuals;
        typedef Specimen S;
        typedef Population<S, Container> P;

        Population (const typename Container::size_type num_specimens = 10, const S& s = S()): individuals (Individuals(num_specimens, s)) {};

        virtual ~Population(){};

        void evolve (const typename S::FitnessValue target, const unsigned long max_iterations = 10000L, Environment& e = Environment::get_void_environment())
        {
          unsigned long generation = 0;
          while ((generation < max_iterations) && (get_best().get_value () < target))
          {
            //std::cout << generation << " best = " << get_best ().get_value () << std::endl;
            //cpplog << generation << " best = " << get_best ().get_value () << std::endl;
            //cpplog << *this << std::endl;
            procriate ();
            //cpplog << *this << std::endl;
            select (e);
            ++generation;
          }
            //std::cout << generation << " best = " << get_best ().get_value () << std::endl;
            //cpplog << generation << " best = " << get_best ().get_value () << std::endl;
            //cpplog << *this << std::endl;
            //std::cout << *this << std::endl;
        }

        void add (const S& s)
        {
          individuals.push_back (s);
        }

        const S& get_best ()
        {
          if (individuals.size () > 0)
            return individuals[individuals.size () - 1];
          else
            throw "There are no specimens in the container.";
        }
        size_t size () const { return individuals.size (); }

        S&       operator [] (const unsigned i)       { return individuals[i]; }
        const S& operator [] (const unsigned i) const { return individuals[i]; }
        bool     operator == (const P& p      ) const { return individuals == p.individuals; }

        friend std::ostream& operator << (std::ostream& o, const P& p)
        {
          o << '{' << std::endl;
          if (p.size () > 0)
          {
            const typename Individuals::size_type _end = p.size () - 1;
            for (typename Individuals::size_type i = 0; i < _end; ++i)
            {
              o << p[i] << ", " << std::endl;
            }
            o << p[_end] << '}';
          }
          return o;
        }

      protected:
        virtual void mutate ()
        {
            for (typename Individuals::iterator i = individuals.begin (); i != individuals.end (); ++i)
            {
                i->mutate ();
            }
            sort ();
        }

        virtual void procriate ()
        {
          Individuals new_generation(individuals);
          for (typename Individuals::iterator i = new_generation.begin (); i < new_generation.end (); ++i)
          {
              i->mutate();
              //std::cout << "specimen_value = " << i->get_value () << std::endl;
          }
          std::copy (new_generation.begin (), new_generation.end (), std::back_inserter(individuals));
          //cpplog << *this << std::endl;
          sort ();
        }

        void select (Environment& e)
        {
            if (individuals.size () > e.get_max_population ())
            {
              static pensar_digital::cpplib::CRandom r(0, e.get_max_population());

              // Calculates how many will survive by being best adapted.
              unsigned best_qty = e.get_best_rate () * e.get_max_population ();
              r.set_max (e.get_max_population() - best_qty);

              // Quantity to die.
              unsigned i = individuals.size () - e.get_max_population ();

              std::random_shuffle(individuals.begin (), individuals.end () - best_qty);
              while (i-- > 0)
              {
                  individuals.pop_front ();
              }
            }
        }

/*
        void select (Environment& e)
        {
            Individuals old_generation(individuals);

            // Calculates how many will survive by being best adapted.
            unsigned best_qty = e.get_best_rate () * e.get_max_population ();
            unsigned count = best_qty;

            individuals.clear ();
            while ((count > 0) && (old_generation.size () > 0))
            {
              const S& s = old_generation.back ();

              individuals.push_back (s);
              old_generation.pop_back ();
              --count;
            }

            // How many will survive by luck.
            unsigned luck_qty = e.get_max_population () - best_qty;

            // Complete the population with the lucky ones.
            std::default_random_engine generator(pensar_digital::cpplib::seed ());

            std::shuffle (old_generation.begin(), old_generation.end(), generator);
            //std::uniform_int_distribution<unsigned> distribution(0,luck_qty - 1);
            for (unsigned i = 0; i < luck_qty; ++i)
            {
              //unsigned index = distribution(generator);
              individuals.push_back(old_generation[i]) ;
            }
            sort ();
        }
*/
      private:
        inline void sort () { std::sort (individuals.begin(), individuals.end());};
        Individuals individuals;
    };
  }
}

#endif // POPULATION_HPP_INCLUDED
