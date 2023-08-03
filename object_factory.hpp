// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_FACTORY_HPP
#define OBJECT_FACTORY_HPP

#include "factory.hpp"
#include "object.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		class ObjectFactory : public Factory<Object, Id>
		{
			public:
				inline static const structVersion VERSION = structVersion(1, 1, 1);
				ObjectFactory(const Id& aid = NULL_ID) : Factory<Object, Id> (3, 10, aid) { };
				virtual ~ObjectFactory() { };
				virtual ObjectPtr  get(const Id& aid = NULL_ID)
				{
					return Factory<Object, Id>::get(aid);
				};
		};

		extern ObjectFactory objectf;
	}	
}

#endif // OBJECT_FACTORY_HPP    

