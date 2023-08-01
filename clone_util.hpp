#include "factory.hpp"
#include <memory> // for std::shared_ptr

namespace pensar_digital
{
	namespace cpplib
	{
        template<class T, typename... Args> //requires FactoryCloneable<T, Args...>
        static std::shared_ptr<T> clone(const T& other, const Args& ... args)
        {
            NewFactory<T, Args...> factory;
            return factory.get(args...);
        }
	} // namespace cpplib
}
