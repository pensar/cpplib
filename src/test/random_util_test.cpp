// $Id$

#include <cmath>
#include <boost/test/unit_test.hpp>
#include "../random_util.hpp"
#include "../log.hpp"

namespace cpp = pensar_digital::cpplib;

BOOST_AUTO_TEST_SUITE(random_util_suite)

BOOST_AUTO_TEST_CASE(random_test)
{
  cpp::CRandom r(0, 9);

  int a[10];

  for (unsigned i = 0; i < 10; ++i)
    a[i] = 0;

  for (unsigned i = 0; i < 10000; ++i)
  {
      unsigned u = r.get ();
      a[u]++;
  }

  double variance = 0.0;

  for (unsigned i = 0; i < 10; ++i)
  {
      variance += pow(a[i] - 1000, 2);
  }

  variance /= 10;

  double standard_deviation = sqrt (variance);

  //cpplog << "standard deviation = " << standard_deviation << std::endl;

  BOOST_CHECK (standard_deviation < 50);
}

BOOST_AUTO_TEST_SUITE_END ()
