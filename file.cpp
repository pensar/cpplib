#include "file.hpp"
#include "path.hpp"


namespace pensar_digital
{
    namespace cpplib
    {
        std::istream& operator >> (std::istream& is,       File<char>& file) { return file.read  (is); }
        std::ostream& operator << (std::ostream& os, const File<char>& file) { return file.write (os); }

        void to_json(Json& j, const File<char>& f)
         {
             j["class"] = f.class_name();
             j["id"] = f.id();
             j["mpublic"] = f.VERSION->get_public();
             j["mprotected"] = f.VERSION->get_protected();
             j["mprivate"] = f.VERSION->get_private();
             j["path"] = f.fullpath ();
             j["mode"] = f.is_binary() ? "BINARY" : "TEXT";
         }

         void from_json(const Json& j, File<char>& f)
         {
             String class_name = f.class_name();
             String json_class = j.at("class");
             if (class_name == json_class)
             {
                 f.set_id(j.at("id"));
                 f.mfullpath = j.at("mfullpath");
                 if (j.at("mode") == "BINARY")
                 {
                     f.set_binary_mode ();
                 }
                 else
                 {
                    f.set_text_mode ();
                 }
             }
             else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
         }

    }   // namespace cpplib
} // namespace pensar_digital
