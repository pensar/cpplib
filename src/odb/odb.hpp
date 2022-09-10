#ifndef ODB_HPP_INCLUDED
#define ODB_HPP_INCLUDED

#include "string_util.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <algorithm>
#include <utility>
#include <iterator>

namespace pensar_digital
{
  namespace odb
  {
    template <class T = std::string>
    struct case_insensitive_hash : public std::unary_function<T, size_t>
    {
      size_t operator()(T val) const
      {
        std::hash<T> shash;
        return shash(cpplib::lower(val));
      }
    };

    template <class T = std::string>
    struct case_insensitive_equal_to: public std::equal_to<T>
    {
      bool operator() (const T& x, const T& y) const {return cpplib::lower(x) == cpplib::lower(y);}
    };

    template <class T = std::string>
    struct no_accents_hash : public std::unary_function<T, size_t>
    {
      size_t operator()(T val) const
      {
        std::hash<T> shash;
        return shash(cpplib::no_accents(val));
      }
    };

    template <class T = std::string>
    struct no_accents_equal_to: public std::equal_to<T>
    {
      bool operator() (const T& x, const T& y) const {return cpplib::no_accents(x) == cpplib::no_accents(y);}
    };

    template <class T = std::string>
    struct case_insensitive_no_accents_hash : public std::unary_function<T, size_t>
    {
      size_t operator()(T val) const
      {
        std::hash<T> shash;
        return shash(cpplib::lower (cpplib::no_accents(val)));
      }
    };

    template <class T = std::string>
    struct case_insensitive_no_accents_equal_to: public std::equal_to<T>
    {
      bool operator() (const T& x, const T& y) const {return (cpplib::lower (cpplib::no_accents(x)) == cpplib::lower (cpplib::no_accents(y)));}
    };

    template <class T, class Key = std::string, class Hash = case_insensitive_no_accents_hash<Key>, class Pred = case_insensitive_no_accents_equal_to<Key>, class Alloc = std::allocator<std::pair<const Key, T>>>
    class ODB
    {
      private:
        typedef std::unordered_multimap<const Key, T*, Hash, Pred, Alloc> OMap;
        OMap omap;

      public:
        ODB (): omap(OMap())  {};

        typedef std::unordered_set<T*> ResultSet;

        void add (const Key& key, T* obj)
        {
            Key k = key;
            typename Key::const_iterator i0 = key.begin ();
            while (i0 != key.end ())
            {
                typename Key::const_iterator i = i0;
                typename Key::const_iterator j = key.end ();
                while (i != j)
                {
                    std::pair<const Key, T*> p(k, obj);
                    //std::cout << p.first << " " << *p.second << std::endl;
                    omap.insert (p);
                    k.clear();
                    std::back_insert_iterator< Key > it (k);
                    std::copy (i, j--, it);
                }
                ++i0;
            }
            //std::cout << "--------------------------------" << std::endl;
        }

        bool contains (const Key& key, ResultSet& result_set, bool clear_result_set = true)
        {
            if (clear_result_set)
              result_set.clear ();
            auto range = omap.equal_range (key);
            bool found = (range.first != range.second);
            while (range.first != range.second)
            {
                T* obj = range.first->second;
                result_set.insert (obj);
                ++ range.first;
            }
            return found;
        }
    };
  }
}

#endif // ODB_HPP_INCLUDED
