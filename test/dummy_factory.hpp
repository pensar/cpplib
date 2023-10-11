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
		class DummyFactory2 : public Factory<Dummy, Id, String>
		{
		public:
			inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);
			DummyFactory2(const Id& aid = NULL_ID, const String& aname = "") : Factory<Dummy, Id, String>(3, 10, aid, aname) { };
			virtual ~DummyFactory2() { };
			using P = Factory<Dummy, Id, String>::P;
			virtual P get(const Id& aid = NULL_ID, const String& aname = "")
			{
				return Factory<Dummy, Id, String>::get(aid, aname);
			};

			P clone (const Dummy& adummy)
			{
				return get (adummy.id (), adummy.get_name ());
			};
			P clone(const DummyPtr& ptr) { return clone (*ptr);}

			P parse_json (const String& sjson)
			{
				Json j;
				P ptr = get(pd::id<Dummy>(sjson, &j));
				ptr->set_name (j.at("name"));
				VersionPtr v = Version::get(j);

				// todo: check version compatibility.
				if (*(ptr->VERSION) != *v)
					throw std::runtime_error("DummyFactory::parse_json: version mismatch.");

				return ptr;
			};
		};

		extern DummyFactory2 dummyf;
	}
}

#endif // DUMMY_FACTORY_HPP    

