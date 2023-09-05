// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef OBJECT_FACTORY_HPP
#define OBJECT_FACTORY_HPP

#include "factory.hpp"
#include "object.hpp"
#include "json_util.hpp"  // for read_json and write_json.
#include "type_util.hpp"  // for class_name and get_id.

namespace pensar_digital
{
	namespace cpplib
	{
		class ObjectFactory : public Factory<Object, Id>
		{
			public:
				inline static const VersionPtr VERSION = pd::versionf.get (1, 1, 1);
				ObjectFactory(const Id& aid = NULL_ID) : Factory<Object, Id> (3, 10, aid) { };
				virtual ~ObjectFactory() { };
				using P = Factory<Object, Id>::P;
				virtual P  get(const Id& aid = NULL_ID)
				{
					return Factory<Object, Id>::get(aid);
				};

				P clone (const Object& aobj)
				{
					return get (aobj.get_id());
				};

				P clone (const ObjectPtr& ptr)
				{
					return clone (*ptr);
				};

				P parse_json (const Json& j)
				{
					String json_class = j.at("class");
					if (json_class != pd::class_name<Object>())
						throw std::runtime_error("Invalid class name: " + pd::class_name<Object>());
					P ptr = get (j.at("id"));

					VersionPtr v = versionf.get(j["VERSION"]);

					if (*(ptr->VERSION) != *v)
						throw std::runtime_error("ObjectFactory::parse_json: version mismatch.");

					return ptr;
				}

				P parse_json(const String& sjson)
				{
					Json j;
					P ptr = get (pd::get_id<Object>(sjson, &j));

					VersionPtr v = versionf.get (j);

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

