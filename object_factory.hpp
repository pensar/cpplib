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
				virtual IObjectPtr  get(const Id& aid = NULL_ID)
				{
					return Factory<Object, Id>::get(aid);
				};

				IObjectPtr clone (const IObject_RO& aobj)
				{
					return get (aobj.get_id());
				};

				IObjectPtr clone (const IObjectPtr& ptr)
				{
					return clone (*ptr);
				};

				IObjectPtr parse_json(const String& sjson)
				{
					auto j = Json::parse(sjson);
					String json_class = j.at("class");
					if (json_class != pd::class_name<Object> ())
						throw std::runtime_error("Invalid class name: " + pd::class_name<Object> ());
					Object o = j;

					return clone(o);
				}

		};

		extern ObjectFactory objectf;
	}	
}

#endif // OBJECT_FACTORY_HPP    

