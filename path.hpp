// license: MIT (https://opensource.org/licenses/MIT)

#ifndef PATH_HPP
#define PATH_HPP

#include "object.hpp"
#include "s.hpp"
#include "system.hpp"

#include "string_def.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "version.hpp"
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
        namespace fs = std::filesystem;
        namespace pd = pensar_digital::cpplib;  

        class Path;
        typedef std::shared_ptr<Path> PathPtr;
        typedef Factory<Path, fs::path, Id> PathFactory;

        // Path class represents a path in the file system. It inherits from fs::path adding implicit conversion to std::string.
        class Path : public fs::path, public Object
        {
            private:
                inline static PathFactory mfactory = { 3, 10, ".", NULL_ID};
            public:
                inline static const VersionPtr VERSION = pd::Version::get (1, 1, 1);

            Path(const fs::path& p = ".", const Id& id = NULL_ID) : Object(id), fs::path(p) {}
            Path(const std::string& s, const Id& id = NULL_ID) : Object(id), fs::path(s) {}
            Path(const char* s, const Id& id = NULL_ID) : Object(id), fs::path(s) {}

            // Copy constructor.
            Path(const Path& p, const Id& aid = NULL_ID) noexcept : Object(aid), fs::path(p) {}

            // Move constructor.
            Path(Path&& p, const Id& aid = NULL_ID) noexcept : Object(aid), fs::path(p) {}

            // virtual destructor.
            virtual ~Path() noexcept = default;
            static PathFactory::P get(const fs::path& p = ".", const Id& aid = NULL_ID)
            {
                return mfactory.get (p, aid);
            };

            PathFactory::P clone(const Path& apath)
            {
                return get(apath.std_path(), apath.id());
            };

            PathFactory::P clone(const PathPtr& ptr) { return clone(*ptr); }
           
            PathFactory::P parse_json(const S& sjson)
            {
                auto j = Json::parse(sjson);
                S json_class = j.at("class");
                if (json_class != pd::class_name<Path, char>())
                    throw std::runtime_error("Invalid class name: " + pd::class_name<Path, char>());
                Id id;
                VersionPtr v;
                Path p;
                pd::read_json<Path>(sjson, p, &id, &v, &j);
				set_id(id);

                return clone(p);
            };

            bool remove() const noexcept
            {
                if (exists())
                {
                    return fs::remove(*this);
                }
                return true;
            }

            bool exists() const noexcept
            {
                return fs::exists(*this);
            }

            Path absolute() const noexcept
            {
                return fs::absolute(*this);
            }

            Path canonical() const noexcept
            {
                return fs::canonical(*this);
            }

            Path relative() const noexcept
            {
                return fs::relative(*this);
            }

            Path parent_path() const noexcept
            {
                return fs::path::parent_path();
            }

            Path filename() const noexcept
            {
                return fs::path::filename();
            }

            Path stem() const noexcept
            {
                return fs::path::stem();
            }

            Path extension() const noexcept
            {
                return fs::path::extension();
            }

            Path& replace_extension(const std::string& s) noexcept
            {
                fs::path::replace_extension(s);
                return *this;
            }

            Path& replace_extension(const Path& p) noexcept
            {
                fs::path::replace_extension(p);
                return *this;
            }

            Path& make_preferred() noexcept
            {
                fs::path::make_preferred();
                return *this;
            }

            Path& remove_filename() noexcept
            {
                fs::path::remove_filename();
                return *this;
            }

            Path& remove_trailing_separator() noexcept
            {
                //fs::path::remove_trailing_separator (); // todo.
                return *this;
            }

            // Implements initialize method from Initializable concept.
            virtual bool initialize(const fs::path& p, const Id& aid = NULL_ID) noexcept
            {
                fs::path::operator = (p);
                Object::set_id(aid);
                return true;
            }

            // Conversion to json string.
            virtual S json() const noexcept
            {
                std::stringstream ss(pd::json<Path>(*this));
                ss << ",path:" << this->fs::path::string() << "}";
                return ss.str();
            }

            virtual std::istream& read(std::istream& is, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native)
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

            virtual std::ostream& write(std::ostream& os, const IO_Mode amode = TEXT, const std::endian& byte_order = std::endian::native) const
            {
                if (amode == BINARY)
                {
                    // todo: implement binary read.
                }
                else // json format
                {
                    os << json();
                }
                return os;
            };

            // Convertion to xml string.
            virtual S xml() const noexcept
            {
                S xml = ObjXMLPrefix() + "><path>";
                xml += fs::path::string() + "</path>";
                xml += "</object>";
                return xml;
            }

            // Convertion from xml string.
            virtual void from_xml(const S& sxml)
            {
                XMLNode node = parse_object_tag(sxml);
                XMLNode n = node.getChildNode("path");
                if (!n.isEmpty()) *this = n.getText();
            }

            virtual S debug_string() const noexcept
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
                if (has_filename())
                    dir = parent_path();
                if (!dir.exists())
                {
                    fs::create_directories(dir);
                }
            }

            inline fs::path std_path() const noexcept { return static_cast<const fs::path&>(*this); }

            // Implicit conversion to fs::path.
            operator fs::path() const noexcept { return std_path(); }

            // Implicit conversion to std::string.
            operator std::string() const noexcept { return fs::path::string(); }

            // Implicit conversion to const char* returning a value allocated in the heap using _strdup.
            operator const char* () const { return _strdup(fs::path::string().c_str()); }

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
            inline void set_id(const Id& value) { Object::set_id(value); }
        };  // class Path

        static inline Path CURRENT_DIR = ".";

        template <typename C = char>
        class CPath : public CS<MAX_UNC_PATH, C>
        {
            public:
                typedef CS<MAX_UNC_PATH, C> Str;
                CPath(const Str& path = ".") : Str(path) {}

                // Constructor from fs::path.
                //CPath (const fs::path& path) : Str (path.string()) {}

                inline fs::path to_fspath() const noexcept { return fs::path(this->Str::std_str()); }
                inline Path     to_path() const noexcept { return Path(this->Str::std_str()); }

                // Conversion to fs::path operator.
                operator fs::path() const noexcept { return to_fspath(); }
        };

        // Json conversion.
        extern void to_json   (      Json& j, const Path& p);
        extern void from_json (const Json& j,       Path& p);
    } // namespace cpplib
} // namespace pensar_digital
#endif  