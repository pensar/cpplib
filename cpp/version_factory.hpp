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

		class VersionFactory2 : public VersionFactoryBase
		{
		public:
			VersionFactory2(const VersionInt& pub = Version::NULL_VERSION, 
													const VersionInt& pro = Version::NULL_VERSION, 
													const VersionInt& pri = Version::NULL_VERSION,
													const Id& aid = NULL_ID) : VersionFactoryBase (3, 10, pub, pro, pri, aid) { };
			virtual ~VersionFactory2 () { };
			using P = VersionFactoryBase::P;
			virtual P get (const VersionInt& pub = pd::Version::NULL_VERSION,
						   const VersionInt& pro = Version::NULL_VERSION,
						   const VersionInt& pri = Version::NULL_VERSION,
						   const Id& aid = NULL_ID)
			{
				return VersionFactoryBase::get (pub, pro, pri, aid);
			};

			P clone(const Version& aversion)
			{
				return get (aversion.get_public(), aversion.get_protected (), aversion.get_private (), aversion.get_id());
			};

			P clone (const VersionPtr& ptr) { return clone(*ptr); }

			/// <summary>
			/// To be called when parsing a json object with an embedded VersionPtr object.
			/// </summary>
			/// <param name="j"></param>
			/// <returns></returns>
			P get (const Json& j)
			{
				Id vid                = j["VERSION"]["id"        ].get<VersionInt>();
				VersionInt vpublic    = j["VERSION"]["mpublic"   ].get<VersionInt>();
				VersionInt vprotected = j["VERSION"]["mprotected"].get<VersionInt>();
				VersionInt vprivate   = j["VERSION"]["mprivate"  ].get<VersionInt>();
				
				return get (vpublic, vprotected, vprivate, vid);
			};

			P parse_json (const String& sjson)
			{
				Json j;
				P ptr = get();
				std::stringstream ss(sjson);
				ss >> *ptr;
				return ptr;
			};
		};

		inline static VersionFactory2 versionf;
	} // namespace cpplib	
} // namespace pensar_digital

#endif // VERSION_FACTORY_HPP    

