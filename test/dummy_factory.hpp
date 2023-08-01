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
			virtual DummyPtr  get(const Id& aid = NULL_ID, const String& aname = "")
			{
				return Factory<Dummy, Id, String>::get(aid, aname);
			};
		};

		extern DummyFactory dummyf;
	}
}

#endif // DUMMY_FACTORy_HPP    

