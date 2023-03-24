#ifndef EVOLUTION_HPP
#define EVOLUTION_HPP

namespace pensar_digital
{
  namespace genetic
  {
    class VoidEnvironment {};

    template <class Population, class StopCriteria, class Environment = VoidEnvironment> class Evolution
    {
      public:
        Evolution(Population* p, const StopCriteria& sc, Environment* e = VoidEnvironment())
        : population(p), stop(sc), environment(e){};

        virtual ~Evolution();

              Environment*  get_environment() { return environment; }
              Population*   get_population () { return population;  }
        const StopCriteria& get_stop_criteria () { return stop; }

        void run ()
        {
          while (not stop(population))
          {
            population.evolve (environment);
          }
        }

      protected:
      private:
        Environment* environment; //!< Member variable "environment"
        Population* population; //!< Member variable "population"
        StopCriteria stop;
    };
  }
}

#endif // EVOLUTION_HPP
