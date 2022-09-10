// $Id$

#include <cmath>
#include <boost/test/unit_test.hpp>
#include "../genetic/Specimen.hpp"

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


BOOST_AUTO_TEST_SUITE(specimen_suite)

BOOST_AUTO_TEST_CASE(specimen_value_test)
{
    typedef gen::Gene<int, 0> G;
    typedef gen::Chromossome<G, gen::ChromossomeVoidMutator<G>> C;
    typedef SpecimenEvaluator<C> Evaluator;
    typedef gen::Specimen<Evaluator, C, int> S;

    C c(0);
    c.add (G(0));
    c.add (G(1));
    c.add (G(2));
    c.add (G(3));
    c.add (G(4));
    c.add (G(5));
    c.add (G(6));
    c.add (G(7));
    c.add (G(8));
    c.add (G(9));
    S s (1, c);

    std::stringstream ss;
    ss << s;
    std::string result = ss.str();
    BOOST_CHECK_EQUAL (std::string("[(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)] = 45"), result);

    C c2(c);
    S s2 (1, c2);
    s2[0][0] = 1;
    s2.evaluate ();
    BOOST_CHECK_EQUAL (46, s2.get_value ());
    BOOST_CHECK (s < s2);
}

BOOST_AUTO_TEST_CASE(specimen_test)
{
    typedef gen::GeneRandomValueMutator<int> GeneMutator;
    typedef gen::Gene<int, 0, GeneMutator> G;
    typedef gen::ChromossomeRandomGeneMutator<G> ChromossomeMutator;
    typedef gen::Chromossome<G, ChromossomeMutator> C;
    typedef SpecimenEvaluator<C> Evaluator;
    typedef gen::Specimen<Evaluator, C, int> S;

    GeneMutator gene_mutator(1, 2);


    ChromossomeMutator cmutator;
    G gene(gene_mutator);
    C c (10, gene, cmutator);
    S specimen (10, c);

    S::FitnessValue value = specimen.get_value ();
    BOOST_CHECK_EQUAL (0, value);

    specimen.mutate ();
    S::FitnessValue value2 = specimen.get_value ();
    BOOST_CHECK_NE (0, value2);

    S s2 (10, c);
    BOOST_CHECK (s2 < specimen);

    S s(specimen);
    S::FitnessValue value3 = s.get_value ();
    BOOST_CHECK_NE (0, value3);
}

BOOST_AUTO_TEST_SUITE_END ()
