// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef ODB_HPP_INCLUDED
#define ODB_HPP_INCLUDED

#include "constant.hpp"
#include "concept.hpp"
#include "string_def.hpp"
#include "generator.hpp"

#include <memory>
#include <type_traits>
#include <vector>
#include <cassert>

namespace pensar_digital
{
    namespace cpplib
    {
        namespace db
        {
            using namespace pensar_digital::cpplib;

            template<typename T>
            concept DBObj = requires(T a) {
                typename T::Data;      // The type T::Data exists

                // Requires struct T::Data to have a field mid of type Id.
				{ T::Data::mid      } -> std::same_as<Id>;
                { T::Data::mindex   } -> std::same_as<Id>;
                { T::Data::min_use  } -> std::same_as<bool>;
                { T::Data::mchanged } -> std::same_as<bool>;

                { T::Data::Data(Id(), Id(), bool(), bool()) } noexcept -> std::same_as<typename T::Data>;  // T::Data has a constructor with the specified parameters
                { a.mdata } -> std::same_as<typename T::Data>;  // T has a member named mdata of type T::Data
            };

            template <class T, typename... Args>
            class NewFactory
            {
            public:
                virtual ~NewFactory() {}
                virtual T* get(const Args& ... args) const { return new T(args ...); }
            };

            template <class T, typename... Args>
            class MockupFactory : public NewFactory <T, Args...>
            {
            public:
                MockupFactory(T* mockup_ptr) : mmockup_ptr(mockup_ptr) { };
                virtual ~MockupFactory() {}
                virtual T* get(const Args& ... args) const { return mmockup_ptr; }

            private:
                T* mmockup_ptr;
            };

            template <class DBObj, typename... Args> //requires Initializable<T, Args...>
            class PoolFactory : public NewFactory <T, Args...>
            {
                private:
                /// <summary>
                /// Creates a pool of objects of type T containing pool_size objects created with the arguments args.
                /// </summary>
                /// <param name="pool_size">Initial pool size.</param>
                /// <param name="refill_size">When </param>
                /// <param name="args"></param>
                void fill_pool(const size_t& pool_size, const Args& ... args)
                {
                    for (size_t i = 0; i < pool_size; ++i)
                    {
                        T* ptr = new T(args ...);   
                        ptr->mdata.mindex = i;
                        ptr->mdata.min_use = false;
                        ptr->mdata.mchanged = false;
                        if (ptr->mdata.mid == NULL_ID)
							ptr->mdata.mid = generator.get_id ();

                        pool.push_back(ptr);
                    }
                    available_count = pool_size;
                }
                void add(const size_t& count, const Args& ... args)
                {
                    size_t index = pool.size ();
                    for (size_t i = 0; i < count; ++i)
                    {
                        T* ptr = new T (args ...);
                        ptr->mdata.mindex = index++;
                        ptr->mdata.min_use = false;
                        ptr->mdata.mchanged = false;
                        if (ptr->mdata.mid == NULL_ID)
                            ptr->mdata.mid = generator.get_id ();
                        pool.push_back (ptr);
                    }
                    available_count += count;
                }
            public:
                PoolFactory(const size_t initial_pool_size, const size_t a_refill_size, const Args& ... args) :
                    available_count(0),
                    refill_size(a_refill_size)
                {
                    generator = new Generator<T>();
                    fill_pool(initial_pool_size, args ...);
                };

                PoolFactory(const Args& ... args) : PoolFactory(10, 10, args ...) {};

                virtual ~PoolFactory() {}

                virtual T* get(const Args& ... args)
                {
                    // Iterate through the pool to find an available object.
                    for (size_t i = 0; i < pool.size (); ++i)
                    {
                        auto ptr = pool[i];
                        if (! ptr->mdata.min_use)
                        {
                            ptr->initialize(args ...);
                            ptr->mdata.min_use = true;
                            ptr->mdata.mchanged = false;
                            available_count--;

                            // Need to copy to increase the use count.
                            return ptr;
                        }
                    }
                    assert(available_count <= 0);
                    add(refill_size, args ...);
                    return get(args ...);
                }
                size_t get_available_count() const { return available_count; }

                size_t get_pool_size() const { return pool.size(); }

                size_t get_refill_size() const { return refill_size; }

                void set_refill_size(const size_t& value) { refill_size = value; }

                void reset(const size_t& initial_pool_size, const size_t& a_refill_size, const Args& ... args)
                {
                    pool.clear();
                    fill_pool(initial_pool_size, args ...);
                    available_count = initial_pool_size;
                    refill_size = a_refill_size;
                }
            private:
                std::vector<T*> pool;
                size_t available_count;
                size_t refill_size;
                Generator<T> generator;
            };

            template <class T, typename... Args>
            class SingletonFactory : public NewFactory <T, Args...>
            {
            private:
                using P = NewFactory<T, Args...>::P;
            public:
                //inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
                SingletonFactory(const Args& ... args) : singleton(std::make_shared<T>(args ...)) { };
                virtual ~SingletonFactory() {}
                virtual P get(const Args& ... args) const { return singleton; }
            private:
                std::shared_ptr<T> singleton;
            };

            // Factory can be configured to be a NewFactory, MockupFactory, PoolFactory or SingletonFactory.   
            template <class T, typename... Args>
            class Factory
            {
            public:
                using P = NewFactory<T, Args...>::P;
                //inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

                Factory(const size_t pool_size, const size_t refill_size, const Args& ... args)
                {
                    ptr = std::make_shared<PoolFactory<T, Args...>>(pool_size, refill_size, args ...);
                };

                virtual ~Factory() { }

                virtual P get(const Args& ... args) { return ptr->get(args ...); }
                NewFactory<T, Args...>& get_factory() const { return *ptr.get(); }

                void set_factory(const NewFactory<T, Args...>* afactory) { ptr = afactory; }
            private:
                std::shared_ptr<NewFactory<T, Args...>> ptr;
            };
        } // namespace db
    } // namespace cpplib
}

#endif // ODB_HPP_INCLUDED
