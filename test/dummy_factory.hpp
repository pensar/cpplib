// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef DUMMY_FACTORy_HPP
#define DUMMY_FACTORy_HPP

#include "factory.hpp"
#include "dummy.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		class DummyFactory : public Factory<Dummy, Id, String>
		{
		public:
			inline static const structVersion VERSION = structVersion(1, 1, 1);
			DummyFactory(const Id& aid = NULL_ID, const String& aname = "") : Factory<Dummy, Id, String>(3, 10, aid, aname) { };
			virtual ~DummyFactory() { };
			virtual IDummyPtr get(const Id& aid = NULL_ID, const String& aname = "")
			{
				return Factory<Dummy, Id, String>::get(aid, aname);
			};

			IDummyPtr clone (const IDummy_RO& adummy)
			{
				return get (adummy.get_id (), adummy.get_name ());
			};
			IDummyPtr clone(const IDummyPtr& ptr) { return clone (*ptr);}

			IDummyPtr parse_json (const String& sjson)
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

#endif // DUMMY_FACTORy_HPP    

