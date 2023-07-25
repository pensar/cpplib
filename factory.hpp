#ifndef FACTORY_HPP_INCLUDED
#define FACTORY_HPP_INCLUDED

#include "version.hpp"
#include "cpplib_concepts.hpp"
#include <memory>

namespace pensar_digital
{
    namespace cpplib
    {
        template <class T>
        class AbstractFactory
        {
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                virtual ~AbstractFactory(){}
                virtual std::shared_ptr<T> get () = 0;
        };

        template <class T>
        class NewFactory: public AbstractFactory <T>
        {
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                virtual ~NewFactory (){}
            std::shared_ptr<T> get(){ return std::make_shared<T> (); }
        };

        template <class T>
        class MockupFactory: public AbstractFactory <T>
        {
        public:
            inline static const structVersion VERSION = structVersion(1, 1, 1);
            MockupFactory(T* amockup_pointer) : mockup_pointer(amockup_pointer) { };
            virtual ~MockupFactory() {}
            std::shared_ptr<T>  get() { std::shared_ptr<T> ptr(mockup_pointer); return ptr; }

        private:
            T* mockup_pointer;
         };

        template <class T>
        class PoolFactory : public AbstractFactory <T>
        {
            private:
                /// <summary>
                /// Creates a pool of objects of type T containing pool_size objects created with the arguments args.
                /// </summary>
                /// <param name="pool_size">Initial pool size.</param>
                /// <param name="refill_size">When </param>
                /// <param name="args"></param>
                void fill_pool(const size_t& pool_size)
                {
                    for (size_t i = 0; i <= pool_size; ++i)
                    {
                        pool.push_back(std::make_shared<T>());
                    }
                    available_count = pool_size;
                }
        public:
            inline static const structVersion VERSION = structVersion(1, 1, 1);
            PoolFactory (const size_t initial_pool_size, const size_t a_refill_size) : 
                         available_count (initial_pool_size), 
                         refill_size(a_refill_size)
            {
                fill_pool(initial_pool_size);
            };
            virtual ~PoolFactory() {}

            std::shared_ptr<T>  get() 
            { 
                if (available_count <= 0)  
                    fill_pool(refill_size);
                return std::shared_ptr<T> (pool[available_count--]); 
            }
            size_t get_available_count() const { return available_count; }
        private:
            std::vector<typename std::shared_ptr<T>> pool;
            size_t available_count;
            size_t refill_size;
        };

        template <class T>
        class SingletonFactory : public AbstractFactory <T>
		{
            public:
                inline static const structVersion VERSION = structVersion(1, 1, 1);
                SingletonFactory () : singleton(std::make_shared<T>()) { };
				virtual ~SingletonFactory () {}
				std::shared_ptr<T>  get () { return singleton; }
            private:
				std::shared_ptr<T> singleton;
        };
    }
}

#endif // FACTORY_HPP_INCLUDED
