// $Id$

#include <cmath>
#include <type_traits>
#include <boost/test/unit_test.hpp>
#include "../genetic/Population.hpp"

namespace gen = pensar_digital::genetic;

template <class ChromossomeType,
          class FitnessValueType = int,
          class Container = std::vector<ChromossomeType> >
class SpecimenEvaluator
{
  public:
    typedef gen::Specimen<SpecimenEvaluator, ChromossomeType, FitnessValueType, Container> S;
    typename S::FitnessValue evaluate (const S& s, const gen::Environment& e = gen::Environment::get_void_environment()) const
    {
      typename S::FitnessValue value = 0;
      for (typename Container::size_type i = 0; i < s.size (); ++i)
      {
        for (typename Container::size_type j = 0; j < s[i].size(); ++j)
          value += s[i][j].get_value ();
      }
      return value;
    };
};


BOOST_AUTO_TEST_SUITE(population_suite)

BOOST_AUTO_TEST_CASE(sort_test)
{
    typedef gen::Gene<int, 0, gen::GeneVoidMutator<>> G;
    typedef gen::Chromossome<G, gen::ChromossomeVoidMutator<G>> C;
    typedef SpecimenEvaluator<C> Evaluator;
    typedef gen::Specimen<Evaluator, C, int> S;
    typedef gen::Population<S> P;

    //BOOST_CHECK (std::is_copy_constructible<P>::value);
    //BOOST_CHECK (std::is_move_constructible<P>::value);
    //BOOST_CHECK (std::is_move_assignable<P>::value);

    P p (3);
    S& s = p[1];
    s[0][0] = 2;
    s.evaluate ();

    std::vector<S> v(0);
    v.push_back (p[0]);
    v.push_back (p[1]);
    v.push_back (p[2]);
    std::sort (v.begin (), v.end ());
    BOOST_CHECK_EQUAL (p[1], v[2]);
    BOOST_CHECK_EQUAL (0, v[0].get_value ());
    BOOST_CHECK_EQUAL (0, v[1].get_value ());
    BOOST_CHECK_EQUAL (2, v[2].get_value ());
}

BOOST_AUTO_TEST_CASE(population_test)
{
    typedef gen::GeneRandomValueMutator<int> GeneMutator;
    typedef gen::Gene<int, 0, GeneMutator> G;
    typedef gen::ChromossomeRandomGeneMutator<G> ChromossomeMutator;
    typedef gen::Chromossome<G, ChromossomeMutator> C;
    typedef SpecimenEvaluator<C> Evaluator;
    typedef gen::Specimen<Evaluator, C, int> S;
    typedef gen::Population<S> P;
    GeneMutator gene_mutator(1, 2);

    G gene(gene_mutator);
    C c (10, gene);
    S specimen (1, c);
    P population (10, specimen);

    S::FitnessValue value = population.get_best().get_value ();
    BOOST_CHECK_EQUAL (0, value);

    population.evolve (20, 100);

    S::FitnessValue value2 = population.get_best().get_value ();
    BOOST_CHECK (value2 > 0);
}

BOOST_AUTO_TEST_SUITE_END ()
