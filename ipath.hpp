// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef IPATH_HPP
#define IPATH_HPP

#include "constant.hpp"
#include "string_def.hpp"
#include "header_lib/json.hpp"
#include "version_factory.hpp"
#include "iobject.hpp"

#include <iostream>
#include <memory> // for std::shared_ptr
#ifdef _MSC_VER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

namespace pensar_digital
{
    namespace cpplib
    {
#ifdef _MSC_VER
        namespace fs = std::filesystem;
#else
        namespace fs = std::experimental::filesystem;
#endif
        class IPath;
        class IPathRO;
        typedef std::shared_ptr<IPathRO> IPath_RO_Ptr;
        typedef std::shared_ptr<IPath> IPathPtr;

        // / \brief Base class for all Paths. Read only interface of Path.
        class IPathRO : public virtual IObjectRO
        {
        public:
            inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);

            virtual ~IPathRO() noexcept = default;
            virtual fs::path to_std_path() const noexcept = 0;
        };

        class IPath : public virtual IPathRO, public virtual IObject
        {
        public:
            inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);

            virtual ~IPath() noexcept = default;

            virtual std::istream& read(std::istream& is) = 0;

            // Convertion from xml string.
            virtual void from_xml(const String& sxml) = 0;
        };
        //extern Factory<IPath, Id, String> Path_factory;

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // IPATH_HPP
