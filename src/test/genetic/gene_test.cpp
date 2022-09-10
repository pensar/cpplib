// $Id$

#include <cmath>
#include <boost/test/unit_test.hpp>

#include "../genetic/Gene.hpp"

namespace gen = pensar_digital::genetic;

BOOST_AUTO_TEST_SUITE(genetic_gene_suite)

BOOST_AUTO_TEST_CASE(gene_test)
{
    typedef gen::GeneRandomValueMutator<int> Mutator;
    Mutator mutator(1, 2);

    typedef gen::Gene<int, 0, Mutator> G;
    G gene(mutator);

    gene = 0;
    BOOST_CHECK_EQUAL (0, gene.get_value());
    G gene2 = gene;
    BOOST_CHECK (gene == gene2);

    gene.mutate ();
    BOOST_CHECK (gene.get_value() > 0);
    BOOST_CHECK (gene != gene2);
}

BOOST_AUTO_TEST_SUITE_END ()
