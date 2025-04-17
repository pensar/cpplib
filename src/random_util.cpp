
#include "random_util.hpp"

namespace pensar_digital
{
  namespace cpplib
  {
      DefaultRandomGeneratorType seed ()
      {
        return std::chrono::system_clock::now().time_since_epoch().count();
      }

      RandGen get_generator ()
      { 
        static RandGen random_generator (seed ())
        ;

        return random_generator;
      }

  }
}
