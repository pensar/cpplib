// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_FACTORY_HPP
#define OBJECT_FACTORY_HPP

#include "factory.hpp"
#include "object.hpp"
#include "json_util.hpp"  // for read_json and write_json.

namespace pensar_digital
{
	namespace cpplib
	{
		class ObjectFactory : public Factory<Object, Id>
		{
			public:
				inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
				ObjectFactory(const Id& aid = NULL_ID) : Factory<Object, Id> (3, 10, aid) { };
				virtual ~ObjectFactory() { };
				using P = Factory<Object, Id>::P;
				virtual P  get(const Id& aid = NULL_ID)
				{
					return Factory<Object, Id>::get(aid);
				};

				P clone (const IObjectRO& aobj)
				{
					return get (aobj.get_id());
				};

				P clone (const IObjectPtr& ptr)
				{
					return clone (*ptr);
				};

				P parse_json(const String& sjson)
				{
					Json j;
					P ptr = get (pd::get_id<Object>(sjson, &j));

					IVersionPtr v = versionf.get (j);

					// todo: check version compatibility.
					if (*(ptr->VERSION) != *v)
						throw std::runtime_error("ObjectFactory::parse_json: version mismatch.");
					return ptr;
				}

		};

		extern ObjectFactory objectf;
	}	
}

#endif // OBJECT_FACTORY_HPP    

