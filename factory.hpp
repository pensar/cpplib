#ifndef FACTORY_HPP_INCLUDED
#define FACTORY_HPP_INCLUDED

#include "version.hpp"
#include "cpplib_concepts.hpp"
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
                virtual std::shared_ptr<T> get(Args&& ... args){ return std::make_shared<T> (std::forward<Args>(args) ...); }
        };

        template <class T, typename... Args>
        class MockupFactory: public NewFactory <T, Args...>
        {
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                MockupFactory(T* amockup_pointer) : mockup_pointer(amockup_pointer) { };
                virtual ~MockupFactory() {}
                virtual std::shared_ptr<T>  get(Args&& ... args) { std::shared_ptr<T> ptr(mockup_pointer); return ptr; }

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
                void fill_pool(const size_t& pool_size, Args&& ... args)
                {
                    for (size_t i = 0; i <= pool_size; ++i)
                    {
                        pool.push_back(std::make_shared<T>(std::forward<Args>(args) ...));
                    }
                    available_count = pool_size;
                }
        public:
            inline static const structVersion VERSION = structVersion(1, 1, 1);
            PoolFactory (const size_t initial_pool_size, const size_t a_refill_size, Args&& ... args) :
                         available_count (initial_pool_size), 
                         refill_size(a_refill_size)
            {
                fill_pool(initial_pool_size, std::forward<Args>(args) ...);
            };
            virtual ~PoolFactory() {}

            virtual std::shared_ptr<T>  get(Args&& ... args)
            { 
                if (available_count <= 0)  
                    fill_pool(refill_size, std::forward<Args>(args) ...);
                std::shared_ptr<T> ptr = pool[available_count--];
                ptr->initialize (std::forward<Args>(args) ...);
                return ptr; 
            }
            size_t get_available_count() const { return available_count; }
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
                SingletonFactory (Args&& ... args) : singleton(std::make_shared<T>(std::forward<Args>(args) ...)) { };
				virtual ~SingletonFactory () {}
                virtual std::shared_ptr<T>  get (Args&& ... args) { return singleton; }
            private:
				std::shared_ptr<T> singleton;
        };
    }
}

#endif // FACTORY_HPP_INCLUDED
