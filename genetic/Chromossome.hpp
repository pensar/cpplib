#ifndef CHROMOSSOME_HPP
#define CHROMOSSOME_HPP

#include <vector>
#include <utility>
#include <functional>
#include <iterator>
#include "Gene.hpp"

namespace pensar_digital
{
  namespace genetic
  {
    template <class GeneType = Gene<>,
              class Container = std::vector<GeneType> >
    class ChromossomeVoidMutator
    {
      public:
        void mutate (Container& c) {}
    };

    template <class GeneType,
              class Container = std::vector<GeneType> >
    class ChromossomeRandomGeneMutator
    {
      public:

        void mutate (Container& c)
        {
            if (c.size () > 0)
            {
              pensar_digital::cpplib::CRandom r(0, c.size () - 1);
              c[r.get ()].mutate();
            }
        }
    };

    template <class GeneType,
              class Container = std::vector<GeneType> >
    class ChromossomeGeneSwapMutator
    {
      public:

        void mutate (Container& c)
        {
            if (c.size () > 1)
            {
              static pensar_digital::cpplib::CRandom r(0, c.size () - 1);
              unsigned i = r.get ();
              unsigned j = r.get ();
              while (i == j)
                j = r.get ();
              std::swap (c[i], c[j]);
            }
        }

    };

    template <class GeneType = Gene<>,
              class ChromossomeMutator = ChromossomeRandomGeneMutator<GeneType>,
              class Container = std::vector<GeneType> >
    class Chromossome
    {
      public:
        typedef Container Genes;
        typedef GeneType G;
        typedef Chromossome<G, ChromossomeMutator, Container> C;

        Chromossome(const typename Container::size_type num_genes = 1, const G& g = G(), const ChromossomeMutator& m = ChromossomeMutator()): genes (Container(num_genes, g)), mutator (m){};

        /*
        Chromossome (const Chromossome& c): genes (Container()), mutator (c.mutator)
        {
          genes.clear ();
          std::copy (c.genes.begin (), c.genes.end (), std::back_inserter(genes));
        }

        Chromossome& operator = (Chromossome c)
        {
          std::swap (genes, c.genes);
          std::swap (mutator, c.mutator);
          return *this;
        }
        */

        void mutate ()
        {
          mutator.mutate(genes);
        };

        void add (const G& g) { genes.push_back(g); }

        Genes& get_genes () { return genes; }

        size_t size () const { return genes.size (); }
        G& operator [] (const unsigned i) { return genes[i]; }
        const G& operator [] (const unsigned i) const { return genes[i]; }
        bool     operator == (const C& c      ) const { return genes == c.genes; }

        bool operator != (const C& c) const { return ! ( *this == c); }

        friend std::ostream& operator << (std::ostream& s, const C& c)
        {
          s << '(';
          if (c.size () > 0)
          {
            const typename Genes::size_type _end = c.size () - 1;
            for (typename Genes::size_type i = 0; i < _end; ++i)
            {
              s << c[i] << ", ";
            }
            s << c[_end] << ')';
          }
          return s;
        }

      private:
        Genes genes;
        ChromossomeMutator mutator;
    };
  }
}

#endif // CHROMOSSOME_HPP
