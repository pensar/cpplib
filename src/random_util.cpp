#include "random_util.hpp"

namespace pensar_digital
{
  namespace cpplib
  {
      std::chrono::system_clock::rep seed ()
      {
        return std::chrono::system_clock::now().time_since_epoch().count();
      }

      std::default_random_engine get_generator ()
      {
        static std::default_random_engine generator (seed ());
        return generator;
      }

      Random<> random_generator(0, 99);
  }
}
