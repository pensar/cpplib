#include "dummy.hpp"

namespace pensar_digital
{
    namespace cpplib
    {
        /// Makes Dummy Streamable.
        std::ostream& operator << (std::ostream& os, const IDummy& d) { return d.write(os); }
        std::istream& operator >> (std::istream& is, IDummy& d) { return d.read(is); }

        void to_json(Json& j, const Dummy& d)
        {
            j["class"] = d.class_name();
            j["id"] = d.get_id();
            j["private_interface_version"] = d.VERSION.PRIVATE;
            j["protected_interface_version"] = d.VERSION.PROTECTED;
            j["public_interface_version"] = d.VERSION.PUBLIC;
            j["name"] = d.get_name();
        }

        void from_json(const Json& j, Dummy& d)
        {
            String class_name = d.class_name();
            String json_class = j.at("class");
            if (class_name == json_class)
            {
                d.set_id(j.at("id"));
                d.name = j.at("name");
            }
            else throw new std::runtime_error("Object expected class = " + class_name + " but json has " + json_class);
        }
    }
}