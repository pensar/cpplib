// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DUMMY_FACTORY_HPP
#define DUMMY_FACTORY_HPP

#include "factory.hpp"
#include "dummy.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		class DummyFactory : public Factory<Dummy, Id, String>
		{
		public:
			inline static const IVersionPtr VERSION = pd::versionf.get (1, 1, 1);
			DummyFactory(const Id& aid = NULL_ID, const String& aname = "") : Factory<Dummy, Id, String>(3, 10, aid, aname) { };
			virtual ~DummyFactory() { };
			using P = Factory<Dummy, Id, String>::P;
			virtual P get(const Id& aid = NULL_ID, const String& aname = "")
			{
				return Factory<Dummy, Id, String>::get(aid, aname);
			};

			P clone (const IDummyRO& adummy)
			{
				return get (adummy.get_id (), adummy.get_name ());
			};
			P clone(const IDummyPtr& ptr) { return clone (*ptr);}

			P parse_json (const String& sjson)
			{
				auto j = Json::parse(sjson);
				String json_class = j.at("class");
				if (json_class != pd::class_name<Dummy>())
					throw std::runtime_error("Invalid class name: " + pd::class_name<Dummy>());
				Dummy d = j;

				return clone(d);
			};
		};

		extern DummyFactory dummyf;
	}
}

#endif // DUMMY_FACTORY_HPP    

