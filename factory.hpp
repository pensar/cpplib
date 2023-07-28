#ifndef FACTORY_HPP_INCLUDED
#define FACTORY_HPP_INCLUDED

#include "version.hpp"
#include "concept.hpp"
#include <memory>
#include <type_traits>

namespace pensar_digital
{
    namespace cpplib
    {
        template <class T, typename... Args>
        class NewFactory
        {
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                virtual ~NewFactory (){}
                virtual std::shared_ptr<T> get(const Args& ... args) const { return std::make_shared<T> (args ...); }
        };

        template <class T, typename... Args>
        class MockupFactory: public NewFactory <T, Args...>
        {
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                MockupFactory(T* amockup_pointer) : mockup_pointer(amockup_pointer) { };
                virtual ~MockupFactory() {}
                virtual std::shared_ptr<T>  get(const Args& ... args) const { std::shared_ptr<T> ptr(mockup_pointer); return ptr; }

            private:
                T* mockup_pointer;
         };

        template <class T, typename... Args> //requires Initializable<T, Args...>
        class PoolFactory : public NewFactory <T, Args...>
        {
            private:
                /// <summary>
                /// Creates a pool of objects of type T containing pool_size objects created with the arguments args.
                /// </summary>
                /// <param name="pool_size">Initial pool size.</param>
                /// <param name="refill_size">When </param>
                /// <param name="args"></param>
                void fill_pool (const size_t& pool_size, const Args& ... args)
                {
                    for (size_t i = 0; i < pool_size; ++i)
                    {
                        std::shared_ptr<T>&& ptr = std::make_shared<T>(args ...);
                        pool.push_back(std::move(ptr));
                    }
                    available_count = pool_size;
                }
                void add (const size_t& count, const Args& ... args)
                {
					for (size_t i = 0; i < count; ++i)
					{
                        std::shared_ptr<T>&& ptr = std::make_shared<T>(args ...);
                        pool.push_back(ptr);
                    }
					available_count += count;
				}
        public:
            inline static const structVersion VERSION = structVersion(1, 1, 1);
            PoolFactory (const size_t initial_pool_size, const size_t a_refill_size, const Args& ... args) :
                         available_count (0), 
                         refill_size(a_refill_size)
            {
                fill_pool(initial_pool_size, args ...);
            };
            
            PoolFactory(const Args& ... args) : PoolFactory (10, 10, args ...) { };
            
            virtual ~PoolFactory() {}

            virtual std::shared_ptr<T>  get(const Args& ... args) 
            { 
                // Iterate through the pool to find an available object.
                for (auto& ptr : pool)
                {
					if (ptr.use_count () < 2)
					{
                        ptr->initialize (args ...);
                        available_count--;

                        // Need to copy to increase the use count.
						return * new (std::shared_ptr<T>) (ptr); 
					}
				}
                assert(available_count <= 0);
    			add (refill_size, args ...);
                return get (args ...);
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
            std::vector<typename std::shared_ptr<T>> pool;
            size_t available_count;
            size_t refill_size;
        };

        template <class T, typename... Args>
        class SingletonFactory : public NewFactory <T, Args...>
		{
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                SingletonFactory (const Args& ... args) : singleton (std::make_shared<T>(args ...)) { };
				virtual ~SingletonFactory () {}
                virtual std::shared_ptr<T>  get (const Args& ... args) const { return singleton; }
            private:
				std::shared_ptr<T> singleton;
        };
    }
}

#endif // FACTORY_HPP_INCLUDED
