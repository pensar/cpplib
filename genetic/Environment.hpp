#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP


namespace pensar_digital
{
  namespace genetic
  {
    class Environment
    {
      public:
        // todo: Raise an exception if bestrate >= 1.
        Environment(double bestrate = 0.5, unsigned maxpop = 10): best_rate(bestrate), max_population(maxpop) {};
        virtual ~Environment();

        static Environment& get_void_environment ()
        {
          static Environment ve;
          return ve;
        }

        double get_best_rate () const { return best_rate; }
        unsigned get_max_population () const { return max_population; }

      private:
        double best_rate; // percentage of best fits to survive.
        unsigned max_population; // maximum number of individuals allowed in this environment.
    };

  }
}

#endif // ENVIRONMENT_HPP
