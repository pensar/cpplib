// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef PATH_FACTORY_HPP
#define PATH_FACTORY_HPP

#include "factory.hpp"
#include "io_util.hpp"
#include "version_factory.hpp"	

namespace pensar_digital
{
	namespace cpplib
	{
		typedef Factory<Path, fs::path, Id> PathFactoryBase;
		class PathFactory : public PathFactoryBase
		{
		public:
			inline static const VersionPtr VERSION = pd::versionf.get(1, 1, 1);
			PathFactory(const fs::path& p = ".", const Id& aid = NULL_ID) : PathFactoryBase(3, 10, p, aid) { };
			virtual ~PathFactory() { };
			using P = PathFactoryBase::P;
			virtual P get(const fs::path& p = ".", const Id& aid = NULL_ID)
			{
				return PathFactoryBase::get(p, aid);
			};

			P clone(const PathRO& apath)
			{
				return get(apath.to_std_path (), apath.get_id());
			};
			P clone(const PathPtr& ptr) { return clone(*ptr); }

			P parse_json(const String& sjson)
			{
				auto j = Json::parse(sjson);
				String json_class = j.at("class");
				if (json_class != pd::class_name<Path>())
					throw std::runtime_error("Invalid class name: " + pd::class_name<Path>());
				Path p = j;

				return clone(p);
			};
		};

		extern PathFactory pathf;
	}
}

#endif // PATH_FACTORY_HPP    

