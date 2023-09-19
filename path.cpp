// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifdef CODE_GEAR
#pragma hdrstop
#endif

#include "io_util.hpp"
#include "path.hpp"

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
        
    }   // namespace cpplib
} // namespace pensar_digital