// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef FILE_FACTORY_HPP
#define FILE_FACTORY_HPP

#include "factory.hpp"
#include "io_util.hpp"
#include "version_factory.hpp"	
#include "json_util.hpp"


namespace pensar_digital
{
	namespace cpplib
	{
		typedef Factory<File, Path, std::ios_base::openmode, Id> FileFactoryBase;
		class FileFactory : public FileFactoryBase
		{
		public:
			inline static const VersionPtr VERSION = pd::versionf.get(1, 1, 1);
			FileFactory(const Path& afull_path = ".", const std::ios_base::openmode amode = File::IN_OUT_ATE_BIN_MODE, const Id aid = NULL_ID) : FileFactoryBase(3, 10, afull_path, amode, aid) { };
			virtual ~FileFactory() { };
			using P = FileFactoryBase::P;
			virtual P get(const Path& afull_path = ".", const std::ios_base::openmode amode = File::IN_OUT_ATE_BIN_MODE, const Id aid = NULL_ID)
			{
				return FileFactoryBase::get (afull_path, amode, aid);
			};

			P clone(const File& afile)
			{
				return get(afile.get_full_path(), afile.get_mode (), afile.get_id ());
			}

			P clone(const FilePtr& ptr) { return clone(*ptr); }

			P parse_json(const String& sjson)
			{
				Json j;
				P ptr = get();
				std::stringstream ss(sjson);
				ss >> *ptr;
				return ptr;
			};
		};

		extern FileFactory filef;
	}
}

#endif // FILE_FACTORY_HPP    

