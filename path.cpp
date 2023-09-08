// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "io_util.hpp"
#include "path.hpp"
#include "path_factory.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>


namespace pensar_digital
{
    namespace cpplib
    {
        std::istream& operator >> (std::istream& is,       Path& path) { return path.read  (is); }
        std::ostream& operator << (std::ostream& os, const Path& path) { return path.write (os); }

        void to_json(Json& j, const Path& p)
        {
            j["class"] = p.class_name();
            j["id"] = p.get_id();
            j["VERSION"] = *p.VERSION;
            j["path"] = p;
        }

        void from_json(const Json& j, Path& p)
        {
            String class_name = p.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
            {
                p.set_id(j.at("id"));
                p = j.at("path");
            }
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
        
        bool Path::remove () const noexcept
        {
            if (exists())
            {
                return fs::remove(*this);
            }
            return true;
        }

        bool Path::exists () const noexcept
        {
            return fs::exists (*this);
        }

        bool Path::is_directory () const noexcept
        {
            return fs::is_directory (*this);
        }

        bool Path::is_regular_file () const noexcept
        {
            return fs::is_regular_file (*this);
        }

        bool Path::is_symlink () const noexcept
        {
            return fs::is_symlink (*this);
        }

        bool Path::is_empty() const noexcept
        {
            return fs::is_empty(*this);
        }

        Path& Path::absolute() const noexcept
        {
            return *(pathf.get (fs::absolute(*this)));
        }

        Path& Path::canonical() const noexcept
        {
            return *(pathf.get(fs::canonical(*this)));
        }

        Path& Path::relative() const noexcept
        {
            return *(pathf.get(fs::relative(*this)));
        }

        Path& Path::parent_path() const noexcept
        {
            return *(pathf.get(fs::path::parent_path()));
        }

        Path& Path::filename() const noexcept
        {
            return *(pathf.get(fs::path::filename()));
        }

        Path& Path::stem() const noexcept
        {
            return *(pathf.get(fs::path::stem()));
        }

        Path& Path::extension() const noexcept
        {
            return *(pathf.get (fs::path::extension()));
        }

        Path& Path::replace_extension(const std::string& s) noexcept
        {
            fs::path::replace_extension(s);
            return *this;
        }

        Path& Path::replace_extension(const Path& p) noexcept
        {
            
            p.path ().replace_extension(p.path ());
            return *this;
        }

        Path& Path::make_preferred() noexcept
        {
            fs::path::make_preferred();
            return *this;
        }

        Path& Path::remove_filename() noexcept
        {
            fs::path::remove_filename();
            return *this;
        }
        
        Path& Path::remove_trailing_separator() noexcept
        {
            String s = to_string ();
            if (s.back() == '/' || s.back() == '\\')
			{
				s.pop_back();
				*this = s;
			}
			return *this;
        }
        bool Path::is_absolute() const noexcept
        {
            return path ().is_absolute ();
        }
        bool Path::is_relative() const noexcept
        {
            return path ().is_relative ();
        }
        bool Path::is_root() const noexcept
        {
            return path().root_path () != path (); // todo: check if this is correct    
        }
        bool Path::is_root_directory() const noexcept
        {
            return path ().root_directory () == path ();
        }
        bool Path::is_root_path() const noexcept
        {
            return path ().root_path () == path ();
        }
        bool Path::has_root_path() const noexcept
        {
            return path ().has_root_path ();
        }
        bool Path::has_root_name() const noexcept
        {
            return path ().has_root_name ();
        }
        bool Path::has_root_directory() const noexcept
        {
            return path ().has_root_directory ();
        }
        bool Path::has_relative_path() const noexcept
        {
            return path ().has_relative_path ();
        }
        bool Path::has_parent_path() const noexcept
        {
            return path ().has_parent_path ();
        }
        bool Path::has_filename() const noexcept
        {
            return path ().has_filename ();
        }
        bool Path::has_stem() const noexcept
        {
            return path ().has_stem ();
        }
        bool Path::has_extension() const noexcept
        {
            return path ().has_extension ();
        }
}   // namespace cpplib
} // namespace pensar_digital