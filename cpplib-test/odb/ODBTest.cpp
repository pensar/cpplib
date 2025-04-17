// $Id

#include <string>

#include <boost/test/unit_test.hpp>

#include "filesystem_util.h"
#include "io_util.h"
#include "my_boost.hpp"
#include "ODB.hpp"
#include "string_util.hpp"

using namespace boost::unit_test_framework;
namespace cpp = pensar_digital::cpplib;
namespace odb = pensar_digital::odb;

class OdbDummy
{
  public:
    OdbDummy (int aid, const std::string& aname, const std::string& aother): id(aid), name(aname), other(aother) {};
    std::string search_string () { return cpp::to_string(id) + "," + name + "," + other; }
  private:
    int id;
    std::string name;
    std::string other;
};

BOOST_AUTO_TEST_SUITE(odb_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  namespace fs = boost::filesystem;

  fs::path dbfile("test_db");
  odb::ODB<std::string> db;
}

BOOST_AUTO_TEST_CASE(add_test)
{
  std::string s1 = "asdfçlkj";
  std::string s2 = "asdkkk";
  std::string s3 = "kj123";

  odb::ODB<std::string> db;
  db.add (s1, &s1);
  db.add (s2, &s2);
  db.add (s3, &s3);

  odb::ODB<std::string>::ResultSet r;
  BOOST_CHECK (db.contains ("kj", r));
  BOOST_CHECK_EQUAL(unsigned(2), r.size());

  BOOST_CHECK (db.contains ("k", r));
  BOOST_CHECK_EQUAL(unsigned(3), r.size());

  BOOST_CHECK (db.contains ("ç", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
}

BOOST_AUTO_TEST_CASE(contains_test)
{
  odb::ODB<OdbDummy> db;
  OdbDummy d1 (1, "d1", "other1");
  db.add (d1.search_string(), &d1);

  odb::ODB<OdbDummy>::ResultSet r;
  BOOST_CHECK(db.contains ("d1", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());

  OdbDummy d2 (2, "a rather long name for this object", "");
  db.add (d2.search_string(), &d2);
  db.add (d2.search_string(), &d2); // it should not include d2 twice.
  BOOST_CHECK(db.contains ("long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(! db.contains ("longa", r));
  BOOST_CHECK_EQUAL(unsigned(0), r.size());
  BOOST_CHECK(db.contains ("Lõng", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());

  OdbDummy d3 (3, "a rather long name for this objec", "");
  OdbDummy d4 (4, "a rather long name for this obje", "");
  OdbDummy d5 (5, "a rather long name for this obj", "");
  db.add (d3.search_string(), &d3);
  db.add (d4.search_string(), &d4);
  db.add (d5.search_string(), &d5);
  BOOST_CHECK(db.contains ("a rather long name for this object", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(db.contains ("a rather long name for this objec", r));
  BOOST_CHECK_EQUAL(unsigned(2), r.size());
  BOOST_CHECK(db.contains ("a rather long name for this obje", r));
  BOOST_CHECK_EQUAL(unsigned(3), r.size());
  BOOST_CHECK(db.contains ("a rather long name for this obj", r));
  BOOST_CHECK_EQUAL(unsigned(4), r.size());
}

BOOST_AUTO_TEST_CASE(case_insensitive_test)
{
  typedef odb::ODB<OdbDummy, std::string, odb::case_insensitive_hash<>, odb::case_insensitive_equal_to<>> DB;
  DB db;
  OdbDummy d2 (2, "a rather long name for this object", "");
  db.add (d2.search_string(), &d2);
  typename DB::ResultSet r;
  BOOST_CHECK(db.contains ("long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(db.contains ("Long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
}

BOOST_AUTO_TEST_CASE(no_accents_test)
{
  typedef odb::ODB<OdbDummy, std::string, odb::no_accents_hash<>, odb::no_accents_equal_to<>> DB;
  DB db;
  OdbDummy d2 (2, "a rather long name for this object", "");
  db.add (d2.search_string(), &d2);
  typename DB::ResultSet r;
  BOOST_CHECK(db.contains ("long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(db.contains ("lóng", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
}

BOOST_AUTO_TEST_CASE(case_insensitive_no_accents_test)
{
  typedef odb::ODB<OdbDummy, std::string, odb::case_insensitive_no_accents_hash<std::string>, odb::case_insensitive_no_accents_equal_to<std::string>> DB;
  DB db;
  OdbDummy d2 (2, "a rather long name for this object", "");
  db.add (d2.search_string(), &d2);
  typename DB::ResultSet r;
  BOOST_CHECK(db.contains ("long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(db.contains ("Lóng", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
}

BOOST_AUTO_TEST_CASE(default_hash_test) // It should be equivalent to the case insensitive no accents test above.
{
  typedef odb::ODB<OdbDummy> DB;
  DB db;
  OdbDummy d2 (2, "a rather long name for this object", "");
  db.add (d2.search_string(), &d2);
  typename DB::ResultSet r;
  BOOST_CHECK(db.contains ("long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(db.contains ("Lóng", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
}

BOOST_AUTO_TEST_CASE(case_sensitive_accents_test)
{
  typedef odb::ODB<OdbDummy, std::string, std::hash<std::string>, std::equal_to<std::string>> DB;
  DB db;
  OdbDummy d2 (2, "a rather long name for this object", "");
  db.add (d2.search_string(), &d2);
  typename DB::ResultSet r;
  BOOST_CHECK(db.contains ("long", r));
  BOOST_CHECK_EQUAL(unsigned(1), r.size());
  BOOST_CHECK(! db.contains ("Long", r));
  BOOST_CHECK_EQUAL(unsigned(0), r.size());
  BOOST_CHECK(! db.contains ("lóng", r));
  BOOST_CHECK_EQUAL(unsigned(0), r.size());
}

BOOST_AUTO_TEST_SUITE_END ()
