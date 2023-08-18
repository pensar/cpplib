// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef VERSION_FACTORY_HPP
#define VERSION_FACTORY_HPP

#include "factory.hpp"
#include "version.hpp"

namespace pensar_digital
{
	namespace cpplib
	{
		typedef Factory<Version, const VersionInt&,
								 const VersionInt&,
								 const VersionInt&,
								 const Id&> VersionFactoryBase;

		class VersionFactory : public VersionFactoryBase
		{
		public:
			VersionFactory(const VersionInt& pub = Version::NULL_VERSION, 
													const VersionInt& pro = Version::NULL_VERSION, 
													const VersionInt& pri = Version::NULL_VERSION,
													const Id& aid = NULL_ID) : VersionFactoryBase (3, 10, pub, pro, pri, aid) { };
			virtual ~VersionFactory() { };
			using P = VersionFactoryBase::P;
			virtual P get (const VersionInt& pub = Version::NULL_VERSION,
						   const VersionInt& pro = Version::NULL_VERSION,
						   const VersionInt& pri = Version::NULL_VERSION,
						   const Id& aid = NULL_ID)
			{
				return VersionFactoryBase::get (pub, pro, pri, aid);
			};

			P clone(const IVersionRO& aversion)
			{
				return get(aversion.get_id(), aversion.get_public(), aversion.get_protected (), aversion.get_private ());
			};

			P clone (const IVersionPtr& ptr) { return clone(*ptr); }

			P parse_json (const String& sjson)
			{
				auto j = Json::parse(sjson);
				String json_class = j.at("class");
				if (json_class != pd::class_name<Version>())
					throw std::runtime_error("Invalid class name: " + pd::class_name<Version>());
				Version v = j;

				return clone(v);
			};
		};

		inline static VersionFactory versionf;
	} // namespace cpplib	
} // namespace pensar_digital

#endif // VERSION_FACTORY_HPP    

