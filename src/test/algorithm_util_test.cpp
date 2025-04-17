// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)#include "../algorithm_util.hpp"

#include <unordered_map>
#include <cctype>	// std::isdigit
#include <functional>	// std::bind2nd


#include "../../../unit-test/src/test.hpp"
#include "../version.hpp"
#include "../algorithm_util.hpp"

#include "../s.hpp"

namespace pensar_digital
{
	namespace test = pensar_digital::unit_test;
	using namespace pensar_digital::unit_test;
	namespace cpplib
	{
		TEST(algorithm_util, true)
			std::unordered_map<int, std::string> map;
			map[0] = "a";
			map[1] = "1";
			map[2] = "c";
			
			//cpplib::erase_if<Map::key_type, Map::mapped_type, Map::key_compare, Map::allocator_type, std::map, IsDigit> (map, IsDigit());
			erase_if(map, std::isdigit);

			test::CHECK_EQ(std::string, map[0], "a", "0");
			CHECK_EQ(W("c"), map[2]);
			CHECK_EQ(2u, map.size());
			std::vector<int> v(3);
			v[0] = 0;
			v[1] = 1;
			v[2] = 2;

			pd::erase_if(v, std::bind2nd(std::equal_to<int>(), 1));

			CHECK_EQ(0, v[0]);
			CHECK_EQ(2, v[1]);
			CHECK_EQ(2u, v.size());
			END_TEST(algorithm_util);
	}
}
