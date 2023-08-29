// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef IFILE_HPP
#define IFILE_HPP

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
        class IFile;
        class IFileRO;
        typedef std::shared_ptr<IFileRO> IFile_RO_Ptr;
        typedef std::shared_ptr<IFile> IFilePtr;

        // / \brief Base class for all Files. Read only interface of File.
        class IFileRO : public virtual IObjectRO
        {
        public:
            inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);

            virtual ~IFileRO() noexcept = default;

            virtual fs::path get_full_path() const noexcept = 0;
            virtual std::ios_base::openmode get_mode() const noexcept = 0;
        };

        class IFile : public virtual IFileRO, public virtual IObject
        {
        public:
            inline static const IVersionPtr VERSION = pd::versionf.get(1, 1, 1);

            virtual ~IFile() noexcept = default;

            virtual std::istream& read (std::istream& os, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) = 0;
            virtual void set_name(const String& name) noexcept = 0;

            // Convertion from xml string.
            virtual void from_xml(const String& sxml) = 0;
        };
        //extern Factory<IFile, Id, String> File_factory;

    }  // namespace cpplib
}      // namespace pensar_digital
#endif // IFILE_HPP
