#include "object.hpp"
#include "string_def.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "ipath.hpp"
#include "version_factory.hpp"
#include "concept.hpp"
#include "json_util.hpp"
#include "io_util.hpp"
#include "clone_util.hpp"

#include <string>
#ifdef _MSC_VER
#include <io.h>
#include <windows.h>
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

namespace pensar_digital
{
    namespace cpplib
    {
        class Path;
        typedef std::shared_ptr<Path> PathPtr;
        // Path class represents a path in the file system. It inherits from fs::path adding implicit conversion to std::string.
        class Path : public fs::path, public Object, public Path
        {
        public:
            inline static const VersionPtr VERSION = pd::versionf.get(1, 1, 1);
            typedef Path    I;  // Interface type.
            typedef PathRO IRO; // Read only interface type.

            Path(const fs::path& p = ".", const Id& aid = NULL_ID) : Object(aid), fs::path(p) {}
            Path(const std::string& s = ".", const Id& aid = NULL_ID) : Object(aid), fs::path(s) {}
            Path(const char* s = ".", const Id& aid = NULL_ID) : Object(aid), fs::path(s) {}

            // Copy constructor.
            Path(const Path& p, const Id& aid = NULL_ID) noexcept : Object(aid), fs::path(p) {}

            // Move constructor.
            Path(Path&& p, const Id& aid = NULL_ID) noexcept : Object(aid), fs::path(p) {}

            // virtual destructor.
            virtual ~Path() noexcept = default;
            inline fs::path path() const noexcept { return static_cast<const fs::path&>(*this);}
            bool remove() const noexcept override;
            bool exists() const noexcept override;
			bool is_directory() const noexcept override;
            bool is_regular_file() const noexcept override;
            bool is_symlink() const noexcept override;
            bool is_empty() const noexcept override;
            Path& absolute() const noexcept override;
            Path& canonical() const noexcept override;
            Path& relative() const noexcept override;
            Path& parent_path() const noexcept override;
            Path& filename() const noexcept override;
            Path& stem() const noexcept override;
            Path& extension() const noexcept override;
            Path& replace_extension (const std::string& s) noexcept override;
            Path& replace_extension (const Path& p) override;
            Path& make_preferred() noexcept override;
            Path& remove_filename() noexcept override;
            Path& remove_trailing_separator() noexcept override;

            // Implements initialize method from Initializable concept.
            virtual bool initialize (const fs::path& p, const Id& aid = NULL_ID) noexcept
            {
                fs::path::operator = (p);
                Object::set_id(aid);
                return true;
            }

            PathPtr clone() const  noexcept { return pd::clone<Path, const fs::path&, const Id&> (*this, static_cast<const fs::path&>(*this), get_id()); }

            // Conversion to json string.
            virtual String json() const noexcept
            {
                std::stringstream ss(pd::json<Path>(*this));
                ss << ",path:" << this->fs::path::string() << "}";
                return ss.str();
            }

            virtual std::istream& read(std::istream& is, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN)
            {
                if (amode == BINARY)
                {
                    // todo: implement binary read.
                }
                else // json format
                {
                    Json j;
                    VersionPtr v;
                    Id stream_id;
                    pd::read_json<Path>(is, *this, &stream_id, &v, &j);
                    set_id(stream_id);
                    *this = j["path"].get<std::string>();

                    if (*VERSION != *v) throw std::runtime_error("Version mismatch.");
                }
                return is;
            };

            virtual std::ostream& write(std::ostream& os, const IO_Mode& amode = TEXT, const ByteOrder& abyte_order = LITTLE_ENDIAN) const
            {
                if (amode == BINARY)
                {
                    // todo: implement binary read.
                }
                else // json format
                {
                    return os << json();
                }
            };

            // Convertion to xml string.
            virtual String xml() const noexcept
            {
                String xml = ObjXMLPrefix() + "><path>";
                xml += fs::path::string() + "</path>";
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual void from_xml(const String& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                XMLNode n = node.getChildNode("path");
                if (!n.isEmpty()) *this = n.getText();
            }

            virtual String debug_string() const noexcept
            {
                return Object::debug_string() + " path = " + fs::path::string();
            }

            // Move assignment operator.
            Path& operator = (Path&& p) noexcept { fs::path::operator = (p); return *this; }

            // Assignment operator for std::string.
            Path& operator = (const std::string& s) { fs::path::operator = (s); return *this; }

            // Assignment operator for const Path&.
            Path& operator = (const Path& p) { fs::path::operator = (p); return *this; }

            void create_dir_if_does_not_exist() const noexcept
            {
                Path dir = *this;
                if (fs::path::has_filename())
                    dir = parent_path();
                if (!dir.exists())
                {
                    fs::create_directories(dir);
                }
            }

            inline fs::path to_std_path() const noexcept override { return static_cast<const fs::path&>(*this); }

            // Implicit conversion to fs::path.
            operator fs::path() const noexcept { return to_std_path(); }

            // Implicit conversion to std::string.
            operator std::string() const noexcept { return fs::path::string(); }

            // Implicit conversion to const char* returning a value allocated in the heap using _strdup.
            operator const char* () const { return _strdup(fs::path::string().c_str()); }

            // / operator for std::string.
            Path operator / (const std::string& s) const { Path p = *this; p /= s; p; }
            
            // /= operator for std::string.
            Path& operator /= (const std::string& s) { fs::path::operator /= (s); return *this; }

            // /= operator for const Path&.
            Path& operator /= (const Path& p) { fs::path::operator /= (p); return *this; }

            // + operator for std::string.s
            Path operator + (const std::string& s) const { Path p = *this; p /= s; return p; }

            // == operator 
            bool operator == (const Path& apath) const { return apath == fs::path::c_str(); }

            // != operator for std::string.
            bool operator != (const std::string& s) const { return !(*this == s); }

            // == operator for const char*.
            bool operator == (const char* s) const { return *this == std::string(s); }

            // != operator for const char*.
            bool operator != (const char* s) const { return !(*this == s); }
            friend void to_json(Json& j, const Path& d);
            friend void from_json(const Json& j, Path& d);

            // Inherited via Object
            inline void set_id(const Id& value) override { Object::set_id(value); }

            friend extern void to_json   (      Json& j, const Path& p);
            friend extern void from_json (const Json& j,       Path& p);


            // Inherited via Path
            bool is_absolute() const noexcept override;

            bool is_relative() const noexcept override;

            bool is_root() const noexcept override;

            bool is_root_directory() const noexcept override;

            bool is_root_path() const noexcept override;


            // Inherited via Path
            bool has_root_path() const noexcept override;

            bool has_root_name() const noexcept override;

            bool has_root_directory() const noexcept override;

            bool has_relative_path() const noexcept override;

            bool has_parent_path() const noexcept override;

            bool has_filename() const noexcept override;

            bool has_stem() const noexcept override;

            bool has_extension() const noexcept override;

};  // class Path
        // Json conversion.
        extern void to_json   (      Json& j, const Path& p);
        extern void from_json (const Json& j,       Path& p);
    } // namespace cpplib
} // namespace pensar_digital
