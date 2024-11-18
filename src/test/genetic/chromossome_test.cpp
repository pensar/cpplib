// $Id$

//#include <cmath>
#include <boost/test/unit_test.hpp>
#include "../genetic/Chromossome.hpp"

namespace gen = pensar_digital::genetic;

BOOST_AUTO_TEST_SUITE(chromossome_suite)

BOOST_AUTO_TEST_CASE(chromossome_test)
{
    typedef gen::GeneRandomValueMutator<int> GeneMutator;
    GeneMutator gene_mutator(2, 3);

    typedef gen::Gene<int, 0, GeneMutator> G;
    //typedef gen::ChromossomeRandomGeneMutator<G> ChromossomeMutator;
    typedef gen::Chromossome<G> C;

    G gene(gene_mutator);
    C c(10, gene);

    int sum = 0;
    for (unsigned i = 0; i < c.size (); ++i)
    {
      sum += c[i].get_value();
    }
    BOOST_CHECK_EQUAL (0, sum);
    C c2 = c;

    BOOST_CHECK (c == c2);

    c.mutate();
    for (unsigned i = 0; i < c.size (); ++i)
    {
      sum += c[i].get_value();
    }
    BOOST_CHECK (sum > 1);
    BOOST_CHECK (c != c2);
}

BOOST_AUTO_TEST_CASE(chromossome_swap_gene_test)
{
    typedef gen::Gene<int, 0> G;
    typedef gen::ChromossomeGeneSwapMutator<G> ChromossomeMutator;
    typedef gen::Chromossome<G, ChromossomeMutator> C;

    C c(0);
    for (int i = 0; i < 2; ++i)
    {
      G gene;
      gene = i;
      c.add (gene);
    }

    BOOST_CHECK_EQUAL (0, c[0].get_value());
    BOOST_CHECK_EQUAL (1, c[1].get_value());

    c.mutate ();
    BOOST_CHECK_EQUAL (1, c[0].get_value());
    BOOST_CHECK_EQUAL (0, c[1].get_value());
}

BOOST_AUTO_TEST_SUITE_END ()
